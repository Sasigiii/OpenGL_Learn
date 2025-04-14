#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID; // 渲染器ID
public:
	VertexBuffer(const void* data, unsigned int size); // 构造函数
	~VertexBuffer(); // 析构函数
	void Bind() const; // 绑定缓冲区
	void Unbind() const; // 解绑缓冲区
};