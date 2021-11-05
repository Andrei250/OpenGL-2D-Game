#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;

    spawnTime = 3; // every 3 seconds spawn new enemy
    currentSpawnTime = 0;
    themeResolution = 3000;
    measureError = 2;
    gameOver = false;
    score = 0;

    player = Player(350);

    player.center = glm::vec3(resolution.x / 2, resolution.y / 2, 0);
    createMap();

    Mesh* bodyInner = object2D::CreateCircle("bodyInner", player.center, 25, 500, glm::vec3(1, 0, 0));
    Mesh* bodyOutter = object2D::CreateCircle("bodyOutter", player.center, 35, 500, glm::vec3(0.7f, 1, 0));
    float angle = 3.14 / 6;
    Mesh* leftHand = object2D::CreateCircle("lefthand", player.center + glm::vec3(35 * cos(angle), 35 * sin(angle), 0), 15, 500, glm::vec3(0.8f, 0.4f, 0.1f));
    Mesh* rightHand = object2D::CreateCircle("rightHand", player.center + glm::vec3(35 * cos(angle), - 35 * sin(angle), 0), 15, 500, glm::vec3(0.8f, 0.4f, 0.1f));
    Mesh* projectile = object2D::CreateSquare("projectile", corner, 15, glm::vec3(0, 0, 0), true);

    Mesh* enemyBody = object2D::CreateCircle("enemyBody", glm::vec3(0, 0, 0), 35, 500, glm::vec3(0, 0, 0));
    Mesh* enemyLeftHand = object2D::CreateCircle("enemyLeftHand", glm::vec3(35 * cos(angle), 35 * sin(angle), 0), 15, 500, glm::vec3(0.8f, 0.4f, 0.1f));
    Mesh* enemyRightHand = object2D::CreateCircle("enemyRightHand", glm::vec3(35 * cos(angle), -35 * sin(angle), 0), 15, 500, glm::vec3(0.8f, 0.4f, 0.1f));

    Mesh* healthBarSmaller = object2D::CreateSquare("healthBarSmaller", corner + glm::vec3(0, 0, 3), 25, glm::vec3(1, 0, 0), true);
    Mesh* healthBarBigger = object2D::CreateSquare("healthBarBigger", corner + glm::vec3(0, 0, 2), 35, glm::vec3(0, 0, 1), true);

    player.addPart(leftHand, "leftHand", player.center.x + 35 * cos(angle), player.center.y + 30 * sin(angle), 15);
    player.addPart(rightHand, "rightHand", player.center.x + 35 * cos(angle), player.center.y - 30 * sin(angle), 15);
    player.addPart(bodyInner, "bodyInner", player.center.x, player.center.y, 30);
    player.addPart(bodyOutter, "bodyOutter", player.center.x, player.center.y, 35);
    AddMeshToList(projectile);
    AddMeshToList(enemyBody);
    AddMeshToList(enemyLeftHand);
    AddMeshToList(enemyRightHand);
    AddMeshToList(healthBarSmaller);
    AddMeshToList(healthBarBigger);
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    loadMap();
    GetSceneCamera()->SetPosition(glm::vec3(player.translateX, player.translateY, 50));
    loadHealthBar();

    if (gameOver) {
        return;
    }

    
    killEnemy();
    loadBullets(deltaTimeSeconds);
    renderPlayer();
    spawnEnemy(deltaTimeSeconds);
    loadEnemies(deltaTimeSeconds);

    player.colideWithEmeny(enemies);
    player.colideWithPojectile(projectiles);

    if (!player.canShoot()) {
        player.updateTimer(deltaTimeSeconds);
    }

    if (player.lives <= 0) {
        gameOver = true;
    }
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    if (gameOver) {
        return;
    }

    float translateY = player.translateY;
    float translateX = player.translateX;

    if (window->KeyHold(GLFW_KEY_W)) {
        translateY += player.velocity * deltaTime;
    }
    
    if (window->KeyHold(GLFW_KEY_A)) {
        translateX -= player.velocity * deltaTime;
    }
    
    if (window->KeyHold(GLFW_KEY_S)) {
        translateY -= player.velocity * deltaTime;
    }
    
    if (window->KeyHold(GLFW_KEY_D)) {
        translateX += player.velocity * deltaTime;
    }

    if (!hitWall(translateX + player.center.x, translateY + player.center.y, player.radius)) {
        player.translateX = translateX;
        player.translateY = translateY;
    }

    if (!hitWall(player.translateX + player.center.x, translateY + player.center.y, player.radius)) {
        player.translateY = translateY;
    }

    if (!hitWall(translateX + player.center.x, player.translateY + player.center.y, player.radius)) {
        player.translateX = translateX;
    }
}


