#ifndef NOVA_ASSET_H
#define NOVA_ASSET_H

#include <string>
#include <memory>
#include <atomic>

class Asset {
public:

	Asset() {};
	virtual ~Asset() = default;

	void AddRef();
	void Release();

	int GetReferenceCount() const;

protected:

	virtual void OnDestroy() {};

private:

	std::atomic<int> ref_count{ 1 };
};


#endif