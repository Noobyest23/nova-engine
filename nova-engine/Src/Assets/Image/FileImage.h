#ifndef NOVA_IMAGE_FILE_H
#define NOVA_IMAGE_FILE_H

#include "image.h"

class FileImage : public Image {
public:

	FileImage(const std::string filepath);

	void Reload();

protected:

	std::string filepath;
	void LoadFromFile(const std::string& filepath);

};


#endif