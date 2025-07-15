#include "glad.c"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <alloca.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

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

	unsigned int vertex_array_obj;
	glGenVertexArrays(1, &vertex_array_obj);
	glBindVertexArray(vertex_array_obj);

	float vert_positions[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};

	unsigned int vert_buffer;
	glGenBuffers(1, &vert_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vert_buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), vert_positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	
	ShaderSource shader_source = ParseShader("shaders/basic.shader");

	unsigned int shader = CreateShader(shader_source.vertex_source, shader_source.fragment_source);
	glUseProgram(shader);

	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shader);

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

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDetachShader(program, vs);
	glDetachShader(program, fs);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

static unsigned int CompileShader(unsigned int type, const std::string &source)
{
	unsigned int id = glCreateShader(type);
	const char *src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *message = (char *)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader." << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}
