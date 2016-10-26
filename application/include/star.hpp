#ifndef APPLICATION_STAR_HPP
#define APPLICATION_STAR_HPP

#include <ctime>
#include <cstdlib>

float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

struct Star
{
  //CONSTRUCTORS

  // ! RANDOM GENERATION !
  Star():
  position{},
  color{}
  {
    float x, y, z, r , g, b;
    x = RandomFloat( -30.0f, 30.0f);
    y = RandomFloat( -30.0f, 30.0f);
    z = RandomFloat( -30.0f, 30.0f);

    r = RandomFloat( 0.0f, 1.0f);
    g = RandomFloat( 0.0f, 1.0f);
    b = RandomFloat( 0.0f, 1.0f);

    position = glm::fvec3{ x, y, z};
    color    = glm::fvec3{ r, g, b};
  }

  Star(glm::fvec3 const& p, glm::fvec3 const& c):
  position{p},
  color{c}
  {}

  //METHODS
  

  //MEMBERS
  glm::fvec3 position;
  glm::fvec3 color;

};



#endif