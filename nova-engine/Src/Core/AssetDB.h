#ifndef NOVA_ENGINE_ASSET_DB
#define NOVA_ENGINE_ASSET_DB

#include <unordered_map>
#include <string>
#include "../Assets/Asset.h"

class AssetDB {
public:

	static void Init();

	static void Register(std::string, Asset* asset);
	static Asset* Get(const std::string& id);

	static void Shutdown();

private:

	static std::unordered_map<std::string, Asset*> assets;

};

#endif