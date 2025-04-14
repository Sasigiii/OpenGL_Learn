#include "Indexbuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: m_Count(count) // 初始化索引数量
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint)); // 检查 unsigned int大小是否与 GLuint 相同

	GLCall(glGenBuffers(1, &m_RendererID)); // 生成索引缓冲区
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); // 绑定索引缓冲区
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW)); // 输入数据
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID)); // 删除索引缓冲区
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); // 绑定索引缓冲区
}

void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); // 解绑索引缓冲区
}
