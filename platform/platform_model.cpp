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

#include "platform_model.h"

/*	PLATFORM MODEL LOADER	*/

std::vector<float> plGenerateNormal(plVector3f_t a, plVector3f_t b, plVector3f_t c)
{
	plVector3f_t x, y;
	plVectorSubtract3fv(c, b, x);
	plVectorSubtract3fv(a, b, y);

	plVector3f_t normal;
	plVectorClear(normal);
	plCrossProduct(x, y, normal);
	plVectorNormalize(normal);
	
	std::vector<float> vnorm = { normal[0], normal[1], normal[2] };
	return vnorm;
}

void plGenerateStaticModelNormals(PLStaticModel *model)
{
	PLModelFrame *frame = &model->frame;
#if 0 // per face...
	for (int i = 0; i < model->num_triangles; i++)
	{
		std::vector<float> normal = plGenerateNormal(
			frame->vertices[frame->triangles[i].indices[0]].position,
			frame->vertices[frame->triangles[i].indices[1]].position,
			frame->vertices[frame->triangles[i].indices[2]].position);
		
		frame->triangles[i].normal[PL_X] = normal[PL_X];
		frame->triangles[i].normal[PL_Y] = normal[PL_Y];
		frame->triangles[i].normal[PL_Z] = normal[PL_Z];
	}
#else // per vertex...
	for (plVertex_t *vertex = &frame->vertices[0]; vertex; ++vertex)
	{
		for (plTriangle_t *triangle = &frame->triangles[0]; triangle; ++triangle)
		{

		}
	}
#endif
}

void plGenerateAnimatedModelNormals(plAnimatedModel_t *model)
{
	if (!model)
		return;

	// Calculate normals for each frame... Can't we transform these? I guess that's
	// only feasible with skeletal animation formats where we can get the transform
	// but hell, if there's a way to abstractily grab the direction of a face then
	// surely we could figure that out.
	for (PLModelFrame *frame = &model->frames[0]; frame; ++frame)
	{
		for (plVertex_t *vertex = &frame->vertices[0]; vertex; ++vertex)
		{
			for (plTriangle_t *triangle = &frame->triangles[0]; triangle; ++triangle)
			{
				
			}
		}
	}
}

void plGenerateSkeletalModelNormals(plSkeletalModel_t *model)
{
	if (!model)
		return;
}

/*
	Static Model
*/

PLStaticModel *plCreateStaticModel(void)
{
	plSetErrorFunction("plCreateStaticModel");

	PLStaticModel *model = new PLStaticModel;
	if (!model)
		return nullptr;

	memset(model, 0, sizeof(PLStaticModel));

	return model;
}

// Less direct implementation to load a model (less efficient too).
PLStaticModel *plLoadStaticModel(const char *path)
{
	if (!path || path[0] == ' ')
		return nullptr;

	std::string extension(path);
	if (extension.find(".pskx") != std::string::npos)
		return plLoadOBJModel(path);
	else if (extension.find(".obj") != std::string::npos)
		return plLoadOBJModel(path);

	return nullptr;
}

void plDeleteStaticModel(PLStaticModel *model)
{
	if (!model)
	{
		plSetError("Invalid model!\n");
		return;
	}

	if (model->frame.triangles)
		delete model->frame.triangles;
	if (model->frame.vertices)
		delete model->frame.vertices;

	delete model;
}

/*	
	Animated Model
*/

plAnimatedModel_t *plCreateAnimatedModel(void)
{
	plSetErrorFunction("plCreateAnimatedModel");

	plAnimatedModel_t *model = new plAnimatedModel_t;
	if (!model)
		return nullptr;
	
	memset(model, 0, sizeof(plAnimatedModel_t));

	return model;
}

// Less direct implementation to load a model (less efficient too).
plAnimatedModel_t *plLoadAnimatedModel(const char *path)
{
	plSetErrorFunction("plLoadAnimatedModel");

	if (!path || path[0] == ' ')
	{
		plSetError("Invalid path!\n");
		return nullptr;
	}

	return nullptr;
}

void plDeleteAnimatedModel(plAnimatedModel_t *model)
{
	plSetErrorFunction("plDeleteAnimatedModel");

	if (!model)
	{
		plSetError("Invalid model!\n");
		return;
	}

	for (unsigned int i = 0; i < model->num_frames; i++)
	{
		if (&model->frames[i])
		{
			if (model->frames[i].triangles)
				delete model->frames[i].triangles;
			if (model->frames[i].vertices)
				delete model->frames[i].vertices;
			delete &model->frames[i];
		}
	}

	delete model;
}

/*
	UNREAL PSKX Static Model Format

	Model format introduced in Unreal Engine 2.0, sadly rather
	hard to dig up much information about it.
*/

#define PSKX_EXTENSION "pskx"

