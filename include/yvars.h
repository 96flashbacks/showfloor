#define		DEBUG_STAGE			2
/********************************************************************************
*																				*
*				---------------------------										*
*				    Ultra64 TestProgram											*
*				---------------------------										*
*					File		: yajima.h										*
*					Description	: program switch & parameter					*
*					Date		: 1994.4.5										*
*					Author		: H.yajima										*
*																				*
*********************************************************************************/

/********************************************************************************
		-------------
		 buffer size
		-------------
*********************************************************************************/

#define		BGCHECK_LISTMAX			7000	/* BGcheck LIST SIZE	*/
#define		BGCHECK_DATAMAX			2300	/* BGcheck DATA	SIZE	*/

#define		SIZE_STRATHEAP			4*512	/* Strategy Used Work	*/
			
#define		NUM_STRATEGY			240		/* strategy max count	*/

#define		size_hit_counter		4		/* hitlist pointer		*/
#define		size_path_work			80		/* strategy main work	*/
#define		size_path_stack			8		/* path stack size		*/

/********************************************************************************
		--------------------------
		Strategy Common Parameter
		--------------------------
*********************************************************************************/

#define		MAX_speedY				-78.0f		/* object speed Y parameter */
#define		OBJ_MAXSPEED_Y			MAX_speedY

#define		DEFAULT_ENEMY_HP		0x800;		
#define		DEFAULT_ENEMY_AP		0x100;

/********************************************************************************
		-------------
		 Map System
		-------------
*********************************************************************************/

#define		null_height			-11000
#define		roofnull_height		 20000

#define		map_overflow		0xe000

#define		map_area_size		0x400
#define		map_area_mask		map_area_size-1
#define		map_area_total		16

#define		map_world_size		map_area_size*map_area_total
#define		map_world_offset	map_world_size/2

#define		MAP_AREA_SIZE		 1024
#define		MAP_HALF_SIZE		 MAP_AREA_SIZE*8
#define		MAP_FULL_SIZE		 MAP_AREA_SIZE*16
#define		MAP_LIMIT_MAX		 MAP_AREA_SIZE*8
#define		MAP_LIMIT_MIN		-(MAP_AREA_SIZE*8)+1

/********************************************************************************
		----------------
		 BGCheck Define
		----------------
*********************************************************************************/

#define		BGCHECK_OVERLAPSIZE		50		/* BGcheck AREA overlap size */

#define		BGCHECKFLAG_STATIC		0		/* static BG check		  */
#define		BGCHECKFLAG_DYNAMIC		1		/* move   BG check		  */

#define		BGCHECKCODE_MAX			64		/* bgcheck code max		  */
#define		BGCHECKCODE_POLYGON		BGCHECKCODE_NORMAL

#define		BGNUKI_CODE				18		/* BGcheck areachange code */

#define		OBJSETCODE_TRIPDOOR		100		/* tripdoor special code  */

#define		CK_GROUND				0
#define		CK_ROOF					1

#define		TAGCODE_END				30
#define		WATERLINE_MAX			20


/********************************************************************************
		-----------------
		 Strategy Define
		-----------------
*********************************************************************************/

#define		obj_actor_high	(( (execstp->s[stw_actorcode].d ) >> 24 ) &0xff )

#define		enemydemo_flag	(execstp->s[stw_enemyflag].word[0])
#define		enemydemo_mode	(execstp->s[stw_enemyflag].word[1])

#define		obj_bgpointer		(execstp->s[stw_bgpointer].pointer)
#define		obj_bgcode			(execstp->s[stw_bgparam].word[0])
#define		obj_bgarea			(execstp->s[stw_bgparam].word[1])


#define		obj_havecoin		(execstp->s[stw_havecoin].d)

#define		obj_areamap			(execstp->s[stw_areamap_info].d)

#define		obj_imm				(execstp->s[stw_imm].d)
#define		obj_mainflag		(execstp->s[stw_flag].d)
#define		obj_enemyflag		(execstp->s[stw_enemyflag].d)
#define		obj_my_status		(execstp->s[stw_my_status].d)
#define		obj_hit_status		(execstp->s[stw_hit_status].d)
#define		obj_hit_timer		(execstp->s[stw_hit_timer].d)
#define		obj_worldX			(execstp->s[stw_worldX].f)
#define		obj_worldY			(execstp->s[stw_worldY].f)
#define		obj_worldZ			(execstp->s[stw_worldZ].f)
#define		obj_speedX			(execstp->s[stw_speedX].f)
#define		obj_speedY			(execstp->s[stw_speedY].f)
#define		obj_speedZ			(execstp->s[stw_speedZ].f)
#define		obj_speedF			(execstp->s[stw_speedF].f)
#define		obj_speedL			(execstp->s[stw_speedL].f)
#define		obj_speedU			(execstp->s[stw_speedU].f)
#define		obj_angleX			(execstp->s[stw_angleX].d)
#define		obj_angleY			(execstp->s[stw_angleY].d)
#define		obj_angleZ			(execstp->s[stw_angleZ].d)
#define		obj_animeangleX		(execstp->s[stw_animeangleX].d)
#define		obj_animeangleY		(execstp->s[stw_animeangleY].d)
#define		obj_animeangleZ		(execstp->s[stw_animeangleZ].d)
#define		obj_animepositionY 	(execstp->s[stw_animepositionY].f)
#define		obj_effect			(execstp->s[stw_effect].d)
#define		obj_gravity			(execstp->s[stw_gravity].f)
#define		obj_groundY			(execstp->s[stw_groundY].f)
#define		obj_movestatus		(execstp->s[stw_movestatus].d)
#define		obj_animecounter	(execstp->s[stw_animecounter].d)	

