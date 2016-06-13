#include "PickableObject.h"

PickableObject::PickableObject() { }

PickableObject::~PickableObject() { }

void PickableObject::setBoundingBox(std::vector<Vertex>& vertices) {
	float min_x, max_x,
	      min_y, max_y,
	      min_z, max_z;
	min_x = max_x = vertices[0].x;
	min_y = max_y = vertices[0].y;
	min_z = max_z = vertices[0].z;
	for(Vertex v : vertices) {
	    if (v.x < min_x) min_x = v.x;
	    if (v.x > max_x) max_x = v.x;
	    if (v.y < min_y) min_y = v.y;
	    if (v.y > max_y) max_y = v.y;
	    if (v.z < min_z) min_z = v.z;
	    if (v.z > max_z) max_z = v.z;
	}
}