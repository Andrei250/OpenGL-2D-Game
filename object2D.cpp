#include "object2D.h"

#include <vector>
#include <iostream>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateCircle(
    const std::string& name,
    glm::vec3 center,
    int radius,
    int numberOfVertices,
    glm::vec3 color)
{
    std::vector<VertexFormat> vertices;

    vertices.push_back(VertexFormat(center, glm::vec3(1, 0, 0), color));

    for (int i = 0; i <= numberOfVertices; ++i) {
        float rad = (1.0 * i * 2 * 3.14) / numberOfVertices;

        vertices.push_back(VertexFormat(center + glm::vec3(radius * cos(rad), radius * sin(rad), 0), color));
    }

    Mesh* circle = new Mesh(name);
    std::vector<unsigned int> indices;

    for (int i = 1; i < numberOfVertices; ++i) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    indices.push_back(0);
    indices.push_back(numberOfVertices);
    indices.push_back(numberOfVertices + 1);

   
   circle->SetDrawMode(GL_LINE_LOOP);

    circle->InitFromData(vertices, indices);
    return circle;
}