#define		obj_anglespeedX		(execstp->s[stw_anglespeedX].d)
#define		obj_anglespeedY		(execstp->s[stw_anglespeedY].d)
#define		obj_anglespeedZ		(execstp->s[stw_anglespeedZ].d)
#define		obj_skelanime_ptr	(execstp->s[stw_skelanime].pointer)
#define		obj_actionmode		(execstp->s[stw_actionmode].d)
#define		obj_wall_offsetR	(execstp->s[stw_wall_offsetR].f)
#define		obj_air_fric		(execstp->s[stw_air_fric].f)
#define		obj_objname			(execstp->s[stw_objname].d)
#define		obj_mail			(execstp->s[stw_mail].d)
#define		obj_skeletonX		(execstp->s[stw_skeletonX].f)
#define		obj_skeletonY		(execstp->s[stw_skeletonY].f)
#define		obj_skeletonZ		(execstp->s[stw_skeletonZ].f)
#define		obj_programselect	(execstp->s[stw_programselect].d)
#define		obj_breakflag		(execstp->s[stw_breakflag].d)
#define		obj_mode			(execstp->s[stw_mode].d)
#define		obj_process			(execstp->s[stw_process].d)
#define		obj_timer			(execstp->s[stw_timer].d)
#define		obj_bound_e			(execstp->s[stw_bound_e].f)
#define		obj_playerdist  	(execstp->s[stw_playerdist].f)
#define		obj_targetangle		(execstp->s[stw_targetangle].d)
#define		obj_attX			(execstp->s[stw_worldX_attention].f)
#define		obj_attY			(execstp->s[stw_worldY_attention].f)
#define		obj_attZ			(execstp->s[stw_worldZ_attention].f)
#define		obj_friction		(execstp->s[stw_friction].f)
#define		obj_specficG		(execstp->s[stw_specficG].f)
#define		obj_skelanimeNo		(execstp->s[stw_skelanimeNo].d)
#define		obj_alpha			(execstp->s[stw_alpha].d)
#define		obj_ap				(execstp->s[stw_ap].d)
#define		obj_hp				(execstp->s[stw_hp].d)

#define		obj_hmsselectNo		(execstp->s[stw_animecounter].d)
#define		obj_oldmode			(execstp->s[stw_oldmode].d)

#define		obj_enemytype		(execstp->s[stw_enemytype].d)

#define		obj_work0			(execstp->s[stw_work0].d)
#define		obj_work1			(execstp->s[stw_work1].d)
#define		obj_work2			(execstp->s[stw_work2].d)
#define		obj_speed			(execstp->s[stw_speedF].f)

#define		s_clear_status(data)	( execstp->s[stw_obj_status].d ) &= ((data)^0xffffffff)
#define		s_set_objname(data)		( execstp->s[stw_objname].d ) = (OBJNAME_##data)
#define		obj_wallangleY 			(execstp->s[stw_wallangleY].d)

#define		obj_returndist			(execstp->s[stw_returndist].f)
#define		obj_returnangle			(execstp->s[stw_returnangle].d)

#define		obj_erasedist			(execstp->s[stw_erasedist].f)

#define		obj_enemyinfo			(execstp->s[stw_enemyinfo].d)

#define		obj_shapeLOD			(execstp->s[stw_shapeLOD].f)
#define		obj_movebg_checkdist	(execstp->s[stw_movebg_checkdist].f)

#define		obj_soundparam			(execstp->s[stw_soundparam].d)

#define		player_worldX			( player1stp->s[stw_worldX].f)
#define		player_worldY			( player1stp->s[stw_worldY].f)
#define		player_worldZ			( player1stp->s[stw_worldZ].f)


/*------------------------------------------------------------------------------
		road move works 
------------------------------------------------------------------------------*/

#define	road_root(p)		(p->s[stw_work2].pointer)
#define	road_pointer(p)		(p->s[stw_work3].pointer)
#define	road_flag(p)		(p->s[stw_work4].d)
#define	road_angleX(p)		(p->s[stw_work5].d)
#define	road_angleY(p)		(p->s[stw_work6].d)

/*------------------------------------------------------------------------------
		data move works
------------------------------------------------------------------------------*/

#define	datamove_addr		(execstp->s[stw_work8].pointer)
#define	datamove_counter	(execstp->s[stw_work9].d)

/*------------------------------------------------------------------------------
		coinspeed works
------------------------------------------------------------------------------*/

#define		initcoin_speedY		s[stw_work7].f


