#include "VertexArray.h"
#include <iostream>

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID)); // 生成顶点数组对象
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID)); // 删除顶点数组对象
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID)); // 绑定顶点数组对象
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0)); // 解绑顶点数组对象
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind(); // 绑定顶点数组对象
	vb.Bind(); // 绑定顶点缓冲区对象
	const auto& elements = layout.GetElements(); // 获取元素
	unsigned int offest = 0; // 偏移量
	for (unsigned i = 0; i < elements.size(); i++) // 遍历元素
	{
		const auto& element = elements[i]; // 获取元素
		GLCall(glEnableVertexAttribArray(i)); // 启用顶点属性
		std::cout << "element.count: " << element.count << std::endl;
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offest));
		offest += element.count * VertexBufferElement::GetSizeOfType(element.type); // 更新偏移量
	}
}
