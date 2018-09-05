#include "Texture.h"

#include "stb_image.h" //image loader library

Texture::Texture(const std::string & path) 
	:m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr),
	m_Width(0), m_Height(0), m_BPP(0)
{

	//Load image
	stbi_set_flip_vertically_on_load(1); //Flip image due to expectation of opengl
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	//TODO: check if local buffer is null here:

	GLCall(glGenTextures(1, &m_RendererID)); //generate texture
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	//Need to specify these four parameters:
	//Minification filter:
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	//Magnification filter:
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	//Wrappers:
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	//Give texture:
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, 
		GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0)); //unbind texture

	//Free image data:
	if (m_LocalBuffer) {
		stbi_image_free(m_LocalBuffer);
	}
}

Texture::~Texture() {
	//Free image data:
	/*if (m_LocalBuffer) {
		stbi_image_free(m_LocalBuffer);
	}*/
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const{
	//Query slots or check for embedded etc...

	//Set slot:
	GLCall(glActiveTexture(GL_TEXTURE0 + slot)); //slots are consecutive

	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0)); //unbind texture
}