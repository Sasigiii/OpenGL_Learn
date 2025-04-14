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
	std::ifstream stream(filePath); // ���ļ� �����ļ���
	if (!stream.is_open()) // ����ļ���ʧ��
	{
		std::cerr << "Failed to open file: " << filePath << std::endl; // ���������Ϣ
		return { "", "" }; // ���ؿ��ַ���
	}

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
	std::string line;
	std::stringstream ss[2]; // �����ַ��������� ���ڴ洢������ɫ����Ƭ����ɫ��
    ShaderType type = ShaderType::NONE;
	// ���ж�ȡ�ļ�
    while (getline(stream,line))
    {
        if (line.find("#shader") != std::string::npos)
        {
			if (line.find("vertex") != std::string::npos)
			{
				// ��ӵ�������ɫ��
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				// ��ӵ�Ƭ����ɫ��
				type = ShaderType::FRAGMENT;
			}
        }
        else
        {
			ss[(int)type] << line << "\n"; // ������ӵ���Ӧ����ɫ��
        }
    }

	return { ss[0].str(), ss[1].str() }; // ���ض�����ɫ����Ƭ����ɫ��
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
	const char* src = source.c_str(); // ����string��ͷ�ַ���ָ��
	glShaderSource(id, 1, &src, nullptr); // ����ɫ��Դ���봫�ݸ�OpenGL  id��Դ����������Դ���룬����
	glCompileShader(id); // ������ɫ��
	/*���������*/
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result); // ��ȡ����״̬
    if (result == GL_FALSE) // �������ʧ��
    {
		// ��ȡ������Ϣ
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); // ��ȡ������Ϣ����
		char* message = (char*)alloca(length * sizeof(char)); // ����ջ�ڴ棨malloc�Ƿ�����ڴ棩
		glGetShaderInfoLog(id, length, &length, message); // ��ȡ������Ϣ
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex " : "frangment ") << "shader!" << std::endl;
		std::cout << message << std::endl; // ���������Ϣ
		glDeleteShader(id); // ɾ����ɫ��
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& frangmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);// ����������ɫ��
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, frangmentShader);// ����Ƭ����ɫ��
    /*������ɫ��*/
	glAttachShader(program, vs); // ��������ɫ�����ӵ�����
	glAttachShader(program, fs); // ��Ƭ����ɫ�����ӵ�����
	glLinkProgram(program); // ���ӳ���
	glValidateProgram(program); // ��֤����
    /*���ǿ���ɾ���м�����Ϊ��ɫ���Ѿ������뵽program����*/
	glDeleteShader(vs); // ɾ��������ɫ��
	glDeleteShader(fs); // ɾ��Ƭ����ɫ��
	
    /*Ҳ���Ե���glDetachShader��ɾ����ɫ��Դ���룬���ǻ�ɥʧ������������������ռ�õ��ڴ�΢�������ɾ�����ǵò���ʧ*/

    return program; // ���س���ID
}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /*ʹ�ú��������ļ�*/
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	glfwSwapInterval(1); // ����֡������ʾ��ˢ����һ��

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */ /*��ȡOpenGL������*/
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error initializing GLEW" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;


#pragma region ��OpenGL�ṩ������������д���
	{
		float positions[] = {
			-0.5f, -0.5f, // 0
			 0.5f, -0.5f, // 1
			 0.5f,  0.5f, // 2
			 -0.5f, 0.5f, // 3
		};

		/*����һ������������*/
		unsigned int indices[] = {
			0, 1, 2, // ��һ��������
			2, 3, 0  // �ڶ���������
		};


		VertexArray va; // ���������������
		VertexBuffer vb(positions, sizeof(float) * 4 * 2); // �������㻺��������

		VertexBufferLayout layout; // �������㻺��������
		layout.Push<float>(2); // ���һ��float���͵�Ԫ�أ���ʾÿ����������������
		va.AddBuffer(vb, layout); // ��ӻ������������������)

#pragma region ����OpenGL���ǵ��ڴ沼��

		/*���ö�������*/
		// GLCall(glEnableVertexAttribArray(0));
		// 0��ʾλ�����ԣ�2��ʾÿ���������������ԣ�GL_FLOAT��ʾ�������ͣ�GL_FALSE��ʾ����׼����sizeof(float) * 2��ʾ������0��ʾƫ����
		// GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
		/*��������������*/
		IndexBuffer ib(indices, 6); // ������������������


#pragma endregion

#pragma endregion
		ShaderProgramSource source = ParseShader("res/shaders/Basic.shader"); // ������ɫ��

		unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
		glUseProgram(shader); // ����ɫ��

		GLCall(int localtion = glGetUniformLocation(shader, "u_Color")); // ��ȡuniform����λ��
		ASSERT(localtion != -1); // ���uniform����λ���Ƿ���Ч
		glUniform4f(localtion, 1.0f, 0.3f, 0.8f, 1.0f); // ����uniform����

		/*������л�����*/
		va.Unbind(); // ��󶥵��������
		GLCall(glUseProgram(0)); // ���Program
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0);) // ���VBO
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); // ���IBO

		float r = 0.0f;
		float increment = 0.05f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);


			/*��ɫ��̬�仯*/
			GLCall(glUseProgram(shader);)
				glUniform4f(localtion, r, 0.3f, 0.8f, 1.0f);

			va.Bind(); // �󶨶����������
			ib.Bind(); // ������������

			/*����һ��DrawCall*/
			// glDrawArrays(GL_TRIANGLES, 0, 6);

			// GLClearError(); // ���OpenGL����
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // ���������������е�Ԫ�� ���������Ͷ�������������
			// ASSERT(GLLogCall()); // ���OpenGL����

			if (r > 1.0f)
			{
				increment = -0.05f;
			}
			else if (r < 0.0f)
			{
				increment = 0.05f;
			}
			r += increment; // ������ɫֵ

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		glDeleteProgram(shader); // �����ǵ�ɾ����ɫ������
	} // ���������� �����Ķ���������������ʱ�Զ�����
    glfwTerminate();
    return 0;
}