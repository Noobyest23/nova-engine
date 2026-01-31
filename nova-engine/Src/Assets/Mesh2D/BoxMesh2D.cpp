#include "BoxMesh2D.h"

BoxMesh2D::BoxMesh2D() {

	const float hw = 0.5f;
	const float hh = 0.5f;

	const std::vector<Vertex2D> box_verticies = {
		{{-hw, -hh}, {0.0f, 0.0f}},
		{{hw, -hh}, {1.0f, 0.0f}},
		{{hw, hh}, {1.0f, 1.0f}},
		{{-hw, hh}, {0.0f, 1.0f}}
	};
	
	const std::vector<unsigned int> box_indicies = {
		0, 1, 2,
		2, 3, 0
	};

	verticies = box_verticies;
	indicies = box_indicies;

	Upload();
}
