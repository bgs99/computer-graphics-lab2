#pragma once

#include <glm/glm.hpp>

extern glm::mat4 ViewMatrix,
                 ProjectionMatrix;

void updateMatricesFromInputs();