#include "glad.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <alloca.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "renderer.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "index_buffer.h"

struct ShaderSource
{
	std::string vertex_source;
	std::string fragment_source;
};

static ShaderSource ParseShader(const std::string &filepath);
static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);
static unsigned int CompileShader(unsigned int type, const std::string &source);

int main()
{
#ifdef DEBUG
	std::cout << "DEBUG MODE" << std::endl;
#endif

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window;
	window = glfwCreateWindow(640, 480, "Window", NULL, NULL);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Couldn't load open gl" << std::endl;
		glfwTerminate();
		return -1;
	}
	else
		std::cout << glGetString(GL_VERSION) << std::endl;

	GLCall(glClearColor(0.25f, 0.5f, 0.75f, 1.0f));

	float vert_positions[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	unsigned int vertex_array_obj;
	GLCall(glGenVertexArrays(1, &vertex_array_obj));
	GLCall(glBindVertexArray(vertex_array_obj));

	VertexArray vertex_array;
	VertexBuffer vertex_buffer(vert_positions, 4 * 2 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2);
	vertex_array.AddBuffer(vertex_buffer, layout);

	IndexBuffer index_buffer(indices, 6);

	ShaderSource shader_source = ParseShader("shaders/basic.shader");

	unsigned int shader = CreateShader(shader_source.vertex_source, shader_source.fragment_source);
	GLCall(glUseProgram(shader));

	GLCall(int location = glGetUniformLocation(shader, "u_color"));
	GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));

	GLCall(glBindVertexArray(0));
	GLCall(glUseProgram(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	float r = 0.0f;
	float increment = 0.05f;

	while(!glfwWindowShouldClose(window))
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		GLCall(glUseProgram(shader));
		GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

		vertex_array.Bind();
		index_buffer.Bind();

		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		if (r > 1.0f)
			increment = -0.05f;
		else if (r < 0.0f)
			increment = 0.05f;

		r += increment;

		glfwSwapBuffers(window);
		GLCall(glfwPollEvents());
	}

	GLCall(glDeleteProgram(shader));
	glfwTerminate();

	return 0;
}

static ShaderSource ParseShader(const std::string &filepath)
{
	std::ifstream stream(filepath);

	enum ShaderType{ NONE = -1, VERTEX = 0, FRAGMENT = 1 };
	ShaderType shader_type = NONE;
	
	std::string line;
	std::stringstream string_streams[2];

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				shader_type = VERTEX;
			else if (line.find("fragment") != std::string::npos)
				shader_type = FRAGMENT;
		}
		else
		{
			string_streams[(int)shader_type] << line << std::endl;
		}
	}

	return { string_streams[0].str(), string_streams[1].str() };
}

static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDetachShader(program, vs));
	GLCall(glDetachShader(program, fs));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

static unsigned int CompileShader(unsigned int type, const std::string &source)
{
	unsigned int id = glCreateShader(type);
	const char *src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char *message = (char *)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader." << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}
