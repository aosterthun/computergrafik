struct Planet
{
	Planet( float s, float r, float d):
	size{s},
	rotationSpeed{r},
	distance{d},
	reference_planet{nullptr}
	{}

	Planet( float s, float r, float d, Planet* rp):
	size{s},
	rotationSpeed{r},
	distance{d},
	reference_planet{rp}
	{}

	float size;
	float rotationSpeed;
	float distance;

	Planet* reference_planet;
};