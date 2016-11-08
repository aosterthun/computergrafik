#ifndef MATERIAL_HPP
#define MATERIAL_HPP

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

struct Material
{
	Material() = default;

	Material(glm::fvec3 const& v):
	a{v},
	d{v},
	s{v},
	g{1.0f}
	{}

	glm::fvec3 a, d, s;
	float g;
};

#endif