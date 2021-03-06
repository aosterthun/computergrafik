#include "application_solar.hpp"
#include "launcher.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"
#include "texture_loader.hpp"
#include "material.hpp"

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

#define NUM_STARS 200000 //amount of stars set to the scene

#define SHADER_PLANET 0
#define SHADER_SUN 1
#define SHADER_SKYBOX 2
#define SHADER_MOON 3
#define PLANET_SCALE 0.01
#define ORBIT_SCALE 1

ApplicationSolar::ApplicationSolar(std::string const& resource_path):
 Application{resource_path},
 planet_object{},
 planets{},
 texture_map{},
 stars_object{},
 stars_indices{},
 stars_f{},
 screen_quad{},
 effect_map{}
{
  create_scene();
  create_sq();

  effect_map.insert(std::pair<std::string, int>("Luminance", 0));
  effect_map.insert(std::pair<std::string, int>("Vertically", 0));
  effect_map.insert(std::pair<std::string, int>("Horizontally", 0));
  effect_map.insert(std::pair<std::string, int>("Gaussian", 0));

  initializeFrameBuffer(800u, 800u);
  initializeGeometry();
  initializeTextures();
  initializeShaderPrograms();
}

void ApplicationSolar::create_scene() {

  Material sun_material{{1.0f, 0.5f, 0.2f}};
  Material earth_material{{0.0f, 0.0f, 1.0f}};
  Material moon_material{{0.4f, 0.4f, 0.4f}};
  Material mercury_material{{0.0f, 1.0f, 1.0f}};
  Material venus_material{{0.5f, 1.0f, 0.0f}};
  Material mars_material{{0.0f, 0.0f, 1.0f}};
  Material jupiter_material{{1.0f, 0.0f, 0.0f}};
  Material saturn_material{{0.0f, 0.0f, 1.0f}};
  Material uranus_material{{1.0f, 1.0f, 0.0f}};
  Material neptun_material{{1.0f, 1.0f, 0.0f}};

  /*
    Planets
  */
  //std::shared_ptr<Planet> sun_ptr      = std::make_shared<Planet>("Sun", 1.0f, 0.03f,15.0f, glm::fvec3{1.0f, 0.5f, 0.2f});
  // std::shared_ptr<Planet> sun_ptr      = std::make_shared<Planet>("Sun",      18.0f * PLANET_SCALE, 0.0f, 0.0f * ORBIT_SCALE,   sun_material, SHADER_SUN);
  // std::shared_ptr<Planet> earth_ptr    = std::make_shared<Planet>("Earth",    1.0f * PLANET_SCALE,  0.3f, 1.0f * ORBIT_SCALE, earth_material, SHADER_PLANET, sun_ptr);
  // std::shared_ptr<Planet> moon_ptr     = std::make_shared<Planet>("Moon",     0.03f * PLANET_SCALE, 0.9f, 0.3f * ORBIT_SCALE,  moon_material, SHADER_PLANET, earth_ptr);
  // std::shared_ptr<Planet> m_o_m_ptr    = std::make_shared<Planet>("Moon",     0.01f * PLANET_SCALE, 1.1f, 0.09f * ORBIT_SCALE, moon_material, SHADER_PLANET, moon_ptr);
  // std::shared_ptr<Planet> mercury_ptr  = std::make_shared<Planet>("Mercury",  0.38f * PLANET_SCALE,  1.5f, 0.387f * ORBIT_SCALE,  mercury_material,SHADER_PLANET, sun_ptr);
  // std::shared_ptr<Planet> venus_ptr    = std::make_shared<Planet>("Venus",    0.95f * PLANET_SCALE,  1.3f, 0.723f * ORBIT_SCALE, venus_material, SHADER_PLANET, sun_ptr);
  // std::shared_ptr<Planet> mars_ptr     = std::make_shared<Planet>("Mars",     0.53f * PLANET_SCALE, 1.0f, 1.52f * ORBIT_SCALE,   mars_material, SHADER_PLANET, sun_ptr);
  // std::shared_ptr<Planet> jupiter_ptr  = std::make_shared<Planet>("Jupiter",  11.19f * PLANET_SCALE, 0.9f, 5.20f * ORBIT_SCALE, jupiter_material, SHADER_PLANET, sun_ptr);
  // std::shared_ptr<Planet> saturn_ptr   = std::make_shared<Planet>("Saturn",   9.4f * PLANET_SCALE, 0.6f, 9.58f * ORBIT_SCALE, saturn_material, SHADER_PLANET, sun_ptr);
  // std::shared_ptr<Planet> uranus_ptr   = std::make_shared<Planet>("Uranus",   4.04f * PLANET_SCALE, 0.3f, 19.20f * ORBIT_SCALE, uranus_material, SHADER_PLANET, sun_ptr);
  // std::shared_ptr<Planet> neptune_ptr  = std::make_shared<Planet>("Neptune",  3.88f * PLANET_SCALE, 0.4f, 30.05f * ORBIT_SCALE, neptun_material, SHADER_PLANET, sun_ptr);

  std::string texture_dir = m_resource_path + "/textures/";

  std::shared_ptr<Planet> skybox      = std::make_shared<Planet>("Skybox",      70.0f, 0.0f, 0.0f,   texture_dir + "skybox.png"    , SHADER_SKYBOX);

  std::shared_ptr<Planet> sun_ptr      = std::make_shared<Planet>("Sun",      1.0f, 0.0f, 0.0f,   texture_dir + "sun.png"    , SHADER_SUN);
  std::shared_ptr<Planet> earth_ptr    = std::make_shared<Planet>("Earth",    0.14f,  0.3f, 7.0f, texture_dir + "earth.png"  , SHADER_PLANET, sun_ptr);
  std::shared_ptr<Planet> moon_ptr     = std::make_shared<Planet>("Moon",     0.03f, 0.9f, 0.3f,  moon_material               , SHADER_MOON, earth_ptr);
  std::shared_ptr<Planet> m_o_m_ptr    = std::make_shared<Planet>("Moon",     0.01f, 1.1f, 0.09f, moon_material               , SHADER_MOON, moon_ptr);
  std::shared_ptr<Planet> mercury_ptr  = std::make_shared<Planet>("Mercury",  0.5f,  1.5f, 5.0f,  texture_dir + "mercury.png" ,SHADER_PLANET, sun_ptr);
  std::shared_ptr<Planet> venus_ptr    = std::make_shared<Planet>("Venus",    0.25f,  1.3f, 6.6f, texture_dir + "venus.png" ,SHADER_PLANET, sun_ptr);
  std::shared_ptr<Planet> mars_ptr     = std::make_shared<Planet>("Mars",     0.4f, 1.0f, 9.0f,   texture_dir + "mars.png" ,SHADER_PLANET, sun_ptr);
  std::shared_ptr<Planet> jupiter_ptr  = std::make_shared<Planet>("Jupiter",  0.43f, 0.9f, 12.0f, texture_dir + "jupiter.png" ,SHADER_PLANET, sun_ptr);
  std::shared_ptr<Planet> saturn_ptr   = std::make_shared<Planet>("Saturn",   0.22f, 0.6f, 14.0f, texture_dir + "saturn.png" ,SHADER_PLANET, sun_ptr);
  std::shared_ptr<Planet> uranus_ptr   = std::make_shared<Planet>("Uranus",   0.42f, 0.3f, 16.0f, texture_dir + "uranus.png" ,SHADER_PLANET, sun_ptr);
  std::shared_ptr<Planet> neptune_ptr  = std::make_shared<Planet>("Neptune",  0.15f, 0.4f, 19.0f, texture_dir + "neptune.png" ,SHADER_PLANET, sun_ptr);

  planets.push_back(skybox);

  planets.push_back(sun_ptr);
  planets.push_back(earth_ptr);
  planets.push_back(moon_ptr);
  planets.push_back(m_o_m_ptr);
  planets.push_back(mercury_ptr);
  planets.push_back(venus_ptr);
  planets.push_back(mars_ptr);
  planets.push_back(jupiter_ptr);
  planets.push_back(saturn_ptr);
  planets.push_back(uranus_ptr);
  planets.push_back(neptune_ptr);

  //setting texture indices
  for(int i = 0; i < planets.size(); ++i)
  {
    planets.at(i)->texture_handle = i;
  }

  /*
    Stars
  */
  for (int i = 0; i < NUM_STARS; ++i)
  {
    stars_indices.push_back(i);

    float x, y, z, r , g, b;

    //position
    x = utils::RandomFloat( -100.0f, 100.0f);
    y = utils::RandomFloat( -100.0f, 100.0f);
    z = utils::RandomFloat( -100.0f, 100.0f);

    //color (konfetti atm)
    r = utils::RandomFloat( 0.0f, 1.0f);
    g = utils::RandomFloat( 0.0f, 1.0f);
    b = utils::RandomFloat( 0.0f, 1.0f);

    stars_f.push_back(x);
    stars_f.push_back(y);
    stars_f.push_back(z);
    stars_f.push_back(r);
    stars_f.push_back(g);
    stars_f.push_back(b);
  }
}

