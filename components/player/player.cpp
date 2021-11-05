#include "lab_m1/Tema1/components/player/player.h"

#include <vector>
#include <iostream>

using namespace std;

Player::Player() {
	this->center = glm::vec3(0, 0, 0);
	this->velocity = 300;
	this->shoot = false;
	this->timeLeft = 0;
	this->radius = 42.5;
	this->lives = 5;
	this->fullLife = 5;
}

Player::~Player() {

}

Player::Player(float velocity) {
	Player();
	this->velocity = velocity;
}

void Player::addPart(Mesh* part, std::string type, int partCenterX, int partCenterY, int radius, int length) {
	parts.push_back(PlayerPart(part, type, partCenterX, partCenterY, radius, length));
}

void Player::updateTimer(float time) {
	this->timeLeft -= time;

	if (this->timeLeft <= 0) {
		this->shoot = false;
	}
}

bool Player::canShoot() {
	return this->shoot == false;
}

bool Player::circlesColide(float x1, float y1, float radius1, float x2, float y2, float radius2) {
	float deltaX = x1 - x2, deltaY = y1 - y2;
	float distance = sqrt((deltaX * deltaX) + (deltaY * deltaY));

	if (distance < radius1 + radius2) {
		return true;
	}

	return false;
}

void Player::colideWithEmeny(std::list<Enemy>& enemies) {
	std::list<Enemy>::iterator it = enemies.begin();
	
	while (it != enemies.end()) {
		if (lives <= 0) {
			return;
		}

		if (circlesColide(center.x + translateX, center.y + translateY, radius, it->positionX, it->positionY, it->outlineRadius)) {
			enemies.erase(it++);
			lives--;
		}
		else {
			it++;
		}
	}
}

void Player::colideWithPojectile(std::vector<Projectile>& projectiles) {
	int len = projectiles.size();
	
	for (int i = 0; i < len; ++i) {
		if (lives <= 0) {
			return;
		}

		if (circlesColide(center.x + translateX, center.y + translateY, radius, projectiles[i].center.x, projectiles[i].center.y, projectiles[i].radius) && projectiles[i].tag != "player") {
			projectiles.erase(projectiles.begin() + i);
			i--;
			len--;
			lives--;
		}
	}
}


