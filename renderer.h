#pragma once

#define ASSERT(x) if(!(x)) exit(1);
#ifdef DEBUG
#define GLCall(x) GLClearErrors(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x
#endif

void GLClearErrors();
bool GLLogCall(const char *function, const char *file, int line);
