#include "application_solar.hpp"
#include "launcher.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"

#include <glbinding/gl/gl.h>
// use gl definitions from glbinding 
using namespace gl;

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}
{
  initializeGeometry();
  initializeShaderPrograms();
}

void ApplicationSolar::upload_planet_transforms(Planet const& planet) const{
    // bind shader to upload uniforms
    glUseProgram(m_shaders.at("planet").handle);

    glm::fvec3 turning_axis{0.0f, 1.0f, 0.0f};

    glm::fmat4 model_matrix;

    //check wether the planet has a reference planet
    if(planet.reference_planet != nullptr)
    {
      Planet refPlan = *planet.reference_planet;
      
      //model_matrix *= glm::rotate(glm::fmat4{}, float(glfwGetTime() * refPlan.rotationSpeed), turning_axis) * glm::rotate(glm::fmat4{}, float(glfwGetTime() * planet.rotationSpeed), turning_axis);
      model_matrix *= glm::rotate(glm::fmat4{}, float(glfwGetTime() * refPlan.rotationSpeed), turning_axis);
      //model_matrix *= glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, refPlan.distance}) * glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, planet.distance });
      model_matrix *= glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, refPlan.distance});
    }

    model_matrix *= glm::rotate(glm::fmat4{}, float(glfwGetTime() * planet.rotationSpeed), turning_axis);
    model_matrix *= glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, planet.distance});


    model_matrix = glm::scale(model_matrix, glm::fvec3{ planet.size, planet.size, planet.size});

    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                       1, GL_FALSE, glm::value_ptr(model_matrix));

    // extra matrix for normal transformation to keep them orthogonal to surface
    glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);

    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                       1, GL_FALSE, glm::value_ptr(normal_matrix));

    // bind the VAO to draw
    glBindVertexArray(planet_object.vertex_AO);

    // draw bound vertex array using bound shader
    glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);  
}

void ApplicationSolar::render() const {
  
  //Planet sun{ 1.0f, 0.0f, 0.0f};
  Planet earth{ 0.14f,  0.3f, 7.0f};
  Planet moon{ 0.03f, 0.9f, 0.3f, &earth};

  std::vector<Planet> planets;

  planets = { Planet{ 1.0f, 0.0f, 0.0f},
              Planet{ 0.5f,  1.5f, 5.0f}, //mercury
              Planet{ 0.25f,  1.3f, 6.6f}, //venus
              earth,
              moon,
              Planet{ 0.4f, 1.0f, 9.0f}, //mars
              Planet{ 0.43f, 0.9f, 12.0f}, //jupiter
              Planet{ 0.22f, 0.6f, 14.0f}, //saturn
              Planet{ 0.42f, 0.3f, 16.0f}, //uranus
              Planet{ 0.15f, 0.4f, 19.0f} //neptune
            };

  for (std::vector<Planet>::iterator i = planets.begin(); i != planets.end(); ++i)
  {
    upload_planet_transforms(*i);
  }
}

void ApplicationSolar::updateView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::updateProjection() {
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
}

// update uniform locations
void ApplicationSolar::uploadUniforms() {
  updateUniformLocations();
  
  // bind new shader
  glUseProgram(m_shaders.at("planet").handle);
  
  updateView();
  updateProjection();
}

// handle key input
void ApplicationSolar::keyCallback(int key, int scancode, int action, int mods) {
  //Button: W Action: move front
  if (key == GLFW_KEY_W && action == GLFW_PRESS) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -1.0f});
    updateView();
  } //Button: S Action: move back
  else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 1.0f});
    updateView();
  } //Button: A Action: move left
  else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{-1.0f, 0.0f, 0.0f});
    updateView();
  } //Button: D Action: move right
  else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{1.0f, 0.0f, 0.0f});
    updateView();
  } //Button: DOWN Action: move down (such wow)
  else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, -1.0f, 0.0f});
    updateView();
  } //Button: UP Action: move up (very suprise)
  else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 1.0f, 0.0f});
    updateView();
  } //Button: O Action: sets the view way back (Overview)
  else if (key == GLFW_KEY_O && action == GLFW_PRESS) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 10.0f});
    updateView();
  }
}

// load shader programs
void ApplicationSolar::initializeShaderPrograms() {
  // store shader program objects in container
  m_shaders.emplace("planet", shader_program{m_resource_path + "shaders/simple.vert",
                                           m_resource_path + "shaders/simple.frag"});
  // request uniform locations for shader program
  m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
}

// load models
void ApplicationSolar::initializeGeometry() {
  model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);

  // generate vertex array object
  glGenVertexArrays(1, &planet_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(planet_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &planet_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, planet_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planet_model.data.size(), planet_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::POSITION]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::NORMAL]);

   // generate generic buffer
  glGenBuffers(1, &planet_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model.indices.size(), planet_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  planet_object.draw_mode = GL_TRIANGLES;
  // transfer number of indices to model object 
  planet_object.num_elements = GLsizei(planet_model.indices.size());
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
}

// exe entry point
int main(int argc, char* argv[]) {
  Launcher::run<ApplicationSolar>(argc, argv);
}