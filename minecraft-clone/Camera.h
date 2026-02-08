#include <glm/geometric.hpp>

struct Camera {
	glm::vec3 position = { 0.0f,0.0f,0.0f };
	glm::vec2 rotation = { 0.0f,0.0f };
	glm::vec3 forward;
	glm::vec3 right;

	void setCameraForward()
	{
		forward.x = cos(glm::radians(rotation.x - 90)) * cos(glm::radians(rotation.y));
		forward.y = sin(glm::radians(rotation.y));
		forward.z = sin(glm::radians(rotation.x - 90)) * cos(glm::radians(rotation.y));
	}

	void setCameraRight()
	{
		right = normalize(cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
	}
};