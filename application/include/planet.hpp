#ifndef APPLICATION_PLANET_HPP
#define APPLICATION_PLANET_HPP

#include <memory>
#include <glbinding/gl/gl.h>

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <glm/gtc/type_ptr.hpp>

struct Planet
{
  //CONSTRUCTORS
  Planet( std::string n, float s, float r, float d, glm::fvec3 const& c):
  name{n},
  size{s},
  rotationSpeed{r},
  ka{c},
  kg{1.0f},
  turningAxis{0.0f, 1.0f, 0.0f},
  distance{d},
  reference_planet{nullptr}
  {}

  Planet( std::string n, float s, float r, glm::fvec3 a, float d):
  name{n},
  size{s},
  rotationSpeed{r},
  ka{1.0, 1.0, 1.0},
  kg{1.0f},
  turningAxis{a},
  distance{d},
  reference_planet{nullptr}
  {}

  Planet( std::string n, float s, float r, float d, std::shared_ptr<Planet> rp):
  name{n},
  size{s},
  rotationSpeed{r},
  ka{1.0, 1.0, 1.0},
  kg{1.0f},
  turningAxis{0.0f, 1.0f, 0.0f},
  distance{d},
  reference_planet{rp}
  {}

  Planet( std::string n, float s, float r, float d, glm::fvec3 const&c, std::shared_ptr<Planet> rp):
  name{n},
  size{s},
  rotationSpeed{r},
  ka{c},
  kg{1.0f},
  turningAxis{0.0f, 1.0f, 0.0f},
  distance{d},
  reference_planet{rp}
  {}

  //MEMBERS
  std::string name;

  float size;
  float rotationSpeed;

  //Material
  glm::fvec3 ka;
  float kg;

  //Position
  glm::fvec3 turningAxis;

  float distance;
  std::shared_ptr<Planet> reference_planet;
};

glm::fmat4 model_matrix(std::shared_ptr<Planet> const& planet)
{
  glm::fmat4 tmp{};

  if(planet->reference_planet != nullptr) {
    tmp *= model_matrix(planet->reference_planet);
  }

  tmp *= glm::rotate(glm::fmat4{}, float(glfwGetTime() * planet->rotationSpeed), planet->turningAxis);
  tmp *= glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, planet->distance});
  
  //not needed
  //tmp = glm::scale(model_matrix, glm::fvec3{ this->size, this->size, this->size});

  return tmp;
}

#endif