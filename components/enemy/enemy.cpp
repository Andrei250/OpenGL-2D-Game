#include "lab_m1/Tema1/components/enemy/enemy.h"

Enemy::Enemy() {
	this->positionX = 0;
	this->positionY = 0;
	this->sideLength = 0;
	this->velocity = 0;
	this->outlineRadius = 42.5;
	this->canShoot = false;
	this->timer = 0;
}

Enemy::Enemy(float positionX, float positionY, float outlineRadius, float velocity, float sideLength) {
	this->positionX = positionX;
	this->positionY = positionY;
	this->sideLength = sideLength;
	this->velocity = velocity;
	this->outlineRadius = outlineRadius;
	this->canShoot = false;
	this->timer = 0;
}

Enemy::~Enemy() {

}