#ifndef NOVA_MATERIAL_H
#define NOVA_MATERIAL_H

#include "../Asset.h"
#include <unordered_map>
#include <variant>
#include <glm/glm.hpp>

using Uniform = std::variant<int, float, bool, glm::vec2, glm::vec3, glm::vec4, glm::mat4>;

class Material : public Asset {
public:

	Material(const std::string& vert_filepath, const std::string& frag_filepath);

	void Reload();

	
protected:

	std::string vertex_filepath;
	std::string fragment_filepath;

	std::unordered_map<std::string, Uniform> uniforms;

	void Load();
	bool Compile();
	void ReflectUniforms();

	void OnDestroy() override;
};



#endif