void Tema1::OnKeyPress(int key, int mods)
{

}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (gameOver) {
        return;
    }

    mouseY = (int)resolution.y - mouseY;

    float y = mouseY - player.center.y;
    float x = mouseX - player.center.x;

    player.angle = glm::atan2(y, x);
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (gameOver) {
        return;
    }

    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && player.canShoot()) {
        player.shoot = true;
        player.timeLeft = 0.5f;

        mouseY = (int)resolution.y - mouseY;

        float y = mouseY - player.center.y;
        float x = mouseX - player.center.x;

        float angle = glm::atan2(y, x);
        float velocity = 1100;
        float velocityX = velocity * cos(angle);
        float velocityY = velocity * sin(angle);

        Projectile proj = Projectile(player.center + glm::vec3(player.translateX + 45 * cos(angle), player.translateY + 45 * sin(angle), 0), velocityX, velocityY, velocity, 600, angle, 10.7f, "player");

        projectiles.push_back(proj);
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}

void Tema1::renderPlayer() {
    modelMatrix = glm::mat3(1);

    modelMatrix *= transform2D::Translate(player.translateX + player.center.x, player.translateY + player.center.y);
    modelMatrix *= transform2D::Scale(player.scaleX, player.scaleY);
    modelMatrix *= transform2D::Rotate(player.angle);
    modelMatrix *= transform2D::Translate(-player.center.x, -player.center.y);


    for (auto it : player.parts) {
        RenderMesh2D(it.part, shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::createMap() {
    glm::vec3 cornerMap = glm::vec3(0, 0, -1);
    glm::vec3 corner = glm::vec3(0, 0, 1);

    Mesh* object3 = object2D::CreateCircle("object3", player.center, 70, 1000, glm::vec3(1, 1, 0)); // objects
    Mesh* map = object2D::CreateSquare("map", cornerMap, themeResolution, glm::vec3(0.7f, 1, 1), true);
    Mesh* object1 = object2D::CreateSquare("object1", corner, 100, glm::vec3(1, 1, 0), true); // random
    Mesh* object2 = object2D::CreateSquare("object2", corner, 150, glm::vec3(1, 1, 0), true); // used for walls

    AddMeshToList(map);
    AddMeshToList(object1);
    AddMeshToList(object2);
    AddMeshToList(object3);
}

void Tema1::loadBullets(float deltaTime) {
    for (int i = 0; i < projectiles.size(); ++i) {
        auto it = projectiles[i];

        if (it.endLife() || hitWall(it.center.x, it.center.y, it.radius)) {
            projectiles.erase(projectiles.begin() + i);
            i--;
            continue;
        }

        float newDeltaX = it.velocityX * deltaTime;
        float newDeltaY = it.velocityY * deltaTime;

        projectiles[i].updateCenter(glm::vec3(newDeltaX, newDeltaY, 0));
        projectiles[i].updateDistance(sqrt(newDeltaX * newDeltaX + newDeltaY * newDeltaY));

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(it.center.x, it.center.y);
        modelMatrix *= transform2D::Rotate(it.angle);
        RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::spawnEnemy(float deltaTime) {
    currentSpawnTime += deltaTime;

    if (currentSpawnTime < spawnTime) {
        return;
    }

    currentSpawnTime = 0;

    if (enemies.size() > 10) {
        return;
    }

    int positionX = rand() % themeResolution;
    int positionY = rand() % themeResolution;

    while (hitWall(positionX, positionY, 45) ||
        circlesColide(positionX, positionY, 45, player.center.x + player.translateX, player.center.y + player.translateY, player.radius)) {
        positionX = rand() % themeResolution;
        positionY = rand() % themeResolution;
    }

    Enemy enemy = Enemy(positionX, positionY, 45, rand() % 250 + 300, 0);

    enemy.canShoot = rand() % 2;
    enemy.timer = 0.5f;
    enemies.push_back(enemy);
}

void Tema1::loadEnemies(float deltaTime) {
    for (std::list<Enemy>::iterator it = enemies.begin(); it != enemies.end(); ++it) {
        float x = (player.center.x + player.translateX) - it->positionX;
        float y = (player.center.y + player.translateY) - it->positionY;

        float angle = glm::atan2(y, x);

        int newX = it->positionX + it->velocity * deltaTime * cos(angle);
        int newY = it->positionY + it->velocity * deltaTime * sin(angle);

        if (!hitWall(newX, newY, it->outlineRadius)) {
            it->positionX += it->velocity * deltaTime * cos(angle);
            it->positionY += it->velocity * deltaTime * sin(angle);
        } else if (!hitWall(it->positionX, newY, it->outlineRadius)) {
            it->positionY += it->velocity * deltaTime * sin(angle);
        } else if (!hitWall(newX, it->positionY, it->outlineRadius)) {
            it->positionX += it->velocity * deltaTime * cos(angle);
        }

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(it->positionX, it->positionY);
        modelMatrix *= transform2D::Rotate(angle);

        RenderMesh2D(meshes["enemyLeftHand"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["enemyRightHand"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["enemyBody"], shaders["VertexColor"], modelMatrix);

        if (it->canShoot) {
            if (it->timer < 0) {
                it->timer = 0.5f;

                float velocity = 1100;
                float velocityX = velocity * cos(angle);
                float velocityY = velocity * sin(angle);

                Projectile proj = Projectile(glm::vec3(it->positionX + 47 * cos(angle), it->positionY + 47 * sin(angle), 0), velocityX, velocityY, velocity, 700, angle, 10.7f, "enemy");

                projectiles.push_back(proj);
            }
            else {
                it->timer -= deltaTime;
            }
        }

    }
}

bool Tema1::hitWall(float x, float y, float radius) {
    for (auto it : walls) {
        float cornerX = x;
        float cornerY = y;

        if (it.startX > x) {
            cornerX = it.startX;
        }
        else if (it.stopX < x) {
            cornerX = it.stopX;
        }

        if (it.startY > y) {
            cornerY = it.startY;
        }
        else if (it.stopY < y) {
            cornerY = it.stopY;
        }

        float deltaX = x - cornerX, deltaY = y - cornerY;
        float distance = sqrt((deltaX * deltaX) + (deltaY * deltaY));

        if (distance <= radius - measureError) {
            return true;
        }
    }

    return false;
}

bool Tema1::circlesColide(float x1, float y1, float radius1, float x2, float y2, float radius2) {
    float deltaX = x1 - x2, deltaY = y1 - y2;
    float distance = sqrt((deltaX * deltaX) + (deltaY * deltaY));

    if (distance < radius1 + radius2) {
        return true;
    }

    return false;
}

void Tema1::killEnemy() {
    int len = projectiles.size();
    
    for (int i = 0; i < len; ++i) {
        std::list<Enemy>::iterator it = enemies.begin();

        while (it != enemies.end()) {
            if (circlesColide(projectiles[i].center.x, projectiles[i].center.y, projectiles[i].radius, it->positionX, it->positionY, it->outlineRadius) && projectiles[i].tag != "enemy") {
                enemies.erase(it);
                projectiles.erase(projectiles.begin() + i);
                i--;
                len--;
                score++;
                break;
            }
            else {
                it++;
            }
        }
    }
}

void Tema1::loadMap() {
    modelMatrix = glm::mat3(1);

    RenderMesh2D(meshes["map"], shaders["VertexColor"], modelMatrix);

    {// random objects
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(1300, 700);
        RenderMesh2D(meshes["object2"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(1400, 1300);
        RenderMesh2D(meshes["object2"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(1500, 1500);
        RenderMesh2D(meshes["object1"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(2000, 1000);
        RenderMesh2D(meshes["object1"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(1500, 2500);
        RenderMesh2D(meshes["object2"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(900, 200);
        RenderMesh2D(meshes["object1"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(2500, 300);
        RenderMesh2D(meshes["object2"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(2200, 600);
        RenderMesh2D(meshes["object2"], shaders["VertexColor"], modelMatrix);


        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(2500, 1700);
        RenderMesh2D(meshes["object2"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(2200, 2300);
        RenderMesh2D(meshes["object2"], shaders["VertexColor"], modelMatrix);

    }

    { // tetris form
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(450, 500);
        RenderMesh2D(meshes["object2"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(600, 500);
        RenderMesh2D(meshes["object2"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(750, 500);
        RenderMesh2D(meshes["object2"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(900, 500);
        RenderMesh2D(meshes["object2"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(1050, 500);
        RenderMesh2D(meshes["object2"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(750, 650);
        RenderMesh2D(meshes["object2"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(750, 800);
        RenderMesh2D(meshes["object2"], shaders["VertexColor"], modelMatrix);
    }

    { // tetris form
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(300, 1000);
        RenderMesh2D(meshes["object2"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(300, 1150);
        RenderMesh2D(meshes["object2"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(450, 1150);
        RenderMesh2D(meshes["object2"], shaders["VertexColor"], modelMatrix);
    }

    { // tetris form
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(1200, 1000);
        RenderMesh2D(meshes["object2"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(1200, 1150);
        RenderMesh2D(meshes["object2"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(1050, 1150);
        RenderMesh2D(meshes["object2"], shaders["VertexColor"], modelMatrix);
    }
    
    walls.push_back(rectangle(1299, 699, 1299 + 152, 699 + 152));
    walls.push_back(rectangle(1399, 1299, 1399 + 152, 1299 + 152));
    walls.push_back(rectangle(1499, 1499, 1499 + 102, 1499 + 102));
    walls.push_back(rectangle(1999, 999, 1999 + 102, 999 + 102));
    walls.push_back(rectangle(1499, 2499, 1499 + 152, 2499 + 152));
    walls.push_back(rectangle(899, 199, 899 + 102, 199 + 102));
    walls.push_back(rectangle(2499, 299, 2499 + 152, 299 + 152));
    walls.push_back(rectangle(2199, 599, 2199 + 152, 599 + 152));
    walls.push_back(rectangle(2499, 1699, 2499 + 152, 1699 + 152));
    walls.push_back(rectangle(2199, 2299, 2199 + 152, 2299 + 152));
    walls.push_back(rectangle(449, 1149, 449 + 152, 1149 + 152));
    walls.push_back(rectangle(299, 999, 299 + 152, 999 + 2 * 150 + 2));
    walls.push_back(rectangle(1049, 1149, 1049 + 152, 1149 + 152));
    walls.push_back(rectangle(1199, 999, 1199 + 152, 999 + 2 * 150 + 2));
    walls.push_back(rectangle(749, 649, 749 + 152, 649 + 2 * 150 + 2));
    walls.push_back(rectangle(449, 499, 449 + 5 * 150 + 2, 499 + 152));
    walls.push_back(rectangle(-10, -10, themeResolution + 10, 1));
    walls.push_back(rectangle(-10, themeResolution, themeResolution + 10, themeResolution + 10));
    walls.push_back(rectangle(-10, -10, 0, themeResolution + 10));
    walls.push_back(rectangle(themeResolution, -10, themeResolution + 10, themeResolution + 10));
}

void Tema1::loadHealthBar() {
    float offsetX = (player.fullLife * 35 - player.fullLife * 25) / 2;
    float offsetY = 5;
    float offsetWindowX = resolution.x - 200;
    float offsetWindowY = 20;

    for (int i = player.fullLife - player.lives; i < player.fullLife; ++i) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(offsetWindowX + offsetX + player.translateX + i * 25, offsetWindowY + offsetY + player.translateY);
        RenderMesh2D(meshes["healthBarSmaller"], shaders["VertexColor"], modelMatrix);
    }

    for (int i = 0; i < player.fullLife; ++i) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(offsetWindowX + player.translateX + i * 35, offsetWindowY + player.translateY);
        RenderMesh2D(meshes["healthBarBigger"], shaders["VertexColor"], modelMatrix);
    }

    
}

