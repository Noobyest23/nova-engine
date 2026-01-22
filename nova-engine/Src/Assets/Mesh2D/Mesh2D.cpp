#include "Mesh2D.h"

void Mesh2D::Upload() {
	index_count = static_cast<uint32_t>(indicies.size());

	glGenVertexArrays(1, &vao);
	glGenVertexArrays(1, &vbo);
	glGenVertexArrays(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vao);
	glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(Vertex2D), verticies.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(uint32_t), indicies.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, tint));

	glBindVertexArray(0);
}

void Mesh2D::Bind() const {
	glBindVertexArray(vao);
}

void Mesh2D::Unbind() const {
	glBindVertexArray(0);
}

void Mesh2D::OnDestroy() {
	if (ebo) glDeleteBuffers(1, &ebo);
	if (vbo) glDeleteBuffers(1, &vbo);
	if (vao) glDeleteBuffers(1, &vao);

	ebo = 0;
	ebo = 0;
	ebo = 0;
}
