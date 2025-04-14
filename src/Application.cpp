#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "Indexbuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"


struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filePath)
{
	std::ifstream stream(filePath); // 打开文件 输入文件流
	if (!stream.is_open()) // 如果文件打开失败
	{
		std::cerr << "Failed to open file: " << filePath << std::endl; // 输出错误信息
		return { "", "" }; // 返回空字符串
	}

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
	std::string line;
	std::stringstream ss[2]; // 创建字符串流数组 用于存储顶点着色器和片段着色器
    ShaderType type = ShaderType::NONE;
	// 逐行读取文件
    while (getline(stream,line))
    {
        if (line.find("#shader") != std::string::npos)
        {
			if (line.find("vertex") != std::string::npos)
			{
				// 添加到顶点着色器
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				// 添加到片段着色器
				type = ShaderType::FRAGMENT;
			}
        }
        else
        {
			ss[(int)type] << line << "\n"; // 将行添加到对应的着色器
        }
    }

	return { ss[0].str(), ss[1].str() }; // 返回顶点着色器和片段着色器
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
	const char* src = source.c_str(); // 返回string开头字符的指针
	glShaderSource(id, 1, &src, nullptr); // 将着色器源代码传递给OpenGL  id，源代码数量，源代码，长度
	glCompileShader(id); // 编译着色器
	/*检查编译错误*/
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result); // 获取编译状态
    if (result == GL_FALSE) // 如果编译失败
    {
		// 获取错误信息
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); // 获取错误信息长度
		char* message = (char*)alloca(length * sizeof(char)); // 分配栈内存（malloc是分配堆内存）
		glGetShaderInfoLog(id, length, &length, message); // 获取错误信息
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex " : "frangment ") << "shader!" << std::endl;
		std::cout << message << std::endl; // 输出错误信息
		glDeleteShader(id); // 删除着色器
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& frangmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);// 创建顶点着色器
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, frangmentShader);// 创建片段着色器
    /*链接着色器*/
	glAttachShader(program, vs); // 将顶点着色器附加到程序
	glAttachShader(program, fs); // 将片段着色器附加到程序
	glLinkProgram(program); // 链接程序
	glValidateProgram(program); // 验证程序
    /*我们可以删除中间产物，因为着色器已经被编译到program里了*/
	glDeleteShader(vs); // 删除顶点着色器
	glDeleteShader(fs); // 删除片段着色器
	
    /*也可以调用glDetachShader来删除着色器源代码，但是会丧失调试能力，而且它们占用的内存微不足道，删除他们得不偿失*/

    return program; // 返回程序ID
}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /*使用核心配置文件*/
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	glfwSwapInterval(1); // 设置帧率与显示器刷新率一致

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */ /*获取OpenGL上下文*/
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error initializing GLEW" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;


#pragma region 向OpenGL提供数据所需的所有代码
	{
		float positions[] = {
			-0.5f, -0.5f, // 0
			 0.5f, -0.5f, // 1
			 0.5f,  0.5f, // 2
			 -0.5f, 0.5f, // 3
		};

		/*定义一个索引缓冲区*/
		unsigned int indices[] = {
			0, 1, 2, // 第一个三角形
			2, 3, 0  // 第二个三角形
		};


		VertexArray va; // 创建顶点数组对象
		VertexBuffer vb(positions, sizeof(float) * 4 * 2); // 创建顶点缓冲区对象

		VertexBufferLayout layout; // 创建顶点缓冲区布局
		layout.Push<float>(2); // 添加一个float类型的元素，表示每个顶点有两个属性
		va.AddBuffer(vb, layout); // 添加缓冲区到顶点数组对象)

#pragma region 告诉OpenGL我们的内存布局

		/*启用顶点属性*/
		// GLCall(glEnableVertexAttribArray(0));
		// 0表示位置属性，2表示每个顶点有两个属性，GL_FLOAT表示属性类型，GL_FALSE表示不标准化，sizeof(float) * 2表示步长，0表示偏移量
		// GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
		/*创建索引缓冲区*/
		IndexBuffer ib(indices, 6); // 创建索引缓冲区对象


#pragma endregion

#pragma endregion
		ShaderProgramSource source = ParseShader("res/shaders/Basic.shader"); // 解析着色器

		unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
		glUseProgram(shader); // 绑定着色器

		GLCall(int localtion = glGetUniformLocation(shader, "u_Color")); // 获取uniform变量位置
		ASSERT(localtion != -1); // 检查uniform变量位置是否有效
		glUniform4f(localtion, 1.0f, 0.3f, 0.8f, 1.0f); // 设置uniform变量

		/*解绑所有缓冲区*/
		va.Unbind(); // 解绑顶点数组对象
		GLCall(glUseProgram(0)); // 解绑Program
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0);) // 解绑VBO
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); // 解绑IBO

		float r = 0.0f;
		float increment = 0.05f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);


			/*颜色动态变化*/
			GLCall(glUseProgram(shader);)
				glUniform4f(localtion, r, 0.3f, 0.8f, 1.0f);

			va.Bind(); // 绑定顶点数组对象
			ib.Bind(); // 绑定索引缓冲区

			/*发出一个DrawCall*/
			// glDrawArrays(GL_TRIANGLES, 0, 6);

			// GLClearError(); // 清除OpenGL错误
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // 绘制索引缓冲区中的元素 数量，类型都是索引缓冲区
			// ASSERT(GLLogCall()); // 检查OpenGL错误

			if (r > 1.0f)
			{
				increment = -0.05f;
			}
			else if (r < 0.0f)
			{
				increment = 0.05f;
			}
			r += increment; // 更新颜色值

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		glDeleteProgram(shader); // 用完后记得删除着色器程序
	} // 创建作用域 创建的对象会在作用域结束时自动销毁
    glfwTerminate();
    return 0;
}