/********************************************************************************
		enemy define
*********************************************************************************/

/*----- player no work ( mail ) ni watasu !! ---*/

#define		PLAYERMAIL_KOPA_JUMPEND	 	 	1
#define		PLAYERMAIL_KOPA_JUMPEND_FAR	 	PLAYERMAIL_KOPA_JUMPEND
#define		PLAYERMAIL_KOPA_JUMPEND_NEAR	(1<<1)

#define		PLAYERMAIL_CARRYMODE_OFF		(1<<2)
#define		PLAYERMAIL_BOMB_BURN			(1<<3)
#define		PLAYERMAIL_FIRERING				(1<<4)
#define		PLAYERMAIL_CARRYTHROW_BIG		(1<<5)
#define		PLAYERMAIL_CARRYTHROW_SMALL		(1<<6)

#define		PLAYERMAIL_BIRDMODE_OFF			(1<<7)

/*----- enemy  no work ( mail ) ni watasu !! ---*/

#define		EMAIL_PLAYER_PUNCH				1
#define		EMAIL_PLAYER_KICK				2
#define		EMAIL_PLAYER_TRAMP				3
#define		EMAIL_PLAYER_HIPDROP			4
#define		EMAIL_PLAYER_SLIDING			5
#define		EMAIL_PLAYER_HEADATTACK			6

#define		EMAIL_PLAYERMETAL				(1<<9)
#define		EMAIL_PLAYERGETOFF				(1<<10)
#define		EMAIL_CARRYMODE_ON				(1<<11)
#define		EMAIL_PLAYEREVEN				(1<<12)
#define		EMAIL_PLAYERDAMAGE				(1<<13)
#define		EMAIL_PLAYERATTACK				(1<<14)
#define		EMAIL_PLAYERHIT					(1<<15)


#define		EMAIL_DOOROPEN_1				(1<<16)
#define		EMAIL_DOOROPEN_2				(1<<17)
#define		EMAIL_DOORCLOSE_1				(1<<18)
#define		EMAIL_DOORCLOSE_2				(1<<19)
#define		EMAIL_RIDEON					(1<<20)
#define		EMAIL_KOPADOWN					(1<<21)
#define		EMAIL_NOKOBOARD_OFF				(1<<22)

#define		EMAIL_BOMB						(1<<23)


#define		EMAIL_PLAYERHITALL				0xffffffff

/****************************************************************/
/*																*/
/*	block item code												*/
/*																*/
/****************************************************************/

#define		TIMESCALE_X				1
#define		TIMESCALE_Y			(1<<1)
#define		TIMESCALE_Z			(1<<2)

#define		DIST_FAR			19000.0f

#define		ITEMHAT_TIME	   (30*10)		/* itemhat remove time  */

#define		HANSWITCH_TIMER	   	400			/* timer block time 	*/

#define		EFFECT_NORMAL	0				/* effect labels		*/	
#define		EFFECT_POLYGON	1
#define		EFFECT_BURN		2
#define		EFFECT_MOWAN	3

/********************************************************************************
		--------------
		 Debug Switch
		--------------
*********************************************************************************/


#define		DebugSW				1		/* 0/1 game/debug			*/  
#define		BRKprogram			0		/* 0/1 game/break program	*/
										/* mcInitBGCheck  */
										/* InitStrategy   */
										/* SetStrategy    */
										/* ExecStrategy   */
										/* RemoveStrategy */

#define		enemy_p0				(yd_work[DBMODE_ENEMYEDIT][0])
#define		enemy_p1				(yd_work[DBMODE_ENEMYEDIT][1])
#define		enemy_p2				(yd_work[DBMODE_ENEMYEDIT][2])
#define		enemy_p3				(yd_work[DBMODE_ENEMYEDIT][3])
#define		enemy_p4				(yd_work[DBMODE_ENEMYEDIT][4])
#define		enemy_p5				(yd_work[DBMODE_ENEMYEDIT][5])
#define		enemy_p6				(yd_work[DBMODE_ENEMYEDIT][6])
#define		enemy_p7				(yd_work[DBMODE_ENEMYEDIT][7])
#define		effect_p0				(yd_work[DBMODE_EFFECTEDIT][0])
#define 	effect_p1				(yd_work[DBMODE_EFFECTEDIT][1])
#define		effect_p2				(yd_work[DBMODE_EFFECTEDIT][2])
#define		effect_p3 				(yd_work[DBMODE_EFFECTEDIT][3])
#define		effect_p4				(yd_work[DBMODE_EFFECTEDIT][4])
#define		effect_p5				(yd_work[DBMODE_EFFECTEDIT][5])
#define		effect_p6				(yd_work[DBMODE_EFFECTEDIT][6])
#define		effect_p7				(yd_work[DBMODE_EFFECTEDIT][7])

#define		YAJIMA_LAND			32

#define		DISP_CENTER_X		160		/* debug screen center */
#define		DISP_CENTER_Y		120		/* debug screen center */

#define		STRATEGY_ENEMYMOVE_OFF	1

/*===============================================================================
		End End 
================================================================================*/

