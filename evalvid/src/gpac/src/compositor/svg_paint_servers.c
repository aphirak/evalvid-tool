/*
 *			GPAC - Multimedia Framework C SDK
 *
 *			Authors: Cyril Concolato - Jean le Feuvre
 *				Copyright (c) 2005-200X ENST
 *					All rights reserved
 *
 *  This file is part of GPAC / Scene Compositor sub-project
 *
 *  GPAC is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *   
 *  GPAC is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *   
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
 *
 */

#include "visual_manager.h"

#ifndef GPAC_DISABLE_SVG
#include "nodes_stacks.h"
#include "texturing.h"



typedef struct
{
	GF_TextureHandler txh;
	Bool no_rgb_support;
	Bool linear;
	Bool animated;
	Fixed *keys;
	u32 *cols;
} SVG_GradientStack;


static void SVG_DestroyPaintServer(GF_Node *node)
{
	SVG_GradientStack *st = (SVG_GradientStack *) gf_node_get_private(node);
	if (st) {
		if (st->cols) free(st->cols);
		if (st->keys) free(st->keys);
		gf_sc_texture_destroy(&st->txh);
		free(st);
	}
}


static GF_Node *svg_copy_gradient_attributes_from(GF_Node *node, SVGAllAttributes *all_atts)
{
	GF_Node *href_node;
	SVGAllAttributes all_href_atts;
	GF_FieldInfo info;

	/*check gradient redirection ...*/
	href_node = node;
	while (href_node && gf_node_get_attribute_by_tag(href_node, TAG_XLINK_ATT_href, 0, 0, &info)==GF_OK) {
		XMLRI*iri = (XMLRI*)info.far_ptr;

		if (iri->type != XMLRI_ELEMENTID) {
			GF_SceneGraph *sg = gf_node_get_graph(node);
			GF_Node *n = gf_sg_find_node_by_name(sg, &(iri->string[1]));
			if (n) {
				iri->type = XMLRI_ELEMENTID;
				iri->target = n;
				gf_node_register_iri(sg, iri);
				free(iri->string);
				iri->string = NULL;
			} else {
				break;
			}
		}
		href_node = ((XMLRI*)info.far_ptr)->target;
		if (href_node == node) href_node = NULL;
	}
	if (href_node == node) href_node = NULL;

	if (href_node) {
		gf_svg_flatten_attributes((SVG_Element *)href_node, &all_href_atts);
		if (!all_atts->gradientUnits) all_atts->gradientUnits = all_href_atts.gradientUnits;
		if (!all_atts->gradientTransform) all_atts->gradientTransform = all_href_atts.gradientTransform;
		if (!all_atts->cx) all_atts->cx = all_href_atts.cx;
		if (!all_atts->cy) all_atts->cy = all_href_atts.cy;
		if (!all_atts->r) all_atts->r = all_href_atts.r;
		if (!all_atts->fx) all_atts->fx = all_href_atts.fx;
		if (!all_atts->fy) all_atts->fy = all_href_atts.fy;
		if (!all_atts->spreadMethod) all_atts->spreadMethod = all_href_atts.spreadMethod;
		if (!all_atts->x1) all_atts->x1 = all_href_atts.x1;
		if (!all_atts->x2) all_atts->x2 = all_href_atts.x2;
		if (!all_atts->y1) all_atts->y1 = all_href_atts.y1;
		if (!all_atts->y2) all_atts->y2 = all_href_atts.y2;
	}

	return href_node;
}

