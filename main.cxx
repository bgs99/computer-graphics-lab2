#define STB_IMAGE_IMPLEMENTATION

#include "controls.hpp"
#include "Model.hpp"
#include "window.hpp"

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

GLuint image;

static const std::vector<GLfloat> g_vertex_buffer_data = {
    -1.0f, -1.0f, -1.0f, //0 -- left right
    -1.0f, -1.0f, 1.0f,  //1
    -1.0f, 1.0f, 1.0f,   //2

    1.0f, 1.0f, -1.0f,   //3 -- back right
    -1.0f, -1.0f, -1.0f, //4
    -1.0f, 1.0f, -1.0f,  //5

    1.0f, -1.0f, 1.0f,   //6 -- bottom left
    -1.0f, -1.0f, -1.0f, //7
    1.0f, -1.0f, -1.0f,  //8

    1.0f, 1.0f, -1.0f,   //9 -- back left
    1.0f, -1.0f, -1.0f,  //10
    -1.0f, -1.0f, -1.0f, //11

    -1.0f, -1.0f, -1.0f, //12 -- left left
    -1.0f, 1.0f, 1.0f,   //13
    -1.0f, 1.0f, -1.0f,  //14

    1.0f, -1.0f, 1.0f,   //15 -- bottom right
    -1.0f, -1.0f, 1.0f,  //16
    -1.0f, -1.0f, -1.0f, //17

    -1.0f, 1.0f, 1.0f,  //18 -- front left
    -1.0f, -1.0f, 1.0f, //19
    1.0f, -1.0f, 1.0f,  //20

    1.0f, 1.0f, 1.0f,   //21 -- right right
    1.0f, -1.0f, -1.0f, //22
    1.0f, 1.0f, -1.0f,  //23

    1.0f, -1.0f, -1.0f, //24 -- right left
    1.0f, 1.0f, 1.0f,   //25
    1.0f, -1.0f, 1.0f,  //26

    1.0f, 1.0f, 1.0f,   //27 -- top right
    1.0f, 1.0f, -1.0f,  //28
    -1.0f, 1.0f, -1.0f, //29

    1.0f, 1.0f, 1.0f,   //30 -- top left
    -1.0f, 1.0f, -1.0f, //31
    -1.0f, 1.0f, 1.0f,  //32

    1.0f, 1.0f, 1.0f,  //33 -- front right
    -1.0f, 1.0f, 1.0f, //34
    1.0f, -1.0f, 1.0f  //35
};

static const std::vector<GLfloat> g_uv_buffer_data = {
    0.0f, 0.0f,         //0 -- left right
    1.0f / 3, 0.0f,     //1
    1.0f / 3, 1.0f / 2, //2

    2.0f / 3, 1.0f / 2, //3 -- back right
    1.0f, 0.0f,         //4
    1.0f, 1.0f / 2,     //5

    1.0f / 3, 0.0f,     //6 -- bottom left
    2.0f / 3, 1.0f / 2, //7
    1.0f / 3, 1.0f / 2, //8

    2.0f / 3, 1.0f / 2, //9 -- back left
    2.0f / 3, 0.0f,     //10
    1.0f, 0.0f,         //11

    0.0f, 0.0f,         //12 -- left left
    1.0f / 3, 1.0f / 2, //13
    0.0f, 1.0f / 2,     //14

    1.0f / 3, 0.0f,     //15 -- bottom right
    2.0f / 3, 0.0f,     //16
    2.0f / 3, 1.0f / 2, //17

    0.0f, 1.0f,         //18 -- front left
    0.0f, 1.0f / 2,     //19
    1.0f / 3, 1.0f / 2, //20

    2.0f / 3, 1.0f, //21 -- right right
    1.0f, 1.0f / 2, //22
    1.0f, 1.0f,     //23

    1.0f, 1.0f / 2,     //24 -- right left
    2.0f / 3, 1.0f,     //25
    2.0f / 3, 1.0f / 2, //26

    2.0f / 3, 1.0f / 2, //27 -- top right
    2.0f / 3, 1.0f,     //28
    1.0f / 3, 1.0f,     //29

    2.0f / 3, 1.0f / 2, //30 ---top left
    1.0f / 3, 1.0f,     //31
    1.0f / 3, 1.0f / 2, //32

    1.0f / 3, 1.0f,     //33 -- front right
    0.0f, 1.0f,         //34
    1.0f / 3, 1.0f / 2, //35
};

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

   Shader ourShader("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");

   Model ourModel("low-poly-fox-by-pixelmannen.obj");

   GLuint VertexArrayID;
   glGenVertexArrays(1, &VertexArrayID);
   glBindVertexArray(VertexArrayID);

   GLuint Texture = TextureFromFile("dice.bmp");

   GLuint vertexbuffer;
   glGenBuffers(1, &vertexbuffer);
   glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
   glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_data.size() * sizeof(GLfloat), g_vertex_buffer_data.data(), GL_STATIC_DRAW);

   GLuint uvbuffer;
   glGenBuffers(1, &uvbuffer);
   glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
   glBufferData(GL_ARRAY_BUFFER, g_uv_buffer_data.size() * sizeof(GLfloat), g_uv_buffer_data.data(), GL_STATIC_DRAW);

   do
   {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      ourShader.use();

      updateMatricesFromInputs();
      const glm::mat4 ModelMatrix = glm::mat4(1.0f);
      const glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

      ourShader.setMat4("MVP", MVP);

      // Bind our texture in Texture Unit 0
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, Texture);

      ourShader.setInt("myTextureSampler", 0);

      // 1rst attribute buffer : vertices
      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
      glVertexAttribPointer(
          0,        // attribute. No particular reason for 0, but must match the layout in the shader.
          3,        // size
          GL_FLOAT, // type
          GL_FALSE, // normalized?
          0,        // stride
          (void *)0 // array buffer offset
      );

      // 2nd attribute buffer : UVs
      glEnableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
      glVertexAttribPointer(
          1,        // attribute. No particular reason for 1, but must match the layout in the shader.
          2,        // size : U+V => 2
          GL_FLOAT, // type
          GL_FALSE, // normalized?
          0,        // stride
          (void *)0 // array buffer offset
      );

      // Draw the triangle !
      glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles

      glDisableVertexAttribArray(0);
      glDisableVertexAttribArray(1);

      //ourModel.Draw(ourShader);

      // Swap buffers
      glfwSwapBuffers(window);
      glfwPollEvents();

   } // Check if the ESC key was pressed or the window was closed
   while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0);

   // Cleanup VBO and shader
   glDeleteBuffers(1, &vertexbuffer);
   glDeleteBuffers(1, &uvbuffer);
   glDeleteTextures(1, &Texture);
   glDeleteVertexArrays(1, &VertexArrayID);

   // Close OpenGL window and terminate GLFW
   glfwTerminate();

   return 0;
}
