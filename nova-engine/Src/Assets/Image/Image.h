#ifndef NOVA_IMAGE_H
#define NOVA_IMAGE_H

#include "../Asset.h"

class Image : public Asset {
public:

	Image() {};

	int GetHeight() const;
	int GetWidth() const;
	int GetChannels() const;
	unsigned char* GetData() const;

	void Free();

protected:

	int width = 0;
	int height = 0;
	int channels = 0;

	bool is_free = false;

	unsigned char* data = nullptr;

	void OnDestroy() override;
};

#endif