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
  Planet( std::string n, float s, float r, float d):
  name{n},
  size{s},
  rotationSpeed{r},
  distance{d},
  reference_planet{nullptr}
  {}

  Planet( std::string n, float s, float r, float d, std::shared_ptr<Planet> rp):
  name{n},
  size{s},
  rotationSpeed{r},
  distance{d},
  reference_planet{rp}
  {}

  std::string name;

  float size;
  float rotationSpeed;
  float distance;

  std::shared_ptr<Planet> reference_planet;
};

#endif