#define STB_IMAGE_IMPLEMENTATION

#include "controls.hpp"
#include "Model.hpp"
#include "window.hpp"

#include "Cube.hpp"
#include "Sphere.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
GLFWwindow *window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <GL/glut.h>

constexpr char title[] = "Computer Graphics. Lab 2";

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;
glm::vec3 position{3.0f, 0.0f, 5.0f};
const glm::vec3 lightPos{3.0f, 3.0f, 3.0f};
const glm::vec3 lightCol{1.0f, 1.0f, 1.0f};

int main(int argc, char **argv)
{
   // Initialise GLFW
   if (!glfwInit())
   {
      fprintf(stderr, "Failed to initialize GLFW\n");
      return -1;
   }

   glfwWindowHint(GLFW_SAMPLES, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   window = glfwCreateWindow(screenW, screenH, "Tutorial 05 - Textured Cube", nullptr, nullptr);
   if (!window)
   {
      fprintf(stderr, "Failed to open GLFW window.\n");
      glfwTerminate();
      return -1;
   }
   glfwMakeContextCurrent(window);

   // Initialize GLEW
   glewExperimental = true; // Needed for core profile
   if (glewInit() != GLEW_OK)
   {
      fprintf(stderr, "Failed to initialize GLEW\n");
      return -1;
   }

   glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

   stbi_set_flip_vertically_on_load(true);

   glfwPollEvents();
   glfwSetCursorPos(window, screenW / 2, screenH / 2);

   glClearColor(0.0f, 0.0f, 0.2f, 0.0f);

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   // Accept fragment if it closer to the camera than the former one
   glDepthFunc(GL_LESS);

   Shader ourShader("light");
   ourShader.use();

   ourShader.setFloat("ambientStrength", 0.000001f);
   ourShader.setVec3("lightPos", lightPos);
   ourShader.setVec3("lightColor", lightCol);
   ourShader.setVec3("spotDir", {-1.0f, 0.0f, 0.0f});
   ourShader.setFloat("cutOff", glm::cos(glm::radians(12.5f)));
   ourShader.setFloat("outerCutOff", glm::cos(glm::radians(17.5f)));

   Model ourModel("low-poly-fox-by-pixelmannen.obj");

   Cube cube;
   Mesh sphere = makeSphere(2.0f, 10, 10);

   double lastTime = glfwGetTime();

   do
   {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      updateMatricesFromInputs();
      constexpr glm::mat4 ModelMatrix = glm::mat4(1.0f);
      ourShader.setMat4("projection", ProjectionMatrix);
      ourShader.setMat4("view", ViewMatrix);
      ourShader.setMat4("model", ModelMatrix);
      ourShader.setMat4("viewPos", ViewMatrix);

      sphere.Draw(ourShader);      
      ourModel.Draw(ourShader);

      const double currentTime = glfwGetTime();
      const float deltaTime = float(currentTime - lastTime);

      const glm::mat4 BoxModelMatrix =
          glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, glm::cos(deltaTime) * 5.0f, glm::sin(deltaTime) * 5.0f));
      ourShader.setMat4("model", BoxModelMatrix);

      cube.Draw(ourShader);

      const glm::mat4 LightModelMatrix =
          glm::scale(glm::translate(glm::mat4(1.0f), lightPos), glm::vec3(0.2f));
      ourShader.setMat4("model", LightModelMatrix);

      cube.Draw(ourShader);

      // Swap buffers
      glfwSwapBuffers(window);
      glfwPollEvents();

   } // Check if the ESC key was pressed or the window was closed
   while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0);

   // Close OpenGL window and terminate GLFW
   glfwTerminate();

   return 0;
}
