#include "lab_m1/Tema1/components/projectile/projectile.h"

Projectile::Projectile() {

}

Projectile::Projectile(glm::vec3 center, float velocityX, float velocityY, float defaultVelocity, float distance, float angle, float radius, std::string type) {
	this->center = center;
	this->velocityX = velocityX;
	this->velocityY = velocityY;
	this->defaultVelocity = defaultVelocity;
	this->distance = distance;
	this->angle = angle;
	this->radius = radius;
	this->tag = type;
}

Projectile::~Projectile() {

}

bool Projectile::endLife() {
	return distance <= 0;
}

void Projectile::updateCenter(glm::vec3 toAdd) {
	this->center += toAdd;
}

void Projectile::updateDistance(float distance) {
	this->distance -= distance;
}
