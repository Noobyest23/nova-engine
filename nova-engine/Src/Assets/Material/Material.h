#ifndef NOVA_MATERIAL_H
#define NOVA_MATERIAL_H

#include "../Asset.h"
#include "../Image/Image.h"
#include <unordered_map>
#include <variant>
#include <glm/glm.hpp>
#include <glad/glad.h>

using Uniform = std::variant<int, float, bool, glm::vec2, glm::vec3, glm::vec4, glm::mat4>;

class Material : public Asset {
public:

	Material(const std::string& vert_filepath, const std::string& frag_filepath);

	void Reload();
	void Bind();
	void Unbind() const;
	
	void SetUniform(const std::string& name, const Uniform&);
	void SetImageUniform(const std::string& name, Image*);
	Uniform GetUniform(const std::string& name) { return uniforms[name]; }

protected:

	std::string vertex_filepath;
	std::string fragment_filepath;

	std::unordered_map<std::string, Uniform> uniforms;
	std::unordered_map<std::string, GLint> uniform_locations;
	std::unordered_map<std::string, Image*> uniform_images;
	GLuint program = 0;

	void Load();
	bool Compile();
	void ReflectUniforms();

	void OnDestroy() override;
};



#endif
