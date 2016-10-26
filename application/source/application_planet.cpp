#include "application_planet.hpp"

glm::fmat4 model_matrix(std::shared_ptr<Planet> const& planet)
{
  glm::fmat4 tmp{};

  if(planet->reference_planet != nullptr) {
    tmp *= model_matrix(planet->reference_planet);
  }

  glm::fvec3 turning_axis{0.0f, 1.0f, 0.0f};


  tmp *= glm::rotate(glm::fmat4{}, float(glfwGetTime() * planet->rotationSpeed), turning_axis);
  tmp *= glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, planet->distance});
  
  //not needed
  //tmp = glm::scale(model_matrix, glm::fvec3{ this->size, this->size, this->size});

  return tmp;
}