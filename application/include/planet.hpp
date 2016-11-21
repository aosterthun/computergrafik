#ifndef APPLICATION_PLANET_HPP
#define APPLICATION_PLANET_HPP

#include "material.hpp"

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
  Planet( std::string n,
          float s,
          float r,
          float d,
          Material const& m,  //planet having a material
          int t,
          std::shared_ptr<Planet> rp = nullptr):
  name{n},
  size{s},
  rotationSpeed{r},
  distance{d},
  material{m},
  texture_path{"NONE"},
  turningAxis{0.0f, 1.0f, 0.0f},
  planetType{t},
  reference_planet{rp}
  {}

  Planet( std::string n,
          float s,
          float r,
          float d,
          std::string const& tp,  //planet having a texture
          int t,
          std::shared_ptr<Planet> rp = nullptr):
  name{n},
  size{s},
  rotationSpeed{r},
  distance{d},
  material{{1.0f, 1.0f, 1.0f}},
  texture_path{tp},
  turningAxis{0.0f, 1.0f, 0.0f},
  planetType{t},
  reference_planet{rp}
  {}

  Planet( std::string n,
          float s,
          float r,
          float d,
          int t,
          std::shared_ptr<Planet> rp = nullptr):
  name{n},
  size{s},
  rotationSpeed{r},
  distance{d},
  material{{1.0f, 1.0f, 1.0f}},
  texture_path{"NONE"},
  turningAxis{0.0f, 1.0f, 0.0f},
  planetType{t},
  reference_planet{rp}
  {}

  //MEMBERS
  //Properties
  std::string name;

  float size;
  float rotationSpeed;

  //Position
  glm::fvec3 turningAxis;
  float distance;
  std::shared_ptr<Planet> reference_planet;

  //Material
  Material material;

  //Texture path
  std::string texture_path;

  //Shader
  int planetType;

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