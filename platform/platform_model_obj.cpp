/*
DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
Version 2, December 2004

Copyright (C) 2011-2016 Mark E Sowden <markelswo@gmail.com>

Everyone is permitted to copy and distribute verbatim or modified
copies of this license document, and changing it is allowed as long
as the name is changed.

DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

0. You just DO WHAT THE FUCK YOU WANT TO.
*/

#include "platform.h"

#include "platform_model.h"

/*
	OBJ Static Model Format
*/

enum
{
	OBJ_SYNTAX_COMMENT			= '#',
	OBJ_SYNTAX_OBJECT			= 'o',
	OBJ_SYNTAX_GROUP			= 'g',
	OBJ_SYNTAX_SMOOTH			= 's',
	OBJ_SYNTAX_VERTEX			= 'v',
	OBJ_SYNTAX_VERTEX_ST		= 't',	// vt
	OBJ_SYNTAX_VERTEX_NORMAL	= 'n',	// vn
	OBJ_SYNTAX_FACE				= 'f',
};

typedef struct OBJFace_s
{

} OBJFace_t;

std::vector<float*> vertices;
std::vector<float*> normals;

std::ifstream pl_obj_data;

void _plUnloadOBJModel()
{
	if (pl_obj_data.is_open())
		pl_obj_data.close();

	// Clear out all the allocated memory.
	for (unsigned int i = 0; i < vertices.size(); i++)
		if (vertices[i]) delete vertices[i];
	for (unsigned int i = 0; i < normals.size(); i++)
		if (normals[i]) delete normals[i];

	// Shrink our vectors down.
	vertices.empty();
	vertices.shrink_to_fit();
	normals.empty();
	normals.shrink_to_fit();
}

PLStaticModel *plLoadOBJModel(const PLchar *path)
{
	plSetErrorFunction("plLoadOBJModel");

	pl_obj_data.open(path, std::ifstream::binary);
	if (!pl_obj_data)
	{
		plSetError("Failed to load file! (%s)\n", path);
		return nullptr;
	}

	// Get the length of the file.
	pl_obj_data.seekg(0, pl_obj_data.end);
	std::streamoff length = pl_obj_data.tellg();
	pl_obj_data.seekg(0, pl_obj_data.beg);
	if (length <= 0)
	{
		_plUnloadOBJModel();

		plSetError("Invalid OBJ model! (%s)\n", path);
		return nullptr;
	}

	std::string line;
	while (std::getline(pl_obj_data, line))
	{
		switch (line[0])
		{
		case OBJ_SYNTAX_COMMENT:continue;
		case OBJ_SYNTAX_VERTEX:
		{
			if (line[1] == OBJ_SYNTAX_VERTEX_NORMAL)
			{
				plVector3f_t normal;
				std::sscanf(line.c_str() + 2, "%f %f %f", &normal[0], &normal[1], &normal[2]);
				
				float *vnormal = new float[3];
				memcpy(vnormal, normal, sizeof(plVector3f_t));
				normals.push_back(vnormal);
			}
			else if (line[1] == OBJ_SYNTAX_VERTEX_ST)
			{ }
			else // Vertex coords
			{
				plVector3f_t position;
				std::sscanf(line.c_str() + 2, "%f %f %f", &position[0], &position[1], &position[2]);
				
				float *vposition = new float[3];
				memcpy(vposition, position, sizeof(plVector3f_t));
				vertices.push_back(vposition);
			}
		}
		break;
		case OBJ_SYNTAX_FACE:
		{

		}
		break;
		default:
			// Materials are ignored for now, do we want these?
			if (!strncmp("mtllib", line.c_str(), 6))
				continue;
			else if (!strncmp("usemtl", line.c_str(), 6))
				continue;
			break;
		}
	}

	PLStaticModel *model = plCreateStaticModel();
	if (!model)
	{
		plSetError("Failed to create static model!\n");

		_plUnloadOBJModel();
		return nullptr;
	}

	model->num_triangles	= 0;
	model->num_vertices		= vertices.size();
	model->primitive		= VL_PRIMITIVE_POINTS;

	// Allocate vertex/triangle arrays.
	model->frame.vertices = new plVertex_t[model->num_vertices];
	for (unsigned int i = 0; i < model->num_vertices; i++)
	{
		plVertex_t *vertex = &model->frame.vertices[0];
		plVectorCopy(vertices[i], vertex->position);
	}

	_plUnloadOBJModel();

	return model;
}