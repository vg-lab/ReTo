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

#include <vector>
#include "ProgramShader.h"

namespace reto {
	class PickableObject {
		public:
			typedef struct Vertex {
				float x, y, z;
			} Vertex;

			typedef struct {
				Vertex origin, direction;
			} ray;

			PickableObject();
			~PickableObject();

			bool intersection(ray r);

			void setBoundingBox(std::vector<Vertex>& vertices);

			uint inline id() {
				return objectId;
			}

			void renderWithPick() {}	// TODO: virtual = 0;
		protected:
			typedef struct {
				float xMin;
				float xMax;
				float yMin;
				float yMax;
				float zMin;
				float zMax;
		  	} TBoundingBox;

		  	TBoundingBox box;

		  	static uint globalId;

		  	uint objectId;

		  	static ProgramShader program;

		  	static ProgramShader initProgramShader();
	};
}

#endif /*_PICKABLE_OBJECT_H_*/