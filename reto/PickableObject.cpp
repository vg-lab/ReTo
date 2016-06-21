#include "PickableObject.h"
#include <algorithm>

namespace reto
{
	//unsigned int PickableObject::globalId = 0;

	/*rogramShader PickableObject::program = PickableObject::initProgramShader();

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
		program.addUniform("modelViewProj");
		return program;
	}*/

	PickableObject::PickableObject( void )
  {
  }

	PickableObject::~PickableObject( void )
  {
  }

	void PickableObject::setBoundingBox(std::vector<Vertex>& vertices)
  {
		float minX, maxX, minY, maxY, minZ, maxZ;
		minX = maxX = vertices[0].x;
		minY = maxY = vertices[0].y;
		minZ = maxZ = vertices[0].z;
		for( const Vertex& v : vertices )
    {
			if (v.x < minX) minX = v.x;
			if (v.x > maxX) maxX = v.x;
			if (v.y < minY) minY = v.y;
			if (v.y > maxY) maxY = v.y;
			if (v.z < minZ) minZ = v.z;
			if (v.z > maxZ) maxZ = v.z;
		}
	}

	bool PickableObject::intersection( ray r )
  	{
		float t0, t1, tmin = 0.0, tmax = 0.0;

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