static void svg_gradient_traverse(GF_Node *node, GF_TraverseState *tr_state, Bool real_traverse)
{
	GF_STENCIL stencil;
	u32 count, nb_col;
	Bool is_dirty, all_dirty;
	Fixed alpha, max_offset;
	SVGAllAttributes all_atts;
	SVGPropertiesPointers backup_props_1;
	u32 backup_flags_1;
	GF_Node *href_node;
	GF_ChildNodeItem *children;
	SVG_GradientStack *st = (SVG_GradientStack *) gf_node_get_private(node);

	gf_svg_flatten_attributes((SVG_Element *)node, &all_atts);
	href_node = svg_copy_gradient_attributes_from(node, &all_atts);
	compositor_svg_traverse_base(node, &all_atts, tr_state, &backup_props_1, &backup_flags_1);

	if (real_traverse &&
		! (tr_state->svg_flags & (GF_SG_SVG_STOPCOLOR_OR_OPACITY_DIRTY|GF_SG_SVG_COLOR_DIRTY)) 
		&& !gf_node_dirty_get(node) 
		&& !st->txh.needs_refresh) 
	{
		memcpy(tr_state->svg_props, &backup_props_1, sizeof(SVGPropertiesPointers));
		tr_state->svg_flags = backup_flags_1;
		return;
	}

	/*for gradients we must traverse the gradient stops to trigger animations, even if the 
	gradient is not marked as dirty*/
	all_dirty = tr_state->svg_flags & (GF_SG_SVG_STOPCOLOR_OR_OPACITY_DIRTY|GF_SG_SVG_COLOR_DIRTY);
	is_dirty = 0;
	if (gf_node_dirty_get(node)) {
		is_dirty = all_dirty = 1;
		gf_node_dirty_clear(node, 0);
		if (st->cols) free(st->cols);
		st->cols = NULL;
		if (st->keys) free(st->keys);
		st->keys = NULL;

		st->animated = gf_node_animation_count(node) ? 1 : 0;
	}

	children = ((SVG_Element *)node)->children;
	if (!children && href_node) {
		children = ((SVG_Element *)href_node)->children;
	}

	if (!st->cols) {
		count = gf_node_list_get_count(children);
		st->cols = (u32*)malloc(sizeof(u32)*count);
		st->keys = (Fixed*)malloc(sizeof(Fixed)*count);
	}
	nb_col = 0;
	max_offset = 0;
	while (children) {
		SVGPropertiesPointers backup_props_2;
		u32 backup_flags_2;
		Fixed key;
		GF_Node *stop = children->node;
		children = children->next;
		if (gf_node_get_tag((GF_Node *)stop) != TAG_SVG_stop) continue;

		gf_svg_flatten_attributes((SVG_Element*)stop, &all_atts);
		compositor_svg_traverse_base(stop, &all_atts, tr_state, &backup_props_2, &backup_flags_2);

		if (gf_node_animation_count(stop))
			st->animated = 1;

		if (all_dirty || gf_node_dirty_get(stop)) {
			is_dirty = 1;
			gf_node_dirty_clear(stop, 0);

			alpha = FIX_ONE;
			if (tr_state->svg_props->stop_opacity && (tr_state->svg_props->stop_opacity->type==SVG_NUMBER_VALUE) )
				alpha = tr_state->svg_props->stop_opacity->value;

			if (tr_state->svg_props->stop_color) {
				if (tr_state->svg_props->stop_color->color.type == SVG_COLOR_CURRENTCOLOR) {
					st->cols[nb_col] = GF_COL_ARGB_FIXED(alpha, tr_state->svg_props->color->color.red, tr_state->svg_props->color->color.green, tr_state->svg_props->color->color.blue);
				} else {
					st->cols[nb_col] = GF_COL_ARGB_FIXED(alpha, tr_state->svg_props->stop_color->color.red, tr_state->svg_props->stop_color->color.green, tr_state->svg_props->stop_color->color.blue);
				}
			} else {
				st->cols[nb_col] = GF_COL_ARGB_FIXED(alpha, 0, 0, 0);
			}

			if (all_atts.offset) {
				key = all_atts.offset->value;
				if (all_atts.offset->type==SVG_NUMBER_PERCENTAGE) key/=100; 
			} else {
				key=0;
			}
			if (key>max_offset) max_offset=key;
			else key = max_offset;
			st->keys[nb_col] = key;
		} else {
			if (st->keys[nb_col]>max_offset) max_offset = st->keys[nb_col];
		}

		nb_col++;

		memcpy(tr_state->svg_props, &backup_props_2, sizeof(SVGPropertiesPointers));
		tr_state->svg_flags = backup_flags_2;
	}

	if (is_dirty) {
		u32 i;

		if (!st->txh.tx_io) gf_sc_texture_allocate(&st->txh);
		stencil = gf_sc_texture_get_stencil(&st->txh);
		if (!stencil) stencil = st->txh.compositor->rasterizer->stencil_new(st->txh.compositor->rasterizer, st->linear ? GF_STENCIL_LINEAR_GRADIENT : GF_STENCIL_RADIAL_GRADIENT);
		/*set stencil even if assigned, this invalidates the associated bitmap state in 3D*/
		gf_sc_texture_set_stencil(&st->txh, stencil);

		st->txh.transparent = 0;
		for (i=0; i<nb_col; i++) {
			if (GF_COL_A(st->cols[i]) != 0xFF) {
				st->txh.transparent = 1;
				break;
			}
		}

		st->txh.compositor->rasterizer->stencil_set_gradient_interpolation(stencil, st->keys, st->cols, nb_col);
		st->txh.compositor->rasterizer->stencil_set_gradient_mode(stencil, /*lg->spreadMethod*/ GF_GRADIENT_MODE_PAD);

		st->txh.needs_refresh = 1;
	} else {
		st->txh.needs_refresh = 0;
	}

	memcpy(tr_state->svg_props, &backup_props_1, sizeof(SVGPropertiesPointers));
	tr_state->svg_flags = backup_flags_1;
}

