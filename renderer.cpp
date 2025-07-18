#include <iostream>
#include "glad.h"
#include "renderer.h"

void GLClearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char *function, const char *file, int line)
{
	bool call_ok = true;
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] 0x0" << std::hex << error << std::dec << " >> " << function 
			<< ", " << file << ": " << line << std::endl;
		call_ok = false;
	}

	return call_ok;
}

