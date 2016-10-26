#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include "application.hpp"
#include "model.hpp"
#include "structs.hpp"

#include "planet.hpp"
#include "star.hpp"

#include <memory>

// gpu representation of model
class ApplicationSolar : public Application {
 public:
  // allocate and initialize objects
  ApplicationSolar(std::string const& resource_path);
  // free allocated objects
  ~ApplicationSolar();

  // update uniform locations and values
  void uploadUniforms();
  // update projection matrix
  void updateProjection();
  // react to key input
  void keyCallback(int key, int scancode, int action, int mods);
    //handle delta mouse movement input
  void mouseCallback(double pos_x, double pos_y);

  // draw all objects
  void render() const;
  void create_scene();

  void upload_planet_transforms(std::shared_ptr<Planet> const& planet) const;
    
 protected:
  void initializeShaderPrograms();
  void initializeGeometry();
  void updateView();

  //planets
  model_object planet_object; // cpu representation of model
  std::vector<std::shared_ptr<Planet>> planets;

  //stars
  model_object stars_object; // cpu representation of model
  std::vector<unsigned> stars_indices;
  std::vector<GLfloat> stars_f;
};

#endif