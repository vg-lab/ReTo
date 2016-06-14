#include "PickableObject.h"

namespace reto {
	uint PickableObject::globalId = 0;

	ProgramShader PickableObject::program = PickableObject::initProgramShader();

	ProgramShader PickableObject::initProgramShader() {
		ProgramShader program;
		program.loadFromText(
			"#version 330 core\n"
			"layout(location = 0) in inPos;\n"
			"uniform mat4 modelViewProj;\n"
			"void main() {\n"
			"	gl_Position = modelViewProj * vec4(inPos, 1.0);\n"
			"}\n",
			"#version 330 core\n"
			"out vec4 color;\n"
			"void main() {\n"
			"	color = vec4(1.0);\n"
			"}\n"
		);
		program.add_uniform("modelViewProj");
		return program;
	}

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
	bool PickableObject::intersection(ray r) {
		float t0, t1, tmin, tmax;

		// X
		t0 = std::min(box.xMin - r.origin.x / r.direction.x,
						(box.xMax - r.origin.x) / r.direction.x);
		t1 = std::max(box.xMin - r.origin.x / r.direction.x,
						(box.xMax - r.origin.x) / r.direction.x);
		tmin = std::max(t0, tmin);
		tmax = std::min(t1, tmax);
		if (tmax <= tmin)
			return false;

		// Y
		t0 = std::min(box.yMin - r.origin.y / r.direction.y,
						(box.yMax - r.origin.y) / r.direction.y);
		t1 = std::max(box.yMin - r.origin.y / r.direction.y,
						(box.yMax - r.origin.y) / r.direction.y);
		tmin = std::max(t0, tmin);
		tmax = std::min(t1, tmax);
		if (tmax <= tmin)
			return false;

		// Z
		t0 = std::min(box.zMin - r.origin.z / r.direction.z,
						(box.zMax - r.origin.z) / r.direction.z);
		t1 = std::max(box.xMin - r.origin.z / r.direction.z,
						(box.zMax - r.origin.z) / r.direction.z);
		tmin = std::max(t0, tmin);
		tmax = std::min(t1, tmax);
		if (tmax <= tmin)
			return false;
		return true;
	}
}