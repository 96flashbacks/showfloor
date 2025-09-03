#ifndef _MBI_H_
#define	_MBI_H_

/**************************************************************************
 *									  *
 *		 Copyright (C) 1994, Silicon Graphics, Inc.		  *
 *									  *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright law.  They  may  not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *									  *
 **************************************************************************/

/*
 * Header file for the Media Binary Interface
 *
 * NOTE: This file is included by the RSP microcode, so any C-specific
 * constructs must be bracketed by #ifdef _LANGUAGE_C
 *
 */

#define	_SHIFT(v, s)	((unsigned int)(v) << (s))
#define	_SHIFTR(v, s)	((unsigned int)(v) >> (s))

#define G_ON	(1)
#define G_OFF	(0)

/**************************************************************************
 *
 * Graphics Binary Interface
 *
 **************************************************************************/

#include <PR/gbi.h>

/**************************************************************************
 *
 * Audio Binary Interface
 *
 **************************************************************************/

#include <PR/abi.h>

/**************************************************************************
 *
 * Video Binary Interface
 *
 **************************************************************************/

#ifdef _LANGUAGE_C
#endif /* _LANGUAGE_C */

/**************************************************************************
 *
 * Task list
 *
 **************************************************************************/

#define	M_GFXTASK	1
#define	M_AUDTASK	2
#define	M_VIDTASK	3

#ifdef _LANGUAGE_C


#endif /* _LANGUAGE_C */

/**************************************************************************
 *
 * Segment macros and definitions
 *
 **************************************************************************/

#define	NUM_SEGMENTS		(16)
#define	SEGMENT_OFFSET(a)	((unsigned int)(a) & 0x00ffffff)
#define	SEGMENT_NUMBER(a)	((unsigned int)(a) >> 24)
#define	SEGMENT_ADDR(num, off)	(((num) << 24) + (off))

#ifndef NULL
#define NULL 0
#endif

#ifdef _LANGUAGE_C
#endif /* _LANGUAGE_C */

#endif /* !_MBI_H_ */