static void svg_update_gradient(SVG_GradientStack *st, GF_ChildNodeItem *children, Bool linear)
{
	SVGPropertiesPointers *svgp;
	GF_Node *node = st->txh.owner;
	GF_TraverseState *tr_state = st->txh.compositor->traverse_state;

	if (!gf_node_dirty_get(node)) {
		if (!st->animated) return;
	}

	GF_SAFEALLOC(svgp, SVGPropertiesPointers);
	gf_svg_properties_init_pointers(svgp);
	tr_state->svg_props = svgp;

	svg_gradient_traverse(node, tr_state, 0);

	gf_svg_properties_reset_pointers(svgp);
	free(svgp);
	tr_state->svg_props = NULL;
}


static void svg_traverse_gradient(GF_Node *node, void *rs, Bool is_destroy)
{
	GF_TraverseState *tr_state = (GF_TraverseState *) rs;

	if (is_destroy) {
		SVG_DestroyPaintServer(node);
		return;
	}
	if (tr_state->traversing_mode != TRAVERSE_SORT) return;
	svg_gradient_traverse(node, tr_state, 1);
}

#define GRAD_TEXTURE_SIZE	128
#define GRAD_TEXTURE_HSIZE	64

static GF_Rect compositor_svg_get_gradient_bounds(GF_TextureHandler *txh, SVGAllAttributes *all_atts)
{
	GF_Rect rc;
	if (gf_node_get_tag(txh->owner)==TAG_SVG_radialGradient) {
		rc.x = rc.y = rc.width = FIX_ONE/2;
		if (all_atts->r) {
			rc.width = 2*all_atts->r->value;
			if (all_atts->r->type==SVG_NUMBER_PERCENTAGE) rc.width /= 100;
		}
		if (all_atts->cx) {
			rc.x = all_atts->cx->value;
			if (all_atts->cx->type==SVG_NUMBER_PERCENTAGE) rc.x /= 100;
		}
		if (all_atts->cy) {
			rc.y = all_atts->cy->value;
			if (all_atts->cy->type==SVG_NUMBER_PERCENTAGE) rc.y /= 100;
		}
		rc.height = rc.width;
		rc.x -= rc.width/2;
		rc.y -= rc.height/2;
	} else {
		rc.x = rc.y = rc.height = 0;
		rc.width = FIX_ONE;
		if (all_atts->x1) {
			rc.x = all_atts->x1->value;
			if (all_atts->x1->type==SVG_NUMBER_PERCENTAGE) rc.x /= 100;
		}
		if (all_atts->y1) {
			rc.y = all_atts->y1->value;
			if (all_atts->y1->type==SVG_NUMBER_PERCENTAGE) rc.y /= 100;
		}
		if (all_atts->x2) {
			rc.width = all_atts->x2->value;
			if (all_atts->x2->type==SVG_NUMBER_PERCENTAGE) rc.width/= 100;
		}
		rc.width -= rc.x;

		if (all_atts->y2) {
			rc.height = all_atts->y2->value;
			if (all_atts->y2->type==SVG_NUMBER_PERCENTAGE) rc.height /= 100;
		}
		rc.height -= rc.y;

		if (!rc.width) rc.width = rc.height;
		if (!rc.height) rc.height = rc.width;
	}
	rc.y += rc.height;
	return rc;
}

