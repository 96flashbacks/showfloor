/*
 * universal leak-importing header
 */

#if !defined(HEADERS_H)
#define HEADERS_H

/*
 * Gex is a 1995 platform game developed by Crystal Dyna-
 *
 * in all seriousness, this is the only original-source header I'll be copying over
 */
#include "gex.h"

/*
 * shut multiple infuriatingly annoying warnings up
 */
#pragma GCC diagnostic ignored "-Wmissing-braces"				/* missing Vtx braces */
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"	/* missing AnimeRecord variable */
#pragma GCC diagnostic ignored "-Woverflow"						/* shut CCM the hell up */

/*
 * from here on out, I'll be defining a bunch of original-source macros, types
 * and other things for the sake of readability, basically turning this single
 * file (with GEX.H) into a near-automatic leak importer
 */

/*
 * build switches
 */
#define	DEBUGSW		0
// #define	DEVELOP		0
#define	VERSION_E3	0

/*
 * light values
 */
#define LIGHT_X 40
#define LIGHT_Y 40
#define LIGHT_Z 40

/*
 * generic ShapeColor defintion
 */
#define	C_SCALE	4
#define ShapeColor(R,G,B) gdSPDefLights1(R/C_SCALE, G/C_SCALE, B/C_SCALE, R, G, B, LIGHT_X, LIGHT_Y, LIGHT_Z)

/*
 * 'Hierarchy' type defintion
 */
typedef u32 Hierarchy;

/*
 * render modes
 */
#define	RM_BACK		0
#define	RM_SURF		1
#define	RM_DECAL	2
#define	RM_INTER	3
#define	RM_SPRITE	4
#define	RM_XSURF	5
#define	RM_XDECAL	6
#define	RM_XINTER	7
#define	RM_SOFT_SPRITE	RM_SPRITE

/*
 * map functions
 */
#define CtrlMarioAlpha geo_mirror_mario_set_alpha
#define CtrlMarioLOD geo_switch_mario_stand_run
#define CtrlMarioEye geo_switch_mario_eyes
#define CtrlMarioWaist geo_mario_tilt_torso
#define CtrlMarioHead geo_mario_head_rotation
#define CtrlMarioHand geo_switch_mario_hand
#define CtrlHandScale geo_mario_hand_foot_scaler
#define CtrlMarioRenderMode geo_switch_mario_cap_effect
#define CtrlMarioSkin geo_switch_mario_cap_effect
#define CtrlMarioCap geo_switch_mario_cap_on_off
#define CtrlMarioWing geo_mario_rotate_wing_cap_wings
#define CtrlMarioTaking geo_switch_mario_hand_grab_pos
#define CtrlMirrorMario geo_render_mirror_mario
#define CtrlMarioGeoMode geo_mirror_mario_backface_culling
#define MarioProc1 geo_move_mario_part_from_parent
#define AlphaControl geo_update_layer_transparency
#define ControlShapeAnime geo_switch_anim_state
#define CtrlPingEye geo_switch_tuxie_mother_eyes
#define KopaProc1 geo_update_body_rot_from_parent

/*
 * a load of hierarchy map macros
 */
#define hmsBegin()							GEO_OPEN_NODE(),
#define hmsEnd()							GEO_CLOSE_NODE(),
#define hmsSelect(code, proc)				GEO_SWITCH_CASE(code, proc),
#define hmsGfx(x, y)						GEO_DISPLAY_LIST(x, y),
#define hmsReturn()							GEO_RETURN(),
#define hmsJoint(mode, gfx, px, py, pz)		GEO_ANIMATED_PART(mode, px, py, pz, gfx),
#define hmsShadow(size, level, flag)		GEO_SHADOW(flag, level, size),
#define hmsScale(f)							GEO_SCALE(0, ((unsigned long)(f*65536.0f))),
#define hmsCall(r)							GEO_BRANCH(1, r),
#define hmsExit()							GEO_END(),
#define hmsLOD(near, far)					GEO_RENDER_RANGE(near, far),
#define hmsCProg(code, proc)				GEO_ASM(code, proc),
#define hmsRotate(ax, ay, az)				GEO_ROTATION_NODE(0, ax, ay, az),
#define hmsGroup()							GEO_NODE_START(),
#define hmsSucker(x, y, z, w, dl)			GEO_HELD_OBJECT(x, y, z, w, dl),
#define hmsHeader(x)						GEO_CULLING_RADIUS(x),
#define hmsBboard(px, py, pz)				GEO_BILLBOARD_WITH_PARAMS(0, px, py, pz),
#define hmsCamera(a, b, c, d, e, f, g, h)	GEO_CAMERA(a, b, c, d, e, f, g, h),
#define hmsPerspective(a, b, c, d)			GEO_CAMERA_FRUSTUM_WITH_FUNC(a, b, c, d),
#define hmsScene(a, b, c, d, e)				GEO_NODE_SCREEN_AREA(10, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2),
#define hmsLayer(a)							GEO_ZBUFFER(a),
#define hmsOrtho(a)							GEO_NODE_ORTHO(a),
#define hmsClear(a,b)						GEO_BACKGROUND_COLOR(a),
#define	RGBA16(r,g,b,a)						(((r) << 11) | ((g) << 6) | ((b) << 1) | (a))
#define hmsObject()							GEO_RENDER_OBJ(),
#define hmsTrans(a,b,c)						GEO_TRANSLATE_NODE(0x00, a, b, c),

/*
 * camera functions
 */
#define ZoomControl		geo_camera_fov
#define GameCamera		geo_camera_main
#define WeatherProc		geo_envfx_main
#define CAM_FIELD		1
#define CAM_DUNGEON		4
#define CAM_DUNGEON_O	16

/*
 * screen info
 */
#define	SCREEN_WIDTH	320
#define	SCREEN_HEIGHT	240
#define	SCREEN_ASPECT	(320.0f/240.0f)
#define	SCREEN_NEAR		100.0f
#define	SCREEN_FAR		12800.0f
#define	SCREEN_ANGLE	30.0f

/*
 * animation flags
 */
#define	MAP_ANIM_NORMAL		0x00
#define	MAP_ANIM_ONETIME	0x01
#define	MAP_ANIM_REVERSE	0x02
#define	MAP_ANIM_FREEZE		0x04
#define	MAP_ANIM_TRVERTI	0x08
#define	MAP_ANIM_TRPLANE	0x10
#define	MAP_ANIM_FIXSHADOW	0x20
#define	MAP_ANIM_NOTRANS	0x40

/*
 * 'AnimeRecord' and 'AnimePtr' type definitions
 */
typedef struct Animation AnimeRecord, *AnimePtr;

/*
 * collision flags
 */
#define	BGCHECKCODE_NORMAL		0
#define BGCHECKCODE_POLYGON		BGCHECKCODE_NORMAL
#define	BGCHECKCODE_POINT		64
#define	BGCHECKCODE_PLANEEND	65
#define	BGCHECKCODE_ALLEND		66

#endif /* HEADERS_H */
