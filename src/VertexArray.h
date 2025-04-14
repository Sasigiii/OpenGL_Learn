#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
	unsigned int m_RendererID; // 渲染器ID
public:
	VertexArray(); // 构造函数
	~VertexArray(); // 析构函数
	void Bind() const; // 绑定顶点数组对象
	void Unbind() const; // 解绑顶点数组对象
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout); // 添加缓冲区
};