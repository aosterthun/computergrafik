#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include "application.hpp"
#include "model.hpp"
#include "structs.hpp"

#include "planet.hpp"

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


  void upload_planet_transforms(std::shared_ptr<Planet> const& planet) const;
    
 protected:
  void create_scene();
  void create_sq();

  void initializeShaderPrograms();
  void initializeGeometry();
  void initializeTextures();
  void initializeFrameBuffer(unsigned const& width, unsigned const& height);
  void updateView();

  //planets
  model_object planet_object; // cpu representation of model
  std::vector<std::shared_ptr<Planet>> planets;
  std::map<std::string, texture_object> texture_map;

  //stars
  model_object          stars_object; // cpu representation of model
  std::vector<unsigned> stars_indices;
  std::vector<GLfloat>  stars_f;

  //screen quad
  model_object    screen_quad;
  texture_object  sq_texture;
  GLuint          sq_handle;

  //effect mapping
  std::map<std::string, int> effect_map;
};

#endif