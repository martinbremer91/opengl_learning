#include "glad.c"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main()
{
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window;
	window = glfwCreateWindow(640, 480, "Window", NULL, NULL);

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Couldn't load open gl" << std::endl;
		glfwTerminate();
		return -1;
	}
	else
		std::cout << glGetString(GL_VERSION) << std::endl;

	glClearColor(0.25f, 0.5f, 0.75f, 1.0f);

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
	}

	return 0;
}
