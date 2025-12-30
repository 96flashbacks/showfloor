/********************************************************************************
						Ultra 64 MARIO Brothers

						stage 32 sequence module

			Copyright 1995 Nintendo co., ltd.  All rights reserved

							December 8, 1995
 ********************************************************************************/

/*
#define	ASSEMBLER

#ifdef yajima
#include "headers.h"
#else
#include "../../headers.h"
#endif

		.data
		.align	2
		.align	0

		.globl	SEQ_DoStage32
*/
#include <ultra64.h>
#include "sm64.h"
#include "behavior_data.h"
#include "model_ids.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "segment_symbols.h"
#include "level_commands.h"
//#include "actors/group8.h"

#include "game/level_update.h"

#include "levels/scripts.h"

#include "actors/common1.h"

#include "make_const_nonconst.h"
#include "levels/yajima_test/header.h"

#define S_NULL MODEL_NONE
#define S_ping MODEL_PENGUIN
#define S_hanbutton MODEL_PURPLE_SWITCH
#define S_hibiblock MODEL_BREAKABLE_BOX
#define S_pushblock MODEL_METAL_BOX
#define S_BCbutton MODEL_BLUE_COIN_SWITCH
#define S_bluecoin MODEL_BLUE_COIN
#define S_nokonoko MODEL_KOOPA_WITH_SHELL
#define S_monky MODEL_UKIKI
#define S_carryboy MODEL_CHUCKYA
#define S_bikkuri_button MODEL_CAP_SWITCH
#define S_itemhat_metalwing MODEL_MARIOS_WINGED_METAL_CAP
#define S_itemhat_metal MODEL_MARIOS_METAL_CAP
#define S_itemhat_wing MODEL_MARIOS_WING_CAP
#define S_itemhat_hat MODEL_MARIOS_CAP
#define S_itembox MODEL_EXCLAMATION_BOX

#define e_wind_ping bhvSLWalkingPenguin
#define e_wind bhvSLSnowmanWind
#define e_timeblock_switch bhvFloorSwitchHiddenObjects
#define e_timeblock bhvHiddenObject
#define e_switchcoin bhvBlueCoinSwitch
#define e_bluecoin bhvHiddenBlueCoin
#define e_pushblock bhvPushableMetalBox
#define e_hibiblock bhvBreakableBox
#define e_noko bhvKoopa
#define e_monky bhvUkiki
#define e_manycoin bhvCoinFormation
#define e_carryboy bhvChuckya
#define e_bikkuri_switch bhvCapSwitch
//#define e_hat_metal_wing bhvWingMetalCap
#define e_hat_metal bhvMetalCap
#define e_hat_wing bhvWingCap
#define e_hat bhvNormalCap
#define e_itembox bhvExclamationBox
#define e_onoff_switch bhvFloorSwitchHardcodedModel
#define e_hanswitch bhvFloorSwitchHardcodedModel
#define e_osublock bhvJumpingBox
#define e_horiage bhvHeaveHo
#define e_testball bhvFreeBowlingBall//it's unknown what testball actually was.
#define seqActor(model,posX,posY,posZ,angX,angY,angZ,paramA,paramB,paramC,behavior) OBJECT(model,posX,posY,posZ,angX,angY,angZ,((0x00) | ((paramB)<<16) | (00) | (00)),behavior),

//some of these objects can't be loaded all together because they're in different groups.
//this may be evidence that most/all objects were once in a single group, or could be loaded at any time.
//the shoshinkai demo levels also shared some textures, so this may have applied to texture banks as well.

/*------- yajima special stage -------*/