void compositor_svg_build_gradient_texture(GF_TextureHandler *txh)
{
	u32 i;
	Fixed size;
	GF_Matrix2D mat;
	GF_STENCIL stencil;
	GF_SURFACE surface;
	GF_STENCIL texture2D;
	GF_Path *path;
	GF_Err e;
	Bool transparent;
	SVGAllAttributes all_atts;
	SVG_GradientStack *st = (SVG_GradientStack *) gf_node_get_private(txh->owner);
	GF_Raster2D *raster = txh->compositor->rasterizer;


	if (!txh->tx_io) return;


	if (txh->data) {
		free(txh->data);
		txh->data = NULL;
	}
	stencil = gf_sc_texture_get_stencil(txh);
	if (!stencil) return;
	
	/*init our 2D graphics stuff*/
	texture2D = raster->stencil_new(raster, GF_STENCIL_TEXTURE);
	if (!texture2D) return;
	surface = raster->surface_new(raster, 1);
	if (!surface) {
		raster->stencil_delete(texture2D);
		return;
	}

	transparent = st->txh.transparent;
	if (st->no_rgb_support) transparent = 1;
	
	if (transparent) {
		if (!txh->data) {
			txh->data = (char *) malloc(sizeof(char)*GRAD_TEXTURE_SIZE*GRAD_TEXTURE_SIZE*4);
		} else {
			memset(txh->data, 0, sizeof(char)*txh->stride*txh->height);
		}
		e = raster->stencil_set_texture(texture2D, txh->data, GRAD_TEXTURE_SIZE, GRAD_TEXTURE_SIZE, 4*GRAD_TEXTURE_SIZE, GF_PIXEL_ARGB, GF_PIXEL_ARGB, 1);
	} else {
		if (!txh->data) {
			txh->data = (char *) malloc(sizeof(char)*GRAD_TEXTURE_SIZE*GRAD_TEXTURE_SIZE*3);
		}
		e = raster->stencil_set_texture(texture2D, txh->data, GRAD_TEXTURE_SIZE, GRAD_TEXTURE_SIZE, 3*GRAD_TEXTURE_SIZE, GF_PIXEL_RGB_24, GF_PIXEL_RGB_24, 1);
		/*try with ARGB (it actually is needed for GDIplus module since GDIplus cannot handle native RGB texture (it works in BGR)*/
		if (e) {
			/*remember for later use*/
			st->no_rgb_support = 1;
			transparent = 1;
			free(txh->data);
			txh->data = (char *) malloc(sizeof(char)*GRAD_TEXTURE_SIZE*GRAD_TEXTURE_SIZE*4);
			e = raster->stencil_set_texture(texture2D, txh->data, GRAD_TEXTURE_SIZE, GRAD_TEXTURE_SIZE, 4*GRAD_TEXTURE_SIZE, GF_PIXEL_ARGB, GF_PIXEL_ARGB, 1);
		}
	}

	if (e) {
		free(txh->data);
		txh->data = NULL;
		raster->stencil_delete(texture2D);
		raster->surface_delete(surface);
		return;
	}
	e = raster->surface_attach_to_texture(surface, texture2D);
	if (e) {
		raster->stencil_delete(texture2D);
		raster->surface_delete(surface);
		return;
	}

	size = INT2FIX(GRAD_TEXTURE_HSIZE);
	/*fill surface*/
	path = gf_path_new();
	gf_path_add_move_to(path, -size, -size);
	gf_path_add_line_to(path, size, -size);
	gf_path_add_line_to(path, size, size);
	gf_path_add_line_to(path, -size, size);
	gf_path_close(path);

	gf_mx2d_init(mat);
	txh->compute_gradient_matrix(txh, NULL, &mat, 0);

	gf_svg_flatten_attributes((SVG_Element*)txh->owner, &all_atts);

	if (all_atts.gradientUnits && (*(SVG_GradientUnit*)all_atts.gradientUnits==SVG_GRADIENTUNITS_OBJECT) ) {
		if (all_atts.gradientTransform)
			gf_mx2d_copy(mat, all_atts.gradientTransform->mat);

		gf_mx2d_add_scale(&mat, 2*size, 2*size);
		gf_mx2d_add_translation(&mat, -size, -size);
	} else {
		GF_Rect rc = compositor_svg_get_gradient_bounds(txh, &all_atts);
		/*recenter the gradient to use full texture*/
		gf_mx2d_add_translation(&mat, -rc.x, rc.height-rc.y);
		gf_mx2d_add_scale(&mat, gf_divfix(2*size, rc.width), gf_divfix(2*size , rc.height));
		gf_mx2d_add_translation(&mat, -size, -size);
	}

	raster->stencil_set_matrix(stencil, &mat);
	raster->surface_set_raster_level(surface, GF_RASTER_HIGH_QUALITY);
	raster->surface_set_path(surface, path);
	raster->surface_fill(surface, stencil);
	raster->surface_delete(surface);
	raster->stencil_delete(texture2D);
	gf_path_del(path);

	txh->width = GRAD_TEXTURE_SIZE;
	txh->height = GRAD_TEXTURE_SIZE;
	txh->transparent = transparent;
	txh->flags |= GF_SR_TEXTURE_NO_GL_FLIP;

	if (transparent) {
		u32 j;
		txh->stride = GRAD_TEXTURE_SIZE*4;

		/*back to RGBA texturing*/
		txh->pixelformat = GF_PIXEL_RGBA;
		for (i=0; i<txh->height; i++) {
			char *data = txh->data + i*txh->stride;
			for (j=0; j<txh->width; j++) {
				u32 val = *(u32 *) &data[4*j];
				data[4*j] = (val>>16) & 0xFF;
				data[4*j+1] = (val>>8) & 0xFF;
				data[4*j+2] = (val) & 0xFF;
				data[4*j+3] = (val>>24) & 0xFF;
			}
		}
	} else {
		txh->stride = GRAD_TEXTURE_SIZE*3;
		txh->pixelformat = GF_PIXEL_RGB_24;
	}
	gf_sc_texture_set_data(txh);
	return;
}


