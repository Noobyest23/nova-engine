#ifndef NOVA_MATERIAL_H
#define NOVA_MATERIAL_H

#include "../Asset.h"
#include <unordered_map>
#include <variant>
#include <glm/glm.hpp>
#include <glad/glad.h>

using Uniform = std::variant<int, float, bool, glm::vec2, glm::vec3, glm::vec4, glm::mat4>;

class Material : public Asset {
public:

	Material(const std::string& vert_filepath, const std::string& frag_filepath);

	void Reload();
	void Bind() const;
	void Unbind() const;
	
	void SetUniform(const std::string& name, const Uniform&);
	bool HasUniform(const std::string& name) const;

protected:

	std::string vertex_filepath;
	std::string fragment_filepath;

	std::unordered_map<std::string, Uniform> uniforms;

	GLuint program = 0;

	void Load();
	bool Compile();
	void ReflectUniforms();

	void OnDestroy() override;
};



#endif
