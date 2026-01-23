#include "AssetDB.h"
#include "Engine.h"
#include "../NovaScript/Library/nova_asset_db.h"
#include "../Assets/Mesh2D/BoxMesh2D.h"
#include "../Assets/Material/Material.h"
std::unordered_map<std::string, Asset*> AssetDB::assets;

void AssetDB::Init() {
	BoxMesh2D* box_mesh = new BoxMesh2D();
	Register("BoxMesh2D", box_mesh);
	box_mesh->Release();
	
	Material* material = new Material("NovaData/Shaders/sprite2d_vertex.glsl", "NovaData/Shaders/sprite2d_fragment.glsl");
	Register("sprite_2D_shader", material);
	material->Release();
	
}

void AssetDB::Register(std::string id, Asset* asset) {
	auto it = assets.find(id);
	if (it != assets.end()) {
		Engine::GetInstance()->PushError("[AssetDB] Asset '" + id + "' is already registered, or a different asset is trying to register with its name");
		return;
	}
	asset->AddRef();
	assets[id] = asset;
}

Asset* AssetDB::Get(const std::string& id) {
	auto it = assets.find(id);
	if (it == assets.end()) {
		Engine::GetInstance()->PushError("[AssetDB] Asset '" + id + "' is not registered");
		return nullptr;
	}
	
	assets[id]->AddRef();
	return assets[id];
}

void AssetDB::Shutdown() {
	for (std::pair<std::string, Asset*> pair : assets) {
		pair.second->Release();
	}
}