/* linear gradient */

static void SVG_UpdateLinearGradient(GF_TextureHandler *txh)
{
	SVG_Element *lg = (SVG_Element *) txh->owner;
	SVG_GradientStack *st = (SVG_GradientStack *) gf_node_get_private(txh->owner);

	svg_update_gradient(st, lg->children, 1);
}


static void SVG_LG_ComputeMatrix(GF_TextureHandler *txh, GF_Rect *bounds, GF_Matrix2D *mat, Bool for_3d)
{
	GF_STENCIL stencil;
	SFVec2f start, end;
	SVGAllAttributes all_atts;

	if (!txh->tx_io) return;
	stencil = gf_sc_texture_get_stencil(txh);
	if (!stencil) return;

	gf_svg_flatten_attributes((SVG_Element*)txh->owner, &all_atts);

	/*get "transfered" attributed from xlink:href if any*/
	svg_copy_gradient_attributes_from(txh->owner, &all_atts);

	gf_mx2d_init(*mat);

	/*gradient is a texture, only update the bounds*/
	if (for_3d) {
		GF_Rect rc;
		if (!all_atts.gradientUnits || (*(SVG_GradientUnit*)all_atts.gradientUnits==SVG_GRADIENTUNITS_OBJECT)) 
			return;

		/*get gradient bounds in local coord system*/
		rc = compositor_svg_get_gradient_bounds(txh, &all_atts);
		gf_mx2d_add_scale(mat, gf_divfix(rc.width, bounds->width), gf_divfix(rc.height, bounds->height));

		return;
	}

	if (all_atts.gradientTransform) 
		gf_mx2d_copy(*mat, all_atts.gradientTransform->mat );

	if (all_atts.x1) {
		start.x = all_atts.x1->value;
		if (all_atts.x1->type==SVG_NUMBER_PERCENTAGE) start.x /= 100;
	} else {
		start.x = 0;
	}
	if (all_atts.y1) {
		start.y = all_atts.y1->value;
		if (all_atts.y1->type==SVG_NUMBER_PERCENTAGE) start.y /= 100;
	} else {
		start.y = 0;
	}
	if (all_atts.x2) {
		end.x = all_atts.x2->value;
		if (all_atts.x2->type==SVG_NUMBER_PERCENTAGE) end.x /= 100;
	} else {
		end.x = FIX_ONE;
	}
	if (all_atts.y2) {
		end.y = all_atts.y2->value;
		if (all_atts.y2->type==SVG_NUMBER_PERCENTAGE) end.y /= 100;
	} else {
		end.y = 0;
	}

	txh->compositor->rasterizer->stencil_set_gradient_mode(stencil, (GF_GradientMode) all_atts.spreadMethod ? *(SVG_SpreadMethod*)all_atts.spreadMethod : 0);


	if (bounds && (!all_atts.gradientUnits || (*(SVG_GradientUnit*)all_atts.gradientUnits==SVG_GRADIENTUNITS_OBJECT)) ) {
		/*move to local coord system - cf SVG spec*/
		gf_mx2d_add_scale(mat, bounds->width, bounds->height);
		gf_mx2d_add_translation(mat, bounds->x, bounds->y  - bounds->height);
	}
	txh->compositor->rasterizer->stencil_set_linear_gradient(stencil, start.x, start.y, end.x, end.y);
}

