/*
 * Copyright (c) 2012 Bjørn Forsman
 *
 * This file is part of fgui which is released under the MIT license. See
 * LICENSE.txt or go to http://www.opensource.org/licenses/MIT for full
 * license text.
 */

/**
 * @file
 *
 * fgui 3D graphics utility functions
 */

#ifndef FGUI_3D_H
#define FGUI_3D_H

#include <stdint.h>

/** A 4-element vector */
struct fgui_vec4 {
	union {
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		float vec[4];
	};
};

/** A 4x4 matrix */
typedef struct { float mat[4][4]; } fgui_mat4_t;

/** A 4x4 matrix */
struct fgui_mat4 {
	float mat[4][4];
};

/** Multiply two 4x4 matrices */
void fgui_mult_matrix(struct fgui_mat4 *mat1, struct fgui_mat4 *mat2, struct fgui_mat4 *out);

/** Multiply a 4x4 matrix and 4x1 vector (result: 4x1 vector) */
void fgui_mult_mat_vec(const struct fgui_mat4 *m, const struct fgui_vec4 *vec, struct fgui_vec4 *out);

/** Multiply matrix by a translation matrix (created from x,y,z parameters) */
void fgui_translate(float x, float y, float z, struct fgui_vec4 *in, struct fgui_vec4 *out);

/** Multiply vector by a scale matrix (created from x,y,z parameters) */
void fgui_scale(float x, float y, float z, const struct fgui_vec4 *in, struct fgui_vec4 *out);

/** Normalize vector (length of 'out' vector will be 1) */
void fgui_normalize_vec(struct fgui_vec4 *in, struct fgui_vec4 *out);

/** Performs scaling, rotation and translation operations on a vector */
void fgui_transform_vector(const struct fgui_vec4 *in,
		float scale_x, float scale_y, float scale_z,
		int rot_x, int rot_y, int rot_z, /* or (angle, rotation_axis)? */
		float translate_x, float translate_y, float translate_z,
		struct fgui_vec4 *out);

/** A 4x4 identity matrix */
extern const struct fgui_mat4 identity4;

/** A box described by 8 vertices */
extern const struct fgui_vec4 box_vertices[8];


#endif /* FGUI_3D_H */
