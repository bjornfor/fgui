/*
 * Copyright (c) 2012 Bjørn Forsman
 *
 * This file is part of fgui which is released under the MIT license. See
 * LICENSE.txt or go to http://www.opensource.org/licenses/MIT for full
 * license text.
 */

#include <math.h>

#include "fgui_3d.h"

const struct fgui_mat4 identity = {
	.mat = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	},
};

const struct fgui_vec4 box_vertices[8] = {
	/* the w=1 (last element) means it's a position, not a direction */
	{ 1,  1,  1,  1},
	{ 1, -1,  1,  1},
	{-1, -1,  1,  1},
	{-1,  1,  1,  1},
	{ 1,  1, -1,  1},
	{ 1, -1, -1,  1},
	{-1, -1, -1,  1},
	{-1,  1, -1,  1},
};

void fgui_mult_matrix(struct fgui_mat4 *mat1, struct fgui_mat4 *mat2, struct fgui_mat4 *out)
{
	int i;
	int row, col;
	float elem;
	const int dim = 4;

	for (row = 0; row < dim; row++)
	{
		for (col = 0; col < dim; col++)
		{
			elem = 0.0;
			for (i = 0; i < dim; i++)
			{
				elem += mat1->mat[row][i] * mat2->mat[i][col];
			}
			out->mat[row][col] = elem;
		}
	}
}

void fgui_mult_mat_vec(const struct fgui_mat4 *m, const struct fgui_vec4 *vec, struct fgui_vec4 *out)
{
	out->vec[0] = m->mat[0][0] * vec->vec[0]
	            + m->mat[0][1] * vec->vec[1]
	            + m->mat[0][2] * vec->vec[2]
	            + m->mat[0][3] * vec->vec[3];

	out->vec[1] = m->mat[1][0] * vec->vec[0]
	            + m->mat[1][1] * vec->vec[1]
	            + m->mat[1][2] * vec->vec[2]
	            + m->mat[1][3] * vec->vec[3];

	out->vec[2] = m->mat[2][0] * vec->vec[0]
	            + m->mat[2][1] * vec->vec[1]
	            + m->mat[2][2] * vec->vec[2]
	            + m->mat[2][3] * vec->vec[3];

	out->vec[3] = m->mat[3][0] * vec->vec[0]
	            + m->mat[3][1] * vec->vec[1]
	            + m->mat[3][2] * vec->vec[2]
	            + m->mat[3][3] * vec->vec[3];
}

void fgui_translate(float x, float y, float z, struct fgui_vec4 *in, struct fgui_vec4 *out)
{
	/* translation matrix */
	struct fgui_mat4 transmat = {
		.mat = {
			{1, 0, 0, x},
			{0, 1, 0, y},
			{0, 0, 1, z},
			{0, 0, 0, 1},
		}
	};
	fgui_mult_mat_vec(&transmat, in, out);
}


void fgui_scale(float x, float y, float z, const struct fgui_vec4 *in, struct fgui_vec4 *out)
{
	/* scale matrix */
	struct fgui_mat4 scalemat = {
		.mat = {
			{x, 0, 0, 0},
			{0, y, 0, 0},
			{0, 0, z, 0},
			{0, 0, 0, 1},
		}
	};
	fgui_mult_mat_vec(&scalemat, in, out);
}

void fgui_normalize_vec(struct fgui_vec4 *in, struct fgui_vec4 *out)
{
	float len = sqrt((in->vec[0] * in->vec[0])
	               + (in->vec[1] * in->vec[1])
	               + (in->vec[2] * in->vec[2]));
	out->vec[0] = in->vec[0] / len;
	out->vec[1] = in->vec[1] / len;
	out->vec[2] = in->vec[2] / len;
	out->vec[3] = in->vec[3] / len;
}

void fgui_transform_vector(const struct fgui_vec4 *in,
		float scale_x, float scale_y, float scale_z,
		int rot_x, int rot_y, int rot_z, /* or (angle, rotation_axis)? */
		float translate_x, float translate_y, float translate_z,
		struct fgui_vec4 *out)
{
	struct fgui_vec4 scaled;
	struct fgui_vec4 rotated;
	struct fgui_vec4 translated;

	fgui_scale(scale_x, scale_y, scale_z, in, &scaled);
	// TOOD: rotate
	rotated = scaled;
	fgui_translate(translate_x, translate_y, translate_z, &rotated, out);
}
