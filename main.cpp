#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC // using GLEW statically

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define Window GLFWwindow*

const int antialiasing = 4;

int initialiseOpenGL();

const GLfloat upperQuadTriangle[] = {
	-0.5f, 0.5f, 0.0f, // upper left
	0.5f, 0.5f, 0.0f,  // upper right
	0.5f, -0.5f, 0.0f  // bottom right
};

const GLfloat lowerQuadTriangle[] = {
	0.5f, 0.5f, 0.0f,   // upper right
	0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f, // bottom left
};

GLuint vaoID;
static GLuint vbo0, vbo1;
Window window;

int main() {
	if (initialiseOpenGL() == -1) { // if there was a faliure in initialising
		return -1; // return error code -1
	}

	do { // main loop
		glClear(GL_COLOR_BUFFER_BIT); // refresh the screen

		// render code
		glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
		glBindBuffer(GL_ARRAY_BUFFER, vbo0); // bind vbo0
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);

		// post render
		glfwSwapBuffers(window); // swap window buffers
		glfwPollEvents(); // poll the window events
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0); // while esc is not being pressed

	return 0;
}

int initialiseOpenGL() {
	if (!glfwInit()) { // try initialise opengl. returns 0 if it cannot initialise
		fprintf(stderr, "Failed to initialise GLFW!"); // If it cannot initialise print an error message to STDERR
		return NULL; // return 0
	}

	glfwWindowHint(GLFW_SAMPLES, antialiasing); // set the antialiasing amount
	glfwWindowHint(GLFW_VERSION_MAJOR, 3); // use opengl 3

	window = glfwCreateWindow(600, 600, "Yeet", 0, 0); // create a window 600x600 with the title "Yeet"

	if (window == NULL) { // if the window is null
		fprintf(stderr, "Failed to open GLFW window. Maybe you have an old Intel GPU\n");
		return -1;
	}
	
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glGenBuffers(1, &vbo0); // Generate 1 buffer, put the resulting identifier in the variable
	glBindBuffer(GL_ARRAY_BUFFER, vbo0); // bind buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(upperQuadTriangle), upperQuadTriangle, GL_STATIC_DRAW); // give vertices to opengl

	glfwMakeContextCurrent(window); // make the window context the current context, init glfw

	glfwShowWindow(window); // show the window
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE); // turn on the setting allowing us to detect keys

	if (glewInit() != GLEW_OK) { // if glew cannot init
		fprintf(stderr, "Failed to initialise GLEW!"); // If it cannot initialise print an error message to STDERR
		return -1; // return the error code -1
	}

	const char* version = (const char*)glGetString(GL_VERSION);
	const char* glfwVersion = glfwGetVersionString();
	printf("OpenGL Version: %s\n", version);
	printf("GLFW Version: %s\n", glfwVersion);

	return 1;
}