const LevelScript level_yajima_test_entry[] = {

    INIT_LEVEL(),
    LOAD_MIO0(        /*seg*/ 0x07, _yajima_test_segment_7SegmentRomStart, _yajima_test_segment_7SegmentRomEnd),
    LOAD_MIO0(        /*seg*/ 0x05, _group7_mio0SegmentRomStart, _group7_mio0SegmentRomEnd),
    LOAD_RAW(         /*seg*/ 0x0C, _group7_geoSegmentRomStart,  _group7_geoSegmentRomEnd),
    //LOAD_MIO0(        /*seg*/ 0x06, _group6_mio0SegmentRomStart, _group6_mio0SegmentRomEnd),
    //LOAD_RAW(         /*seg*/ 0x0D, _group6_geoSegmentRomStart,  _group6_geoSegmentRomEnd),
    LOAD_MIO0(        /*seg*/ 0x08, _common0_mio0SegmentRomStart, _common0_mio0SegmentRomEnd),
    LOAD_RAW(         /*seg*/ 0x0F, _common0_geoSegmentRomStart,  _common0_geoSegmentRomEnd),
    //LOAD_MIO0         /*seg*/ 0x09, _group8_mio0SegmentRomStart, _group8_mio0SegmentRomEnd),
    //LOAD_RAW(         /*seg*/ 0x0A, _group8_geoSegmentRomStart,  _group8_geoSegmentRomEnd),

    ALLOC_LEVEL_POOL(),

    MARIO(/*model*/ MODEL_MARIO, /*behParam*/ 0x00000001, /*beh*/ bhvMario),
    JUMP_LINK(script_func_global_1),
    JUMP_LINK(script_func_global_8),
//    JUMP_LINK(script_func_global_9),

    AREA(/*index*/ 1, RCP_Stage32Scene1),
		//seqActor(S_pushblock,	1242, 200, 1959,   0,0,0,  0,0,0, e_pushblock	)

#if 0																				  


			 seqActor(S_carryboy		,	-1000, 0,  0,   0,0,0,  0,0,0, e_carryboy	) 
			 seqActor(S_carryboy		,	-1000, 0,  0,   0,0,0,  0,0,0, e_carryboy	) 


			seqActor(S_bikkuri_button	,	-1500,   0,   1000,   0,0,0,  0,0,0, e_bikkuri_switch	)
			seqActor(S_bikkuri_button	,	 -500,   0,  -1000,   0,0,0,  0,1,0, e_bikkuri_switch	)
			seqActor(S_bikkuri_button	,	  500,   0,  -1000,   0,0,0,  0,2,0, e_bikkuri_switch	)

			seqActor(S_itembox			,    -400,  250,  1500,   0,0,0,  0,0, 0, e_itembox 	 	)
			seqActor(S_itembox			,    -800,  250,  1500,   0,0,0,  0,1, 0, e_itembox 	 	)
			seqActor(S_itembox			,   -1200,  250,  1500,   0,0,0,  0,2, 0, e_itembox 	 	)

			 seqActor(S_hibiblock		,	-2867, 331,   -895,   0,0,0,  0,0,0, e_testball	) 

			seqActor(S_hanbutton,    0, 0, -900,   0,0,0,  0,0,0,	e_onoff_switch )

			seqActor(S_hanbutton,    0, 0, -300,   0,0,0,  0,0,0,	e_timeblock_switch )
			seqActor(S_hibiblock, -500, 0, -700,   0,0,0,  0,0,0,	e_timeblock )
			seqActor(S_hibiblock,    0, 0, -700,   0,0,0,  0,1,0,	e_timeblock )
			seqActor(S_hibiblock,  500, 0, -700,   0,0,0,  0,2,0,	e_timeblock )

			//seqActor(S_pushblock, 1000, 0, -1000,   0,0,0,  0,0,0,	e_pushblock )

			seqActor(S_NULL,	  -3000, 0, 2000,   0,0,0,  0,0,0,	e_manycoin  )
			seqActor(S_NULL,	  -2000, 0, 2000,   0,0,0,  0,1,0,	e_manycoin  )
			seqActor(S_NULL,	  -1000, 0, 2000,   0,0,0,  0,2,0,	e_manycoin  )
			seqActor(S_NULL,	      0, 0, 2000,   0,0,0,  0,3,0,	e_manycoin  )
			seqActor(S_NULL,	    500, 0, 2000,   0,0,0,  0,4,0,	e_manycoin  )

			seqActor(S_NULL,	  -3000, 200, 1000,   0,0,0,  0,0x10,0,	e_manycoin  )
			seqActor(S_NULL,	  -2000, 200, 1000,   0,0,0,  0,0x11,0,	e_manycoin  )
			seqActor(S_NULL,	  -1000, 200, 1000,   0,0,0,  0,0x12,0,	e_manycoin  )
			seqActor(S_NULL,	      0, 200, 1000,   0,0,0,  0,0x13,0,	e_manycoin  )
			seqActor(S_NULL,	    500, 200, 1000,   0,0,0,  0,0x14,0,	e_manycoin  )

			seqActor(S_NULL , 	0, 100,-500,   0,0,0,   0,0,0, e_wind	) 
			seqActor(S_ping ,-900,   0, 500,   0,90,0,  0,0,0, e_wind_ping ) 

			seqActor(S_itembox			,     000,  250,  1500,   0,0,0,  0,7, 0, e_itembox 	 	) 
			seqActor(S_itembox			,    -400,  250,  1500,   0,0,0,  0,0, 0, e_itembox 	 	)
			seqActor(S_itembox			,    -800,  250,  1500,   0,0,0,  0,1, 0, e_itembox 	 	)
			seqActor(S_itembox			,   -1200,  250,  1500,   0,0,0,  0,2, 0, e_itembox 	 	)
			seqActor(S_itembox			,   -1600,  250,  1500,   0,0,0,  0,3, 0, e_itembox 	 	)	
			seqActor(S_itembox			,   -2000,  250,  1500,   0,0,0,  0,4, 0, e_itembox  	 	)
			seqActor(S_itembox			,   -2400,  250,  1500,   0,0,0,  0,5, 0, e_itembox  	 	)
			seqActor(S_itembox			,   -2800,  250,  1500,   0,0,0,  0,6, 0, e_itembox  	 	)
			seqActor(S_itembox			,   -3200,  250,  1500,   0,0,0,  0,8, 0, e_itembox  	 	)
			seqActor(S_bikkuri_button	,	-1500,   0,  -1000,   0,0,0,  0,0,0, e_bikkuri_switch	)
			seqActor(S_bikkuri_button	,	 -500,   0,  -1000,   0,0,0,  0,1,0, e_bikkuri_switch	)
			seqActor(S_bikkuri_button	,	  500,   0,  -1000,   0,0,0,  0,2,0, e_bikkuri_switch	)

			seqActor(S_NULL				,	  200,    0,      0,  0,0,0,  0,1,0, e_hanswitch 	 )
			seqActor(S_NULL				,	  600,    0,      0,  0,0,0,  0,0,0, e_hanswitch 	 )

			seqActor(S_monky			,      0,   200,     0,   0,0,0,  0,0,0, e_monky			)
			seqActor(S_horiage			,   2000,   200,  2000,   0,0,0,  0,0,0, e_horiage			)
			seqActor(S_motosman			,	    0,    0, -1500,   0,0,0,  0,0,0, e_motos			)
			seqActor(S_hibiblock		,  	 1000,    0,     0,   0,0,0,  0,0,0, e_osublock			)
			seqActor(S_itemhat_metalwing,    -200,  200,  2500,   0,0,0,  0,0,0, e_hat_metal_wing	)
			seqActor(S_itemhat_metal 	,    -600,  200,  2500,   0,0,0,  0,0,0, e_hat_metal 		)
			seqActor(S_itemhat_wing  	,    -400,  200,  2500,   0,0,0,  0,0,0, e_hat_wing 		)
			seqActor(S_itemhat_hat 		,    -800,  200,  2500,   0,0,0,  0,0,0, e_hat				)
			seqActor(S_hibiblock		,       0,  200,  3000,   0,0,0,  0,0,0, e_hibiblock		)
			seqActor(S_hibiblock		,     500,    0,  3000,   0,0,0,  0,0,0, e_hibiblock		)

			seqActor(S_NULL 			,    -400,  300,   500,   0,0,0,  0,0,0, e_qbox				)
			seqActor(S_NULL 			,    -800,  300,   500,   0,0,0,  0,1,0, e_qbox				)
			seqActor(S_NULL 			,   -1200,  300,   500,   0,0,0,  0,2,0, e_qbox 			)
			seqActor(S_NULL 			,   -1400,  300,   500,   0,0,0,  0,3,0, e_qbox 			)

			seqActor(S_nokonoko 		,       0,    0,  -500,   0,0,0,  0,1,0, e_noko				)

			seqActor(S_bikkuri_button	,	-1500,    0,     0,   0,0,0,  0,0,0, e_bikkuri_switch	)
			seqActor(S_bikkuri_button	,	 -500,    0,     0,   0,0,0,  0,1,0, e_bikkuri_switch	)
			seqActor(S_bikkuri_button	,	  500,    0,     0,   0,0,0,  0,2,0, e_bikkuri_switch	)
			seqActor(S_bikkuri_button	,	 1500,    0,     0,   0,0,0,  0,3,0, e_bikkuri_switch	)

			seqActor(S_hibiblock		,       0,    0,  -500,   0,0,0,  0,0,0, e_hibiblock		)

			seqActor(S_BCbutton			,	    0,    0,   500,   0,0,0,  0,0,0, e_switchcoin		)
			seqActor(S_bluecoin			,	    0,    0,   200,   0,0,0,  0,0,0, e_bluecoin			)
			seqActor(S_bluecoin			,	  200,    0,   200,   0,0,0,  0,0,0, e_bluecoin			)
			seqActor(S_bluecoin			,	  400,    0,   200,   0,0,0,  0,0,0, e_bluecoin			)
			seqActor(S_bluecoin			,	 -200,    0,   200,   0,0,0,  0,0,0, e_bluecoin			)
			seqActor(S_bluecoin			,	 -400,    0,   200,   0,0,0,  0,0,0, e_bluecoin			)



#endif
	


        TERRAIN(/*terrainData*/ testmap1_info),
        MACRO_OBJECTS(/*objList*/ yajima_test_seg7_macro_objs),
        SET_BACKGROUND_MUSIC(/*settingsPreset*/ 0x0000, /*seq*/ SEQ_SOUND_PLAYER),
        TERRAIN_TYPE(/*terrainType*/ TERRAIN_GRASS),

    END_AREA(),
    FREE_LEVEL_POOL(),


    MARIO_POS(/*area*/ 1, /*yaw*/ 0, /*pos*/ 0, 0, 0),

    CALL(/*arg*/ 0, /*func*/ lvl_init_or_update),
    CALL_LOOP(/*arg*/ 1, /*func*/ lvl_init_or_update),
    CLEAR_LEVEL(),
    SLEEP_BEFORE_EXIT(/*frames*/ 1),
    EXIT(),
};

//#include "stage.dat"

/*------------------------------------*/


