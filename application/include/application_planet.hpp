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

class Planet
{
public:
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

	glm::fmat4 model_matrix() const
	{
		glm::fmat4 tmp{};

		if(reference_planet != nullptr)
		{
			tmp *= reference_planet->model_matrix();
		}

    	glm::fvec3 turning_axis{0.0f, 1.0f, 0.0f};


    	tmp *= glm::rotate(glm::fmat4{}, float(glfwGetTime() * this->rotationSpeed), turning_axis);
    	tmp *= glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, this->distance});
    	
    	//not needed
    	//tmp = glm::scale(model_matrix, glm::fvec3{ this->size, this->size, this->size});

		return tmp;
	}

	std::string name;

	float size;
	float rotationSpeed;
	float distance;

	std::shared_ptr<Planet> reference_planet;
};

#endif