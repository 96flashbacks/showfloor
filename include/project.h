/********************************************************************************
	project.h: Project B-Roll include file

	Copyright (c) 2020 Project B-Roll Team

	September 19, 2020
 ********************************************************************************/

#ifndef PROJECT_H
#define	PROJECT_H


/*-------------------------------------------------------------------------------
 *	Switch.
 */

#define	DEBUGSW			1			// debug stuff
#define	DEVELOP			1			// development stuff
#define DEMO            0			// enable Shoshinkai-like demo functionality (WORK IN PROGRESS)
#define LUIGI           0           // enable Luigi, 1 is included along with Mario, while 2 is replace Mario altogether

#define ENGLISH         0			// enable English translation (work in progress, almost finished)


/********************************************************************************/
/*	memory																		*/
/********************************************************************************/

#define	APPL_MEMORY_START	0x80000400
#define	APPL_MEMORY_END		0x801C1000


#define	DEPTHBUF_START		0x80000400
#define	FREEZONE_START		0x8005C000
#define	FREEZONE_END		0x801C1000

#define	DEMOCODE_START		0x8016F000
#define	TEXTBUFF_START		0x801C1000
#define	CPROGBUF_START		0x801CE000
#define	FIFOBUFF_START		0x8022A000		/* 0x80227000	*/
#define	CPROGRAM_START		0x80249000		/* 0x80246000	*/
#define	ULIBCODE_START		0x80378800
#define	CPROGRAM_END		0x8038F800
#define	FRAMEBUF_START		0x8038F800


#endif
