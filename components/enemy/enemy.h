#pragma once

#include <string>
#include <vector>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

class Enemy {
 public:
	 float positionX, positionY, outlineRadius;
	 float velocity;
	 float sideLength;
	 bool canShoot = false;
	 float timer = 0;

	 Enemy();
	 Enemy(float positionX, float positionY, float outlineRadius, float velocity, float sideLength);
	 ~Enemy();
};
