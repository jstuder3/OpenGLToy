#pragma once

#include <glm/vec3.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	glm::vec3 get_position();
	glm::vec3 get_target();
	glm::vec3 get_up();
};
