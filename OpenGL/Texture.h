#pragma once
#include "Renderer.h"


class Texture {
private:
	unsigned int m_RendererID;
	std::string m_FilePath; //Keep track of source file
	unsigned char* m_LocalBuffer; //local storage of texture
	int m_Width, m_Height, m_BPP; //meta data
public:
	Texture(const std::string & path);
	~Texture();

	//texture slot defaults to 0 (# of slots dependent on GPU)
	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	//Helpers:
	inline int GetWidth() const {
		return m_Width;
	}
	inline int GetHeight() const {
		return m_Height;
	}



};