#ifndef NOVA_MESH_H
#define NOVA_MESH_H

#include "../Asset.h"
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
	glm::vec3 position;
	glm::vec2 texcoord;
	glm::vec3 normal;
	glm::vec3 tangent;
};

class Mesh : public Asset {
public:

	

protected:

	std::vector<Vertex> verticies = {};
	std::vector<unsigned int> indicies = {};

};


#endif
