#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Simulation.h"
#include "ResourceManager.h"


const char  * APP_TITLE     = "FluidSim";
const GLuint  SCREEN_WIDTH  = 500;
const GLuint  SCREEN_HEIGHT = 500;
GLFWwindow	* gWindow       = NULL; //pointer to a window

bool		 initOpenGL();
Simulation   simulation(SCREEN_WIDTH, SCREEN_HEIGHT);

int main()
{
	if (!initOpenGL())
	{
		std::cerr << "GLFW initialization failed" << std::endl;
		return -1;
	}

	// Initialize Simulation
	simulation.Initialize();

	// GL Loop
	while (!glfwWindowShouldClose(gWindow))
	{
		// Poll and process events
		glfwPollEvents();

		// Clear the screen (color and depth buffers)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSwapBuffers(gWindow); //This should avoid flickering
	}
	glfwTerminate();//Shut down GLFW

	return 0;
}



void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

}


bool initOpenGL()
{
	//Initialize GLFW
	if (!glfwInit())
	{
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}

	//Setting (minimum) OpenGL version (ver. 3.3 here)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//Force to call modern methods (i.e., cannot call old methods)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	gWindow = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, APP_TITLE, NULL, NULL);

	if (!gWindow)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate(); //Let GLFW clean up properly
		return false;
	}

	glfwMakeContextCurrent(gWindow); //make pWindow the current window
	glfwSetKeyCallback(gWindow, glfw_onKey);

	glewExperimental = GL_TRUE; //Need to initalize to ensure modern openGL is set up properly
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW initialization failed" << std::endl;
		return false;
	}

	return true;
	
}