void compositor_init_svg_linearGradient(GF_Compositor *compositor, GF_Node *node)
{
	SVG_GradientStack *st;
	GF_SAFEALLOC(st, SVG_GradientStack);

	gf_sc_texture_setup(&st->txh, compositor, node);
	st->txh.update_texture_fcnt = SVG_UpdateLinearGradient;
	st->txh.compute_gradient_matrix = SVG_LG_ComputeMatrix;
	st->linear = 1;
	gf_node_set_private(node, st);
	gf_node_set_callback_function(node, svg_traverse_gradient);
}

/* radial gradient */

static void SVG_UpdateRadialGradient(GF_TextureHandler *txh)
{
	SVG_Element *rg = (SVG_Element *) txh->owner;
	SVG_GradientStack *st = (SVG_GradientStack *) gf_node_get_private(txh->owner);

	svg_update_gradient(st, rg->children, 0);
}

static void SVG_RG_ComputeMatrix(GF_TextureHandler *txh, GF_Rect *bounds, GF_Matrix2D *mat, Bool for_3d)
{
	GF_STENCIL stencil;
	SFVec2f center, focal;
	Fixed radius;
	SVGAllAttributes all_atts;

	/*create gradient brush if needed*/
	if (!txh->tx_io) return;
	stencil = gf_sc_texture_get_stencil(txh);
	if (!stencil) return;

	gf_svg_flatten_attributes((SVG_Element*)txh->owner, &all_atts);

	/*get "transfered" attributed from xlink:href if any*/
	svg_copy_gradient_attributes_from(txh->owner, &all_atts);

	gf_mx2d_init(*mat);

	/*gradient is a texture, only update the bounds*/
	if (for_3d && bounds) {
		GF_Rect rc;
		if (!all_atts.gradientUnits || (*(SVG_GradientUnit*)all_atts.gradientUnits==SVG_GRADIENTUNITS_OBJECT)) 
			return;

		/*get gradient bounds in local coord system*/
		rc = compositor_svg_get_gradient_bounds(txh, &all_atts);
		gf_mx2d_add_translation(mat, gf_divfix(rc.x-bounds->x, rc.width), gf_divfix(bounds->y - rc.y, rc.height) );
		gf_mx2d_add_scale(mat, gf_divfix(rc.width, bounds->width), gf_divfix(rc.height, bounds->height));

		gf_mx2d_inverse(mat);
		return;
	}

	if (all_atts.gradientTransform) 
		gf_mx2d_copy(*mat, all_atts.gradientTransform->mat);
	
	if (all_atts.r) {
		radius = all_atts.r->value;
		if (all_atts.r->type==SVG_NUMBER_PERCENTAGE) radius /= 100;
	} else {
		radius = FIX_ONE/2;
	}
	if (all_atts.cx) {
		center.x = all_atts.cx->value;
		if (all_atts.cx->type==SVG_NUMBER_PERCENTAGE) center.x /= 100;
	} else {
		center.x = FIX_ONE/2;
	}
	if (all_atts.cy) {
		center.y = all_atts.cy->value;
		if (all_atts.cy->type==SVG_NUMBER_PERCENTAGE) center.y /= 100;
	} else {
		center.y = FIX_ONE/2;
	}

	txh->compositor->rasterizer->stencil_set_gradient_mode(stencil, (GF_GradientMode) all_atts.spreadMethod ? *(SVG_SpreadMethod*)all_atts.spreadMethod : 0);

	if (all_atts.fx) {
		focal.x = all_atts.fx->value;
		if (all_atts.fx->type==SVG_NUMBER_PERCENTAGE) focal.x /= 100;
	} else {
		focal.x = center.x;
	}
	if (all_atts.fy) {
		focal.y = all_atts.fy->value;
		if (all_atts.fy->type==SVG_NUMBER_PERCENTAGE) focal.y /= 100;
	} else {
		focal.y = center.y;
	}

	if (bounds && (!all_atts.gradientUnits || (*(SVG_GradientUnit*)all_atts.gradientUnits==SVG_GRADIENTUNITS_OBJECT)) ) {
		/*move to local coord system - cf SVG spec*/
		gf_mx2d_add_scale(mat, bounds->width, bounds->height);
		gf_mx2d_add_translation(mat, bounds->x, bounds->y  - bounds->height);
	} 
	txh->compositor->rasterizer->stencil_set_radial_gradient(stencil, center.x, center.y, focal.x, focal.y, radius, radius);
}

