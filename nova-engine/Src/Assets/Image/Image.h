#ifndef NOVA_IMAGE_H
#define NOVA_IMAGE_H

#include "../Asset.h"
#include "glad/glad.h"

class Image : public Asset {
public:

	int GetHeight() const;
	int GetWidth() const;
	int GetChannels() const;
	unsigned char* GetData() const;

	void Free();

	void Upload();
	void Bind(uint32_t slot);
	GLuint GetTexture() const;
	
	bool IsUploaded();

protected:

	int width = 0;
	int height = 0;
	int channels = 0;

	bool is_free = false;

	unsigned char* data = nullptr;

	void OnDestroy() override;
	GLuint texture = 0;
};

#endif