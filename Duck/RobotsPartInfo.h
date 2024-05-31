#pragma once
#include <vector>

struct RobotsPartInfo {
    int vertexPositions_numberOfFloatsInDataUnit;
    std::vector<float> vertexPositions;

    int vertexId_And_normals_numberOfFloatsInDataUnit;
    std::vector<float> vertexId_And_normals;

    int triangles_numberOfFloatsInDataUnit;
    std::vector<GLuint> triangles;

    int edges_numberOfFloatsInDataUnit;
    std::vector<GLuint> edges;
};