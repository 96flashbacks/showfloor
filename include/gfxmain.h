/********************************************************************************
	gfxmain.h: Ultra 64 MARIO Brothers include file

	Copyright (c) 1996 Nintendo co., ltd.  All rights reserved

	March 29, 1996
 ********************************************************************************/

#ifndef GFXMAIN_H
#define	GFXMAIN_H

#define	GRAPH_TASK		0
#define	AUDIO_TASK		1

#define	SCREEN_WIDTH		320
#define	SCREEN_HEIGHT		240
#define	SCREEN_ASPECT		(320.0f/240.0f)
#define	SCREEN_NEAR			100.0f
#define	SCREEN_FAR			12800.0f
#define	SCREEN_ANGLE		30.0f

#define	CLIP_UPPER			8
#define	CLIP_LOWER			(SCREEN_HEIGHT-8)


#define	LIGHT_X				40
#define	LIGHT_Y				40
#define	LIGHT_Z				40

#define	SC_CLIENT_AUDIO		1
#define	SC_CLIENT_GRAPHIC	2


#define	GAME_DEMO			0
#define	GAME_ONE_PLAYER		1
#define	GAME_TWO_PLAYER		2

#define	MARIO_ID			0
#define	LUIGI_ID			1


typedef void (*RDPDoneProc)(void);


/********************************************************************************/
/*																				*/
/*	Controller button assignment												*/
/*																				*/
/********************************************************************************/

#define	CONT_XA		CONT_A
#define	CONT_XB		CONT_B
#define	CONT_XL		CONT_C
#define	CONT_XD		CONT_D
#define	CONT_XU		CONT_E
#define	CONT_XR		CONT_F
#define	CONT_BACK	CONT_G
#define	CONT_EXIT	0x0080

#define AUTO_A		0x80
#define AUTO_B		0x40
#define AUTO_Z		0x20
#define	AUTO_END	0x10
#define AUTO_CU		0x08
#define AUTO_CD		0x04
#define AUTO_CL		0x02
#define AUTO_CR		0x01

#endif
