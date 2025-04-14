#pragma once

#include <GL/glew.h>
#include <vector>

#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int type;	// 元素类型
	unsigned int count; // 每个元素的数量
	unsigned char normalized; // 是否归一化

	static unsigned int GetSizeOfType(unsigned int type) // 获取元素类型大小
	{
		switch (type)
		{
		case GL_FLOAT:				return sizeof(GLfloat); // 浮点型 4
		case GL_UNSIGNED_INT:		return sizeof(GLuint); // 无符号整型 4
		case GL_UNSIGNED_BYTE:		return sizeof(GLbyte); // 字节型 1
		}
		ASSERT(false); // 如果没有实现该类型的GetSizeOfType函数，则断言失败
		return 0;
	}
};

class VertexBufferLayout
{
private:
	unsigned int m_Stride; // 步长
	std::vector<VertexBufferElement> m_Elements; // 顶点缓冲区元素
public:
	VertexBufferLayout()
		: m_Stride(0) // 初始化步长
	{
	}

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false); // 如果没有实现该类型的Push函数，则断言失败
	}
	
	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count; // 更新步长
	}
	
	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count; // 更新步长
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count; // 更新步长
	}

	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }; // 获取元素
	inline unsigned int GetStride() const { return m_Stride; }; // 获取步长
};