void ApplicationSolar::create_sq(){
  
  std::vector<float> properties {
          -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // v1
           1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // v2
          -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, // v4
           1.0f,  1.0f, 0.0f, 1.0f, 1.0f  // v3
         };
  
  model sq_model = {properties, model::POSITION | model::TEXCOORD};
  // generate vertex array object
  glGenVertexArrays(1, &screen_quad.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(screen_quad.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &screen_quad.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, screen_quad.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sq_model.data.size(), sq_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, sq_model.vertex_bytes, sq_model.offsets[model::POSITION]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::TEXCOORD.components, model::TEXCOORD.type, GL_FALSE, sq_model.vertex_bytes, sq_model.offsets[model::TEXCOORD]);

}

void ApplicationSolar::upload_planet_transforms(std::shared_ptr<Planet> const& planet) const{


    /*
      Model matrix
    */
    glm::fmat4 mo_ma;

    mo_ma *= model_matrix(planet);
    mo_ma = glm::scale(mo_ma, glm::fvec3{ planet->size, planet->size, planet->size});
    
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                       1, GL_FALSE, glm::value_ptr(mo_ma));

    // extra matrix for normal transformation to keep them orthogonal to surface
    glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * mo_ma);

    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                       1, GL_FALSE, glm::value_ptr(normal_matrix));

    /*
      Lighting
    */
    //retrieving sun position
    std::shared_ptr<Planet> sun;
    if(planet->reference_planet == nullptr){ //current planet is sun
      sun = planet;
    }
    else{ //retrieving sun
      
      sun = planet->reference_planet;
      
      while(sun->name != "Sun")
      {
        sun = sun->reference_planet;
      }
    }

    glm::fmat4 mo_ma_sun = model_matrix(sun);

    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("SunPosition"), 1, GL_FALSE, glm::value_ptr(mo_ma_sun));

    //Material properties
    glUniform3f(m_shaders.at("planet").u_locs.at("ColorAmbient"), planet->material.a.x,
                                                                  planet->material.a.y,
                                                                  planet->material.a.z);
    glUniform1f(m_shaders.at("planet").u_locs.at("Glossiness"),   planet->material.g);
    glUniform1i(m_shaders.at("planet").u_locs.at("PlanetType"),   planet->planetType);

    /*
      Texturing
    */
    //std::cout << "Loading: " << planet->texture_path << std::endl;
    if(texture_map.find(planet->name) != texture_map.end())
    {
      auto texture_object = texture_map.at(planet->name);
      
      glActiveTexture(GL_TEXTURE0+planet->texture_handle);
      glBindTexture(texture_object.target, texture_object.handle);

      int sampler_location = glGetUniformLocation(m_shaders.at("planet").handle, "pass_TexColor");
      glUniform1i(sampler_location, planet->texture_handle);
    }
    else
    {
      //moons without textures are handled by SHADER_MOON within the shader
    }





    // bind the VAO to draw
    glBindVertexArray(planet_object.vertex_AO);

    // draw bound vertex array using bound shader
    glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);  
}

