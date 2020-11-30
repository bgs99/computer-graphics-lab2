#define STB_IMAGE_IMPLEMENTATION

#include "controls.hpp"
#include "Model.hpp"
#include "window.hpp"

#include "Cube.hpp"

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

int main()
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

   // Open a window and create its OpenGL context
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

   // Ensure we can capture the escape key being pressed below
   glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
   // Hide the mouse and enable unlimited mouvement
   //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

   stbi_set_flip_vertically_on_load(true);

   glfwPollEvents();
   glfwSetCursorPos(window, screenW / 2, screenH / 2);

   // Dark blue background
   glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

   // Enable depth test
   glEnable(GL_DEPTH_TEST);
   // Accept fragment if it closer to the camera than the former one
   glDepthFunc(GL_LESS);

   Shader ourShader("shader.vs", "shader.fs");

   Model ourModel("low-poly-fox-by-pixelmannen.obj");

   Cube cube;

   do
   {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      ourShader.use();

      updateMatricesFromInputs();
      constexpr glm::mat4 ModelMatrix = glm::mat4(1.0f);
      ourShader.setMat4("projection", ProjectionMatrix);
      ourShader.setMat4("view", ViewMatrix);
      ourShader.setMat4("model", ModelMatrix);

      cube.Draw(ourShader);
      ourModel.Draw(ourShader);

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
