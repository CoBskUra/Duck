#pragma once
#include <glm/ext/vector_float3.hpp>
namespace Parametryzations {
	static glm::vec3 circle(float angle) {
		return glm::vec3(cosf(angle), sinf(angle), 0) * 0.5f;
	}
}