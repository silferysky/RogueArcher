// OpenGL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Quad.h"
#include "SOIL.h"
#include <stdio.h>

GLuint texture[1];

/* int LoadGLTextures()
{
	texture[0] = SOIL_load_OGL_texture
	(
		"test.bmp",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);

	if (texture[0] == 0)
		return false;

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

/* int InitGL(GLvoid)                        // All Setup For OpenGL Goes Here
{
	if (!LoadGLTextures())                   // Jump To Texture Loading Routine ( NEW )
		return 0;                            // If Texture Didn't Load Return FALSE ( NEW )

	glEnable(GL_TEXTURE_2D);                 // Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);                 // Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);    // Black Background
	glClearDepth(1.0f);                      // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);                 // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);                  // The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective Calculations
	return 1;                                // Initialization Went OK
} */

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Terence Dad Gay", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Error check */
	if (glewInit() != GLEW_OK)
		std::cout << "Oh fuck" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	float vertex[8] = { -0.5f,  0.5f,
						 0.5f,  0.5f,
						 0.5f, -0.5f,
						-0.5f, -0.5f,  };

	Quad test(vertex, "(1.0, 0.0, 0.0, 0.0)");

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		test.Draw();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}