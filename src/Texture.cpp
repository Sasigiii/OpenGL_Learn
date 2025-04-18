#include "Texture.h"
#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	:m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0), m_RendererID(0)
{
	stbi_set_flip_vertically_on_load(1);
	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

Texture::~Texture()
{
}

void Texture::Bind(unsigned int slot) const
{

}

void Texture::Unbind() const
{
}