void ApplicationSolar::render() const {

  /*
    Framebuffer [1]
  */

  glBindFramebuffer(GL_FRAMEBUFFER, sq_handle);

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  /*
    Stars
  */
  // bind shader to upload uniforms
  glUseProgram(m_shaders.at("star").handle);

  //bind VAO of geometry
  glBindVertexArray(stars_object.vertex_AO);
  //draw geometry
  glDrawElements(stars_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

  /*
    Planets
  */
  // bind shader to upload uniforms
  glUseProgram(m_shaders.at("planet").handle);

  for(std::vector<std::shared_ptr<Planet>>::const_iterator i = planets.begin(); i != planets.end(); ++i)
  {
    upload_planet_transforms(*i);
  }

  /*
    Framebuffer [2]
  */
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glUseProgram(m_shaders.at("screenquad").handle);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, sq_texture.handle);
  int color_location = glGetUniformLocation(m_shaders.at("screenquad").handle, "ColorTex");
  glUniform1i(color_location, 0);

  glBindVertexArray(screen_quad.vertex_AO);
  utils::validate_program(m_shaders.at("screenquad").handle);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}

void ApplicationSolar::updateView() {

  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  
  /*
    Planets
  */
  glUseProgram(m_shaders.at("planet").handle);
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));

  /*
    Stars
  */
  glUseProgram(m_shaders.at("star").handle);

  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("star").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));

  glUseProgram(m_shaders.at("planet").handle); //reset to planet
}

