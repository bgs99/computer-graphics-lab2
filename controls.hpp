#pragma once

#include <glm/glm.hpp>

extern glm::mat4 ViewMatrix,
    ProjectionMatrix;
extern glm::vec3 position;
void updateMatricesFromInputs();