#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(GL_VERTEX_SHADER);
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

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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


    
    float positions[6] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.0f,  0.5f
	};

    /*定义缓冲区*/
    unsigned int buffer; 
    glGenBuffers(1, &buffer);
    /*绑定缓冲区*/
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    /*输入数据*/
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, positions, GL_STATIC_DRAW);


#pragma region 告诉OpenGL我们的内存布局
   
    /*启用顶点属性*/
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    
#pragma endregion

#pragma endregion
    
	/*编写第一个着色器*/
    std::string vertexShader =
        "#version 410 core\n"
        "\n"
        "layout(location = 0) in vec4 position;"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";
    std::string fragmentShader =
        "#version 410 core\n"
        "\n"
        "layout(location = 0) out vec4 color\n;"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

	unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader); // 绑定着色器


    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /*发出一个DrawCall*/
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

	glDeleteProgram(shader); // 用完后记得删除着色器程序

    glfwTerminate();
    return 0;
}