void ApplicationSolar::updateProjection() {
  /*
    Framebuffer 
  */
  initializeFrameBuffer(800u,800u);

  /*
    Planets
  */
  glUseProgram(m_shaders.at("planet").handle);

  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));

  /*
    Stars
  */
  glUseProgram(m_shaders.at("star").handle);

  glUniformMatrix4fv(m_shaders.at("star").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));

  
  glUseProgram(m_shaders.at("planet").handle); //reset to planet
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
  else if(key == GLFW_KEY_7 && action == GLFW_PRESS){ //luminance
    int status = effect_map.at("Luminance");

    effect_map.at("Luminance") = (status == 0) ? 1 : 0; //flip 0 <-> 1

    glUseProgram(m_shaders.at("screenquad").handle);
    auto loc = glGetUniformLocation(m_shaders.at("screenquad").handle, "Luminance");
    glUniform1ui(loc, status);
  }
  else if(key == GLFW_KEY_8 && action == GLFW_PRESS){ //Vertically
    int status = effect_map.at("Vertically");
    
    effect_map.at("Vertically") = (status == 0) ? 1 : 0; //flip 0 <-> 1

    glUseProgram(m_shaders.at("screenquad").handle);
    auto loc = glGetUniformLocation(m_shaders.at("screenquad").handle, "Vertically");
    glUniform1ui(loc, status);
  }
  else if(key == GLFW_KEY_9 && action == GLFW_PRESS){ //Horizontally
    int status = effect_map.at("Horizontally");

    effect_map.at("Horizontally") = (status == 0) ? 1 : 0; //flip 0 <-> 1

    glUseProgram(m_shaders.at("screenquad").handle);
    auto loc = glGetUniformLocation(m_shaders.at("screenquad").handle, "Horizontally");
    glUniform1ui(loc, status);
  }
  else if(key == GLFW_KEY_0 && action == GLFW_PRESS){ //Gaussian
    int status = effect_map.at("Gaussian");

    effect_map.at("Gaussian") = (status == 0) ? 1 : 0; //flip 0 <-> 1

    glUseProgram(m_shaders.at("screenquad").handle);
    auto loc = glGetUniformLocation(m_shaders.at("screenquad").handle, "Gaussian");
    glUniform1ui(loc, status);
  }
}

// handle mouse input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y)
{
  // mouse handling
  m_view_transform = glm::rotate(m_view_transform, (float)(pos_y/100), glm::fvec3(-1.0f,0.0f,0.0f));
  m_view_transform = glm::rotate(m_view_transform, (float)(pos_x/100), glm::fvec3(0.0f,-1.0f,0.0f));
  updateView();
}


