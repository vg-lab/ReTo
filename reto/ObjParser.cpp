/*
 * Copyright (c) 2014-2016 GMRV/URJC.
 *
 * Authors: Juan Jose Garcia
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

#include "ObjParser.h"

namespace reto {
	std::string ObjParser::loadFile(std::string filename)
	{
		std::ifstream file(filename.c_str());
		std::stringstream buffer;

		buffer << file.rdbuf();
		std::string str = buffer.str();
		return str;
	};

	std::vector<std::string> ObjParser::split(const std::string& s, char c)
	{
		std::vector<std::string> v;
		std::string::size_type i = 0;
		std::string::size_type j = s.find(c);

		while (j != std::string::npos) {
			v.push_back(s.substr(i, j - i));
			i = ++j;
			j = s.find(c, j);

			if (j == std::string::npos)
				v.push_back(s.substr(i, s.length()));
		}
		return v;
	}

	bool ObjParser::isNumeric(const std::string& input)
	{
		return std::all_of(input.begin(), input.end(), ::isdigit);
	}

	bool ObjParser::isFloat(std::string& myString)
	{
		std::istringstream iss(myString);
		float f;
		iss >> std::noskipws >> f;
		return iss.eof() && !iss.fail();
	}

	std::vector<float> ObjParser::splitLineToFloats(std::string& line)
	{
		std::vector<float> values;
		std::vector<std::string> split_ = split(line, ' ');
		for (auto str : split_) {
			if (isFloat(str)) {
				values.push_back(std::stod(str));
			}
		}
		return values;
	}

	std::string ObjParser::trim(std::string& str)
	{
		size_t first = str.find_first_not_of(' ');
		size_t last = str.find_last_not_of(' ');
		return str.substr(first, (last - first + 1));
	}

	std::vector<int> ObjParser::splitFace(std::string& line)
	{
		std::vector<int> values;
		std::vector<std::string> _splitFace = split(line, '/');

		for (std::string face : _splitFace) {
			if (isNumeric(face))
				values.push_back(std::stoi(face));
		}
		return values;
	}

	void ObjParser::loadObj(std::string filename)
	{
		this->vertices.clear();
		this->normals.clear();
		this->texCoords.clear();
		this->indices.clear();

		std::vector<float> verts, normals, textures;
		std::map<std::string, int> idxCache;
		int idx = 0;

		std::vector<std::string> lines = split(loadFile(filename), '\n');
		for (auto line : lines) {
			std::cout << line << std::endl;
			auto elems = split(line, ' ');//resplit(line, "/\s+/");
			if (elems.empty()) continue;
			elems.erase(elems.begin());

			std::string type = trim(line.substr(0, 2));
			if (type == "v") {
				auto values = splitLineToFloats(line);
				verts.push_back(values[0]);
				verts.push_back(values[1]);
				verts.push_back(values[2]);
			}
			else if (type == "vn") {
				auto values = splitLineToFloats(line);
				normals.push_back(values[0]);
				normals.push_back(values[1]);
				normals.push_back(values[2]);
			}
			else if (type == "vt") {
				auto values = splitLineToFloats(line);
				textures.push_back(values[0]);
				textures.push_back(values[1]);
			}
			else if (type == "f") {
				bool quad = false;
				for (int j = 0, size = elems.size(); j < size; ++j) {
					if (j == 3 && !quad) {
						j = 2;
						quad = true;
					}
					if (idxCache.find(elems[j]) != idxCache.end()) {
						this->indices.push_back(idxCache[elems[j]]);
					}
					else {
						std::vector<int> vertex = splitFace(elems[j]);
						auto v = (vertex[0] - 1) * 3;
						this->vertices.push_back(verts[v]);
						this->vertices.push_back(verts[v + 1]);
						this->vertices.push_back(verts[v + 2]);
						if (!textures.empty()) {
							auto tc = (vertex[1] - 1) * 2;
							this->texCoords.push_back(textures[tc]);
							this->texCoords.push_back(textures[tc + 1]);
						}
						auto n = (vertex[2] - 1) * 3;
						this->normals.push_back(normals[n]);
						this->normals.push_back(normals[n + 1]);
						this->normals.push_back(normals[n + 2]);
						idxCache[elems[j]] = idx;
						this->indices.push_back(idx);
						++idx;
					}
					if (j == 3 && quad) {
						this->indices.push_back(idxCache[elems[0]]);
					}
				}
			}
		}
	}
};