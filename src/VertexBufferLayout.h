#pragma once

#include <GL/glew.h>
#include <vector>

#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int type;	// Ԫ������
	unsigned int count; // ÿ��Ԫ�ص�����
	unsigned char normalized; // �Ƿ��һ��

	static unsigned int GetSizeOfType(unsigned int type) // ��ȡԪ�����ʹ�С
	{
		switch (type)
		{
		case GL_FLOAT:				return sizeof(GLfloat); // ������ 4
		case GL_UNSIGNED_INT:		return sizeof(GLuint); // �޷������� 4
		case GL_UNSIGNED_BYTE:		return sizeof(GLbyte); // �ֽ��� 1
		}
		ASSERT(false); // ���û��ʵ�ָ����͵�GetSizeOfType�����������ʧ��
		return 0;
	}
};

class VertexBufferLayout
{
private:
	unsigned int m_Stride; // ����
	std::vector<VertexBufferElement> m_Elements; // ���㻺����Ԫ��
public:
	VertexBufferLayout()
		: m_Stride(0) // ��ʼ������
	{
	}

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false); // ���û��ʵ�ָ����͵�Push�����������ʧ��
	}
	
	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count; // ���²���
	}
	
	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count; // ���²���
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count; // ���²���
	}

	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }; // ��ȡԪ��
	inline unsigned int GetStride() const { return m_Stride; }; // ��ȡ����
};