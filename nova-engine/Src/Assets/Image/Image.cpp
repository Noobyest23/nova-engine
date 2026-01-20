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
	if (!is_free) {
		free(data);
		is_free = true;
	}
}

void Image::OnDestroy() {
	Free();
}