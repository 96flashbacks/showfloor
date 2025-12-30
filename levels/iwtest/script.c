/********************************************************************************
						Ultra 64 MARIO Brothers

						stage 37 sequence module

			Copyright 1995 Nintendo co., ltd.  All rights reserved

							  1996.1.8
 ********************************************************************************/

#include <ultra64.h>
#include "sm64.h"
#include "behavior_data.h"
#include "model_ids.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "segment_symbols.h"
#include "level_commands.h"

#include "game/level_update.h"

#include "levels/scripts.h"

#include "actors/common1.h"

#include "make_const_nonconst.h"
#include "levels/iwtest/header.h"

/********************************************************************************
		stage 37 Seq
 ********************************************************************************/

const LevelScript level_iwtest_entry[] = {
    INIT_LEVEL(),
    LOAD_MIO0(         /*seg*/ 0x07, _iwtest_segment_7SegmentRomStart, _iwtest_segment_7SegmentRomEnd),


    ALLOC_LEVEL_POOL(),
    MARIO(/*model*/ MODEL_MARIO, /*behParam*/ 0x00000001, /*beh*/ bhvMario),

		/*------ test enemy's -----------------------------------------------*/
//		seqHmsShape(S_nokonoko		 , RCP_HmsEnemynokonoko	 		)
//		seqHmsShape(S_nokonoko_nude	 , RCP_HmsEnemynokonoko_nude		)
//		seqHmsShape(S_koura			 , RCP_HmsGreenNoko 			)
//		seqHmsShape(S_sanbo_head	 , RCP_HmsSanboHead 			)
//		seqHmsShape(S_sanbo_body	 , RCP_HmsSanboBody		 	)
//		seqHmsShape(S_bat			 , RCP_HmsEnemybat	 		)
//		seqHmsShape(S_heyho		 	 , RCP_HmsEnemyheyho_fly		)
//		seqHmsShape(S_kuribo		 , RCP_HmsEnemykuribo	 		)
//		seqHmsShape(S_wanwan		 , RCP_HmsEnemywanwan	 		)
//		seqHmsShape(S_hanachan_head	 , RCP_HmsEnemyhanahead	 		)
//		seqHmsShape(S_hanachan_body	 , RCP_HmsEnemyhanabody	 		)
//		seqHmsShape(S_jugem			 , RCP_HmsEnemyjygem	 		)
//		seqGfxShape(S_NULL			 , gfx_cloud, RM_XSURF 			)
//		seqHmsShape(S_indy			 , RCP_HmsEnemyindy	 		)
//		seqGfxShape(S_poohole		 , RCP_hole, RM_XDECAL	 		)
//		seqGfxShape(S_stone			 , RCP_stone, RM_SPRITE	 		)
//		seqGfxShape(S_lift0			 , gfx_testlift, RM_SURF 		)
//		seqHmsShape(S_togezo		 , RCP_HmsEnemytogezo	 		)
//		seqHmsShape(S_pipo			 , RCP_HmsEnemypipo	 		)
//		seqHmsShape(S_wan_ball		 , RCP_wan_ball		 		)
		/*-------------------------------------------------------------------*/

    AREA(/*index*/ 1, RCP_Stage37Scene1),

			/*------ test enemy's -----------------------------------------------*/
//			seqActor( S_koura		,    1000, 0,0	, 0,0,0, 0,0,0, 	e_kame2		 )
//			seqActor( S_NULL		,    3500, 0,   -2000, 0,0,0, 0,0,0, 	e_sanbo		 )
//			seqActor( S_bat			,    1500, 500,0, 0,0,0, 0,1,0, 	e_bat		 )
//			seqActor( S_bat			,    1500, 500,500, 0,0,0, 0,0,0, 	e_bat		 )
//			seqActor( S_bat			,    2000, 500,0, 0,0,0, 0,0,0, 	e_bat		 )
//			seqActor( S_bat			,    2000, 500,500, 0,0,0, 0,0,0, 	e_bat		 )
//			seqActor( S_bat			,    -1500, 500,0, 0,0,0, 0,0,0, 	e_bat		 )
//			seqActor( S_bat			,    -1500, 500,500, 0,0,0, 0,0,0, 	e_bat		 )
//			seqActor( S_bat			,    -2000, 500,0, 0,0,0, 0,0,0, 	e_bat		 )
//			seqActor( S_bat			,    -2000, 500,500, 0,0,0, 0,0,0, 	e_bat		 )
//			seqActor( S_heyho		,    2000, 300,0, 0,0,0, 0,0,0, 	e_heyho		 )
//			seqActor( S_wanwan		,   -2500, 0,-1500, 0,0,0, 0,0,0, 	e_wanwan	 )
//			seqActor( S_wanwan		,    0,    0,-2000, 0,0,0, 0,0,0, 	e_wanwan	 )
//			seqActor( S_wanwan		,   -2500, 0,1000, 0,0,0, 0,0,0, 	e_wanwan	 )
//			seqActor( S_jugem		,    4000, 100,4000, 0,0,0, 0,0,0, 	e_jugem		 )
//			seqActor( S_indy		,    0, 0,  4000, 0,0,0, 0,0,0, 	e_indy		 )
//			seqActor( S_indy		,    0, 0,  4200, 0,0,0, 0,1,0, 	e_indy		 )
//			seqActor( S_poohole		,    0, 0,  3800, 0,0,0, 0,0,0, 	e_hole		 )
//			seqActor( S_poohole		,    200, 0,  4000, 0,0,0, 0,0,0, 	e_hole		 )
//			seqActor( S_poohole		,    0, 0,  4200, 0,0,0, 0,0,0, 	e_hole		 )
//			seqActor( S_poohole		,    -200, 0,  4000, 0,0,0, 0,0,0, 	e_hole		 )
//			seqActor( S_lift0		,    3000, 300, -1000, 0,0,0, 0,0,0, 	e_linelift	 )
//			seqActor( S_lift0		,    -4000, 300, 4000, 0,0,0, 0,0,0, 	e_seesaw	 )
//			seqActor( S_nokonoko		,    3500, 0, 200, 0,0,0, 0,1,0, 	e_noko		 )
//			seqActor( S_nokonoko_nude	,    3500, 0, 200, 0,0,0, 0,0,0, 	e_noko		 )
//			seqActor( S_kuribo		,    3500, 0, 200, 0,0,0, 0,0,0, 	e_kuribo	 )
//			seqActor( S_hanachan_head	,    3500, 0, 200, 0,0,0, 0,0,0, 	e_hanachan	 )
//			seqActor( S_NULL		,    -3000, 700, 4000, 0,0,0, 0,0,0, 	e_pedallift	 )
			/*-------------------------------------------------------------------*/

        TERRAIN(/*terrainData*/ map37_info),
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

/*===============================================================================
		End
===============================================================================*/


