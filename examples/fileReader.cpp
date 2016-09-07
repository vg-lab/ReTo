/*
 * Copyright (c) 2014-2016 GMRV/URJC.
 *
 * Authors: Cristian Rodríguez Bernal
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

#include <iostream>
#include "ObjParser.h"

int main(int argc, char** argv) {
  //if (argc !== 3) return;
	std::string filename = argv[1];
  auto loader = reto::ObjParser();
	auto model = loader.loadObj(filename, true, true);

	std::ofstream fout("./omg.json");
	std::ostringstream oss;

	fout << "{" << std::endl;

	if (!model.vertices.empty())
	{
		// Convert all but the last element to avoid a trailing ","
		std::copy(model.vertices.begin(), model.vertices.end() - 1,
			std::ostream_iterator<float>(oss, ","));

		// Now add the last element with no delimiter
		oss << model.vertices.back();
	}
	fout << "\tvertices: [" << oss.str() << "]," << std::endl;

	std::ostringstream oss2;

	if (!model.normals.empty())
	{
		// Convert all but the last element to avoid a trailing ","
		std::copy(model.normals.begin(), model.normals.end() - 1,
			std::ostream_iterator<float>(oss2, ","));

		// Now add the last element with no delimiter
		oss2 << model.normals.back();
	}
	fout << "\tnormals: [" << oss2.str() << "]," << std::endl;

	std::ostringstream oss3;

	if (!model.texCoords.empty())
	{
		// Convert all but the last element to avoid a trailing ","
		std::copy(model.texCoords.begin(), model.texCoords.end() - 1,
			std::ostream_iterator<float>(oss3, ","));

		// Now add the last element with no delimiter
		oss3 << model.texCoords.back();
	}
	fout << "\ttexCoords: [" << oss3.str() << "]," << std::endl;

	std::ostringstream oss4;

	if (!model.indices.empty())
	{
		// Convert all but the last element to avoid a trailing ","
		std::copy(model.indices.begin(), model.indices.end() - 1,
			std::ostream_iterator<float>(oss4, ","));

		// Now add the last element with no delimiter
		oss4 << model.indices.back();
	}
	fout << oss4.str() << std::endl;
	fout << "\tindices: [" << oss4.str() << "]," << std::endl;


	std::ostringstream oss5;
	if (!model.tangents.empty())
	{
		// Convert all but the last element to avoid a trailing ","
		std::copy(model.tangents.begin(), model.tangents.end() - 1,
			std::ostream_iterator<float>(oss5, ","));

		// Now add the last element with no delimiter
		oss4 << model.tangents.back();
	}
	fout << oss5.str() << std::endl;
	fout << "\ttangents: [" << oss5.str() << "]," << std::endl;


	std::ostringstream oss6;
	if (!model.bitangents.empty())
	{
		// Convert all but the last element to avoid a trailing ","
		std::copy(model.bitangents.begin(), model.bitangents.end() - 1,
			std::ostream_iterator<float>(oss6, ","));

		// Now add the last element with no delimiter
		oss4 << model.bitangents.back();
	}
	fout << oss6.str() << std::endl;
	fout << "\tbitangents: [" << oss6.str() << "]," << std::endl;

	fout << "}" << std::endl;
	fout.close();

	char a;
	//std::cin >> a;
	return 0;
}
