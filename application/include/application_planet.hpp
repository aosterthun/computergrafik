struct Planet
{
	Planet( std::string n, float s, float r, float d):
	name{n},
	size{s},
	rotationSpeed{r},
	distance{d},
	reference_planet{nullptr}
	{}

	Planet( std::string n, float s, float r, float d, Planet* rp):
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

	Planet* reference_planet;
};