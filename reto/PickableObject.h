/*
 * Copyright (c) 2014-2016 GMRV/URJC.
 *
 * Authors: Cristian Rodriguez Bernal (crodriguez)
 *
 * This file is part of ReTo <https://gitlab.gmrv.es/nsviz/ReTo>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
 
#ifndef _PICKABLE_OBJECT_H_ 
#define _PICKABLE_OBJECT_H_ 

class PickableObject {
public:
	PickableObject();
	~PickableObject();

	void setBoundingBox(std::vector<Vertex>& vertices);
protected:
	typedef struct {
		float x, y, z;
	} Vertex;

	typedef struct {
		float xMin;
		float xMax;
		float yMin;
		float yMax;
		float zMin;
		float zMax;
  	} TBoundingBox;

  	TBoundingBox box;

  	/*GLfloat
	    min_x, max_x,
	    min_y, max_y,
	    min_z, max_z;
	min_x = max_x = mesh->vertices[0].x;
	min_y = max_y = mesh->vertices[0].y;
	min_z = max_z = mesh->vertices[0].z;
  	for (int i = 0; i < mesh->vertices.size(); i++) {
	    if (mesh->vertices[i].x < min_x) min_x = mesh->vertices[i].x;
	    if (mesh->vertices[i].x > max_x) max_x = mesh->vertices[i].x;
	    if (mesh->vertices[i].y < min_y) min_y = mesh->vertices[i].y;
	    if (mesh->vertices[i].y > max_y) max_y = mesh->vertices[i].y;
	    if (mesh->vertices[i].z < min_z) min_z = mesh->vertices[i].z;
	    if (mesh->vertices[i].z > max_z) max_z = mesh->vertices[i].z;
  	}*/
};

#endif /*_PICKABLE_OBJECT_H_*/