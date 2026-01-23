#include "Image.h"
#include <stb/stb_image.h>

int Image::GetWidth() const {
	return width;
}

int Image::GetHeight() const {
	return height;
}

int Image::GetChannels() const {
	return channels;
}

unsigned char* Image::GetData() const {
	return data;
}

void Image::Free() {
	if (data and !is_free) {
		free(data);
		data = nullptr;
		is_free = true;
	}
}

void Image::OnDestroy() {
	if (texture) {
		glDeleteTextures(1, &texture);
		texture = 0;
	}

	Free();
}

void Image::Upload() {
	if (!data || texture != 0) return;

	GLenum format = GL_RGBA;
	if (channels == 3) format = GL_RGB;
	if (channels == 1) format = GL_RED;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		format,
		width,
		height,
		0,
		format,
		GL_UNSIGNED_BYTE,
		data
	);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Image::Bind(uint32_t slot) {
	if (texture != 0) {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, texture);
	}
}

GLuint Image::GetTexture() const {
	return texture;
}

bool Image::IsUploaded() {
	return texture != 0;
}