#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema1/components/player/player.h"
#include "lab_m1/Tema1/components/projectile/projectile.h"
#include "lab_m1/Tema1/components/enemy/enemy.h"
#include <list>

namespace m1
{
    struct rectangle {
        int startX, startY, stopX, stopY;

        rectangle(int startX, int startY, int stopX, int stopY) {
            this->startX = startX;
            this->startY = startY;
            this->stopX = stopX;
            this->stopY = stopY;
        }
    };

    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void renderPlayer();
        void createMap();
        void loadMap();
        void loadBullets(float deltaTime);
        void spawnEnemy(float deltaTime);
        void loadEnemies(float deltaTime);
        bool hitWall(float x, float y, float radius);
        bool circlesColide(float x1, float y1, float radius1, float x2, float y2, float radius2);
        void killEnemy();
        void loadHealthBar();

     protected:
        glm::mat3 modelMatrix;
        glm::vec2 resolution;
        Player player;
        std::vector<Projectile> projectiles;
        std::list<Enemy> enemies;
        float spawnTime, currentSpawnTime;
        int themeResolution;
        std::vector<rectangle> walls;
        float measureError;
        bool gameOver;
        int score;

    };
}   // namespace m1
