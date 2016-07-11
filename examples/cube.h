/*
 * Copyright (c) 2014-2016 GMRV/URJC.
 *
 * Authors: Cristian Rodriguez
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
#ifndef __RETO__CUBE__
#define __RETO__CUBE__

const int cubeNVertex = 24;
const int cubeNTriangleIndex = 12;

const unsigned int cubeTriangleIndex[] =
{
	//Face z = 1
	0,1,2,			1,3,2,
	//Face z = -1
	4,6,5,			5,6,7,
	//Face x = 1
	8,10,9,			9,10,11,
	//Face x = -1
	12,13,14,		13,15,14,
	//Face y = 1
	16,17,18,		17,19,18,
	//Face y = -1
	20,22,21,		21,22,23,
};

const float cubeVertexPos[] =
{
	//Face z = 1
	-1.0f,	-1.0f,	 1.0f, //0
	 1.0f,	-1.0f,	 1.0f, //1
	-1.0f,	 1.0f,	 1.0f, //2
	 1.0f,	 1.0f,	 1.0f, //3

	//Face z = -1
	-1.0f,	-1.0f,	-1.0f, //4
	 1.0f,	-1.0f,	-1.0f, //5
	-1.0f,	 1.0f,	-1.0f, //6
	 1.0f,	 1.0f,	-1.0f, //7

	//Face x = 1
	1.0f,	-1.0f,	-1.0f, //8
	1.0f,	-1.0f,	 1.0f, //9
	1.0f,	 1.0f,	-1.0f, //10
	1.0f,	 1.0f,	 1.0f, //11

	//Face x = -1
	-1.0f,	-1.0f,	-1.0f, //12
	-1.0f,	-1.0f,	 1.0f, //13
	-1.0f,	 1.0f,	-1.0f, //14
	-1.0f,	 1.0f,	 1.0f, //15

	//Face y = 1
	-1.0f,	 1.0f,	-1.0f, //16
	-1.0f,	 1.0f,	 1.0f, //17
	 1.0f,	 1.0f,	-1.0f, //18
	 1.0f,	 1.0f,	 1.0f, //19

	//Face y = -1
	-1.0f,	-1.0f,	-1.0f, //20
	-1.0f,	-1.0f,	 1.0f, //21
	 1.0f,	-1.0f,	-1.0f, //22
	 1.0f,	-1.0f,	 1.0f  //23
 };

const float cubeVertexNormal[] =
{
	//Face z = 1
	0.0f,	0.0f,	 1.0f,
	0.0f,	0.0f,	 1.0f,
	0.0f,	0.0f,	 1.0f,
	0.0f,	0.0f,	 1.0f,

	//Face z = -1
	0.0f,	0.0f,	-1.0f,
	0.0f,	0.0f,	-1.0f,
	0.0f,	0.0f,	-1.0f,
	0.0f,	0.0f,	-1.0f,

	//Face x = 1
	1.0f,	0.0f,	 0.0f,
	1.0f,	0.0f,	 0.0f,
	1.0f,	0.0f,	 0.0f,
	1.0f,	0.0f,	 0.0f,

	//Face x = -1
	-1.0f,	0.0f,	 0.0f,
	-1.0f,	0.0f,	 0.0f,
	-1.0f,	0.0f,	 0.0f,
	-1.0f,	0.0f,	 0.0f,

	//Face y = 1
	0.0f,	1.0f,	0.0f,
	0.0f,	1.0f,	0.0f,
	0.0f,	1.0f,	0.0f,
	0.0f,	1.0f,	0.0f,

	//Face y = -1
	0.0f,	-1.0f,	0.0f,
	0.0f,	-1.0f,	0.0f,
	0.0f,	-1.0f,	0.0f,
	0.0f,	-1.0f,	0.0f
 };

const float cubeVertexColor[] =
{
	//Face z = 1
	0.0f,	0.0f,	 1.0f,
	0.0f,	0.0f,	 1.0f,
	0.0f,	0.0f,	 1.0f,
	0.0f,	0.0f,	 1.0f,

	//Face z = -1
	0.6f,	0.6f,	 1.0f,
	0.6f,	0.6f,	 1.0f,
	0.6f,	0.6f,	 1.0f,
	0.6f,	0.6f,	 1.0f,

	//Face x = 1
	1.0f,	0.0f,	 0.0f,
	1.0f,	0.0f,	 0.0f,
	1.0f,	0.0f,	 0.0f,
	1.0f,	0.0f,	 0.0f,

	//Face x = -1
	1.0f,	0.6f,	 0.6f,
	1.0f,	0.6f,	 0.6f,
	1.0f,	0.6f,	 0.6f,
	1.0f,	0.6f,	 0.6f,

	//Face y = 1
	0.0f,	1.0f,	0.0f,
	0.0f,	1.0f,	0.0f,
	0.0f,	1.0f,	0.0f,
	0.0f,	1.0f,	0.0f,

	//Face y = -1
	0.6f,	1.0f,	0.6f,
	0.6f,	1.0f,	0.6f,
	0.6f,	1.0f,	0.6f,
	0.6f,	1.0f,	0.6f
 };

const float cubeVertexTexCoord[] =
{
	//Face z = 1
	 0.0f, 0.0f,
	 1.0f, 0.0f,
	 0.0f, 1.0f,
	 1.0f, 1.0f,

	//Face z = -1
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,

	//Face x = 1
	0.0f,	1.0f,
	1.0f,	1.0f,
	0.0f,	0.0f,
	1.0f,	0.0f,

	//Face x = -1
	0.0f,	0.0f,
	1.0f,	0.0f,
	0.0f,	1.0f,
	1.0f,	1.0f,

	//Face y = 1
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,

	//Face y = -1
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
};

#endif /* __RETO__CUBE__ */
