/********************************************************************************
					-------------------------
				 	 Ultra 64 MARIO Brothers
   					-------------------------

					File		: ymacs.h
					Description	: yajima's macro
					Date		: 1995.4.10
					Author		: H.yajima

 ********************************************************************************/

/********************************************************************************
			Shape (Light)Color Set Macro
 ********************************************************************************/

#define	C_SCALE	4
#define ShapeColor(R,G,B) gdSPDefLights1(R/C_SCALE ,G/C_SCALE,B/C_SCALE,R,G,B,LIGHT_X,LIGHT_Y,LIGHT_Z)//{{R/C_SCALE ,G/C_SCALE ,B/C_SCALE ,0 ,R/C_SCALE ,G/C_SCALE ,B/C_SCALE ,0 },{ R,G,B,0, R,G,B,0, LIGHT_X, LIGHT_Y, LIGHT_Z, 0} }

/********************************************************************************
			make skeleton object lang
 ********************************************************************************/

#define	Mabs(data)		(data) >=0 ? (data) : -(data)
#define	Mdeg(ang)		((ang)*182)

/********************************************************************************
			error macro
 ********************************************************************************/

#define		rmondebug(n,f)		if (n==1)	rmonpf	(f);
#define		ybk(n)				rmonpf(("n\n"));

/********************************************************************************
			global memory macros
 ********************************************************************************/

#ifdef	Mglobal_value_define
#define	Mglobal	
#define	Mglobal_val(v) = (v)
#else
#define	Mglobal extern
#define	Mglobal_val(v)
#endif

/********************************************************************************
			bitclear macros 
 ********************************************************************************/

#define	Mbitclr(work,bit)		(work) = (work) & ((bit)^-1)
#define	Mbitset(work,bit)		(work) = (work) | (bit)

/********************************************************************************
			strategy macros
 ********************************************************************************/

#define	s_setfloat(stratp,index,data)	(stratp)->s[(stw_##index)].f  = (data)
#define	s_setd(stratp,index,data)		(stratp)->s[(stw_##index)].d  = (data)

#define	s_timerINC(stratp)				if (((stratp)->s[stw_work7].d) >= 0 )	(stratp->s[stw_work7].d)++; \
										else  (stratp->s[stw_work7].d) = 0x7fffffff

#define	s_timerRESET(stratp)			((stratp)->s[stw_work7].d)   = 0

#define	STangleY						s[stw_angleY].d
#define	STactionmode					s[stw_actionmode].d
#define	STtimer							s[stw_work7].d

//enum {
// 	DBMODE_YAJIMA,
//	DBMODE_MAPINFO,
//	DBMODE_PLAYERINFO,
//	DBMODE_STAGEINFO,
//	DBMODE_EFFECTEDIT,
//	DBMODE_ENEMYEDIT,
//	DBMODE_COUNTMAX
//};

/********************************************************************************
		position & angle 
*********************************************************************************/

#define		_x			0
#define		_y			1
#define		_z			2

/********************************************************************************
		math
*********************************************************************************/

#define		defscale			0.25

#define		pi					3.141592
#define		rad2pi				2*pi
#define 	deg2rad(s)			(s)*pi/180.0

/*==============================================================================*/
/*				End End End End End End											*/
/*==============================================================================*/



