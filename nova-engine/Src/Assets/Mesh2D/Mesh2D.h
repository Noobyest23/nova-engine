#ifndef NOVA_MESH2D_H
#define NOVA_MESH2D_H

#include "../Asset.h"
#include <glm/glm.hpp>
#include <vector>

#include <glad/glad.h>

struct Vertex2D {
	glm::vec2 position;
	glm::vec2 texcoord;
	glm::vec4 tint;
};

class Mesh2D : public Asset {
public:

	const std::vector<Vertex2D>& GetVerticies() const { return verticies; };
	const std::vector<unsigned int>& GetIndicies() const { return indicies; };
	
	void Upload();

	void Bind() const;
	void Unbind() const;

	uint32_t GetIndexCount() const { return index_count; }

protected:

	void Mesh2D::OnDestroy();

	std::vector<Vertex2D> verticies = {};
	std::vector<unsigned int> indicies = {};

	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ebo = 0;

	uint32_t index_count = 0;

};


#endif
