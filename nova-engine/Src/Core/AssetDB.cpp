#include "AssetDB.h"
#include "Engine.h"
#include "../NovaScript/Library/nova_asset_db.h"
#include "../Assets/Mesh2D/BoxMesh2D.h"

std::unordered_map<std::string, Asset*> AssetDB::assets;

void AssetDB::Init() {
	if (nova_asset_db_init::basic_2D_shapes) {
		BoxMesh2D* box_mesh = new BoxMesh2D();
		Register("BoxMesh2D", box_mesh);
		box_mesh->Release();
		

	}
}

void AssetDB::Register(std::string id, Asset* asset) {
	auto it = assets.find(id);
	if (it != assets.end()) {
		Engine::GetInstance()->PushError("[AssetDB] Asset '" + id + "' is already registered, or a different asset is trying to register with its name");
	}
	asset->AddRef();
	assets[id, asset];
}

Asset* AssetDB::Get(const std::string& id) {
	auto it = assets.find(id);
	if (it == assets.end()) {
		Engine::GetInstance()->PushError("[AssetDB] Asset '" + id + "' is not registered");
	}
	
	assets[id]->AddRef();
	return assets[id];
}

void AssetDB::Shutdown() {
	for (std::pair<std::string, Asset*> pair : assets) {
		pair.second->Release();
	}
}