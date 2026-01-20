#include "FileImage.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

void FileImage::Reload() {
	Free();
	LoadFromFile(filepath);
}

FileImage::FileImage(const std::string filepath) {
	this->filepath = filepath;
	LoadFromFile(filepath);
}

void FileImage::LoadFromFile(const std::string& filepath) {
	stbi_set_flip_vertically_on_load(false);
	data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
}