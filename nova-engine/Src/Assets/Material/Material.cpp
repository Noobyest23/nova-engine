#include "Material.h"
#include <fstream>

Material::Material(const std::string& frag_path, const std::string& vert_path) : vertex_filepath(vert_path), fragment_filepath(frag_path) {
	Load();
	if (!Compile()) {
		return;
	}
	ReflectUniforms();
}

void Material::Load() {
	
}

bool Material::Compile() {
	return true;
}

void Material::ReflectUniforms() {

}

void Material::OnDestroy() {

}