// load shader programs
void ApplicationSolar::initializeShaderPrograms() {
  /*
    Framebuffer
  */
  //initialize screen squad shader
  m_shaders.emplace("screenquad", shader_program{m_resource_path + "shaders/screenquad.vert",
                                                m_resource_path + "shaders/screenquad.frag"});
  /*
    Planet shader
  */
  // store shader program objects in container
  m_shaders.emplace("planet", shader_program{m_resource_path + "shaders/simple.vert",
                                           m_resource_path + "shaders/simple.frag"});
  // request uniform locations for shader program
  m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;

  m_shaders.at("planet").u_locs["SunPosition"] = -1;

  m_shaders.at("planet").u_locs["ColorAmbient"] = 1;
  m_shaders.at("planet").u_locs["Glossiness"] = 1;
  m_shaders.at("planet").u_locs["PlanetType"] = 1;

  /*
    Star shader
  */
  // store shader program objects in container
  m_shaders.emplace("star", shader_program{m_resource_path + "shaders/star.vert",
                                          m_resource_path + "shaders/star.frag"});
  m_shaders.at("star").u_locs["ViewMatrix"] = -1;
  m_shaders.at("star").u_locs["ProjectionMatrix"] = -1;
}

// load models
void ApplicationSolar::initializeGeometry() {

  /*
    Planet
  */
  model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL | model::TEXCOORD);

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

  //Textures
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, model::TEXCOORD.components, model::TEXCOORD.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::TEXCOORD]);

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


  /*
    Star
  */
  // generate vertex array object
  glGenVertexArrays(1, &stars_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(stars_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &stars_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, stars_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * stars_f.size(), stars_f.data() , GL_STATIC_DRAW);

  // first attribute: position
  glEnableVertexAttribArray(0);
  // index, num_components, data_type, normalize, stride, offset
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, NULL);
  
  //second attribute: color
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (GLvoid*)uintptr_t(sizeof(float)*3));

  // generate generic buffer
  glGenBuffers(1, &stars_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, stars_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * NUM_STARS, stars_indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  stars_object.draw_mode = GL_POINTS;
  // transfer number of indices to model object 
  stars_object.num_elements = GLsizei(NUM_STARS);
}

void ApplicationSolar::initializeTextures() {

  for(int i = 0; i < planets.size(); ++i)
  {
    if(planets.at(i)->texture_path != "NONE")
    {   
      pixel_data texture_data = texture_loader::file(planets.at(i)->texture_path);

      texture_object tex_obj = utils::create_texture_object(texture_data);

      texture_map.insert(std::pair<std::string, texture_object>(planets.at(i)->name, tex_obj));
    }
    else
    {
      std::cout << "No texture given for planet " << planets.at(i)->name << std::endl;
    }
  }
}

void ApplicationSolar::initializeFrameBuffer(unsigned const& width, unsigned const& height){
  //create render buffer
  GLuint rb_handle = 0;
  glGenRenderbuffers(1, &rb_handle);
  glBindRenderbuffer(GL_RENDERBUFFER, rb_handle);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height); 

  //texture with color attechment 0
  //GLuint tex_handle = 0;
  glGenTextures(1, &sq_texture.handle);
  glBindTexture(GL_TEXTURE_2D, sq_texture.handle);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GLint(GL_RGB), width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

  //frame buffer
  glGenFramebuffers(1, &sq_handle);
  glBindFramebuffer(GL_FRAMEBUFFER, sq_handle);

  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, sq_texture.handle,0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb_handle);

  GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, draw_buffers);
}



ApplicationSolar::~ApplicationSolar() {
  //planets
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);

  //stars
  glDeleteBuffers(1, &stars_object.vertex_BO);
  glDeleteBuffers(1, &stars_object.element_BO);
  glDeleteVertexArrays(1, &stars_object.vertex_AO);
}

// exe entry point
int main(int argc, char* argv[]) {
  Launcher::run<ApplicationSolar>(argc, argv);
}