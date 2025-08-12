/********************************************************************************
						Ultra 64 MARIO Brothers

					   Header file of "areamap.c".

			Copyright 1995 Nintendo co., ltd.  All rights reserved

				  This module was programmed by Y.Tanimoto.

							Sept 11, 1995
*********************************************************************************/

#ifndef	_AREAMAP_PROT
#define	_AREAMAP_PROT

Gfx *AreaMap(s32 code, struct GraphNode *node, void *data);

typedef struct {
	/*u16*/u8*		image_upper	;
	/*u16*/u8*		image_lower	;
	f32		xo_g		;
	f32		zo_g		;
	f32		side_g		;
} AreaMapData ;

#endif
