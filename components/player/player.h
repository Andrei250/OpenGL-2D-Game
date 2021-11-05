#pragma once

#include <string>
#include <list>
#include <vector>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"
#include "lab_m1/Tema1/components/enemy/enemy.h"
#include "lab_m1/Tema1/components/projectile/projectile.h"

class PlayerPart {
 public:
	 Mesh* part;
	 std::string type;
	 glm::vec3 center;
	 int radius, length, translateX, translateY;

	 PlayerPart() {
		 type = "";
		 part = NULL;
		 radius = length = 0;
	 }

	 PlayerPart(Mesh* part, std::string type, int partCenterX, int partCenterY, int radius, int length) {
		 this->type = type;
		 this->part = part;
		 this->center = glm::vec3(partCenterX, partCenterY, 0);
		 this->radius = radius;
		 this->length = length;
		 this->translateX = 0;
		 this->translateY = 0;
	 }

	 ~PlayerPart() {

	 }
};

class Player {
 public:
	 glm::vec3 center;
	 std::vector<PlayerPart> parts;
	 float angle = 0;
	 float translateX = 0, translateY = 0;
	 float scaleX = 1, scaleY = 1;
	 float velocity;
	 bool shoot = false;
	 float timeLeft;
	 float radius = 42.5;
	 int lives = 5, fullLife = 5;

	 Player();
	 Player(float velocity);
	 ~Player();

	 void addPart(Mesh* part, std::string type, int partCenterX, int partCenterY, int radius = 0, int length = 0);
	 void updateTimer(float time);
	 bool canShoot();
	 bool circlesColide(float x1, float y1, float radius1, float x2, float y2, float radius2);
	 void colideWithEmeny(std::list<Enemy>& enemies);
	 void colideWithPojectile(std::vector<Projectile>& projectiles);
};
