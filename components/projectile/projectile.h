#pragma once

#include <string>
#include <vector>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

class Projectile {
 public:
	 glm::vec3 center;
	 float velocityX, velocityY;
	 float defaultVelocity;
	 float distance;
	 float angle;
	 float radius;
	 std::string tag;

	 Projectile();
	 Projectile(glm::vec3 center, float velocityX, float velocityY, float defaultVelocity, float distance, float angle, float radius, std::string type);
	 ~Projectile();
	 bool endLife();
	 void updateCenter(glm::vec3 toAdd);
	 void updateDistance(float distance);
};
