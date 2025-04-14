#pragma once

#include <GL/glew.h>


#define ASSERT(x) if (!(x)) __debugbreak(); // 断言宏
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__)); // 调用OpenGL函数时使用宏来检查错误


// 清除OpenGL错误
void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);