void compositor_init_svg_radialGradient(GF_Compositor *compositor, GF_Node *node)
{
	SVG_GradientStack *st;
	GF_SAFEALLOC(st, SVG_GradientStack);

	gf_sc_texture_setup(&st->txh, compositor, node);
	st->txh.update_texture_fcnt = SVG_UpdateRadialGradient;
	st->txh.compute_gradient_matrix = SVG_RG_ComputeMatrix;
	gf_node_set_private(node, st);
	gf_node_set_callback_function(node, svg_traverse_gradient);
}


static void svg_traverse_PaintServer(GF_Node *node, void *rs, Bool is_destroy)
{
	SVGPropertiesPointers backup_props;
	SVGAllAttributes all_atts;
	u32 backup_flags;
	u32 styling_size = sizeof(SVGPropertiesPointers);
	SVG_Element *elt = (SVG_Element *)node;
	GF_TraverseState *tr_state = (GF_TraverseState *) rs;

	if (is_destroy) {
		SVG_DestroyPaintServer(node);
		return;
	}

	gf_svg_flatten_attributes(elt, &all_atts);
	compositor_svg_traverse_base(node, &all_atts, tr_state, &backup_props, &backup_flags);
	
	if (tr_state->traversing_mode == TRAVERSE_GET_BOUNDS) {
		return;
	} else {
		compositor_svg_traverse_children(elt->children, tr_state);
	}
	memcpy(tr_state->svg_props, &backup_props, styling_size);
	tr_state->svg_flags = backup_flags;
}
void compositor_init_svg_solidColor(GF_Compositor *compositor, GF_Node *node)
{
	gf_node_set_callback_function(node, svg_traverse_PaintServer);
}

void compositor_init_svg_stop(GF_Compositor *compositor, GF_Node *node)
{
	gf_node_set_callback_function(node, svg_traverse_PaintServer);
}

GF_TextureHandler *compositor_svg_get_gradient_texture(GF_Node *node)
{
	SVG_GradientStack *st = (SVG_GradientStack*) gf_node_get_private((GF_Node *)node);
	return &st->txh;
}


#endif


