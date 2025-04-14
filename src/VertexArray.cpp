#include "VertexArray.h"
#include <iostream>

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID)); // ���ɶ����������
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID)); // ɾ�������������
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID)); // �󶨶����������
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0)); // ��󶥵��������
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind(); // �󶨶����������
	vb.Bind(); // �󶨶��㻺��������
	const auto& elements = layout.GetElements(); // ��ȡԪ��
	unsigned int offest = 0; // ƫ����
	for (unsigned i = 0; i < elements.size(); i++) // ����Ԫ��
	{
		const auto& element = elements[i]; // ��ȡԪ��
		GLCall(glEnableVertexAttribArray(i)); // ���ö�������
		std::cout << "element.count: " << element.count << std::endl;
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offest));
		offest += element.count * VertexBufferElement::GetSizeOfType(element.type); // ����ƫ����
	}
}
