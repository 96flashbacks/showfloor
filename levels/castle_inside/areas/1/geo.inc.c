const GeoLayout castle_geo_lobby[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, castle_inside_main_dl_mesh),
      GEO_DISPLAY_LIST(LAYER_TRANSPARENT, castle_inside_decal_dl_mesh),
   GEO_CLOSE_NODE(),
   GEO_RETURN(),
};

const GeoLayout castle_geo_lobby_to_bowser[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, castle_inside_main_dl_mesh),
      GEO_DISPLAY_LIST(LAYER_TRANSPARENT, castle_inside_decal_dl_mesh),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, castle_inside_bowser_dl_mesh),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, castle_inside_bowser_painting_dl_mesh),
   GEO_CLOSE_NODE(),
   GEO_RETURN(),
};

const GeoLayout castle_geo_bowser_hallway[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, castle_inside_bowser_dl_mesh),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, castle_inside_bowser_painting_dl_mesh),
   GEO_CLOSE_NODE(),
   GEO_RETURN(),
};

const GeoLayout castle_geo_lobby_to_mountain[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, castle_inside_main_dl_mesh),
      GEO_DISPLAY_LIST(LAYER_TRANSPARENT, castle_inside_decal_dl_mesh),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, castle_inside_mountain_dl_mesh),
      GEO_ASM(  0, geo_painting_update),
      GEO_ASM(0, geo_painting_draw),
   GEO_CLOSE_NODE(),
   GEO_RETURN(),
};

const GeoLayout castle_geo_mountain_room[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, castle_inside_mountain_dl_mesh),
      GEO_ASM(  0, geo_painting_update),
      GEO_ASM(0, geo_painting_draw),
   GEO_CLOSE_NODE(),
   GEO_RETURN(),
};

const GeoLayout castle_geo_lobby_to_fire_bubble[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, castle_inside_main_dl_mesh),
      GEO_DISPLAY_LIST(LAYER_TRANSPARENT, castle_inside_decal_dl_mesh),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, castle_inside_fire_bubble_dl_mesh),
      GEO_DISPLAY_LIST(LAYER_TRANSPARENT, castle_inside_fire_bubble_decal_dl_mesh),
      GEO_ASM(  0, geo_painting_update),
      GEO_ASM(1, geo_painting_draw),
   GEO_CLOSE_NODE(),
   GEO_RETURN(),
};

const GeoLayout castle_geo_fire_bubble_room[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, castle_inside_fire_bubble_dl_mesh),
      GEO_DISPLAY_LIST(LAYER_TRANSPARENT, castle_inside_fire_bubble_decal_dl_mesh),
      GEO_ASM(  0, geo_painting_update),
      GEO_ASM(1, geo_painting_draw),
   GEO_CLOSE_NODE(),
   GEO_RETURN(),
};

const GeoLayout castle_geo_lobby_to_water_land[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, castle_inside_main_dl_mesh),
      GEO_DISPLAY_LIST(LAYER_TRANSPARENT, castle_inside_decal_dl_mesh),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, castle_inside_water_land_dl_mesh),
      GEO_ASM(  0, geo_painting_update),
      GEO_ASM(3, geo_painting_draw),
   GEO_CLOSE_NODE(),
   GEO_RETURN(),
};

const GeoLayout castle_geo_water_land_room[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, castle_inside_water_land_dl_mesh),
      GEO_ASM(  0, geo_painting_update),
      GEO_ASM(3, geo_painting_draw),
   GEO_CLOSE_NODE(),
   GEO_RETURN(),
};

const GeoLayout castle_geo_lobby_to_snow_slider[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, castle_inside_main_dl_mesh),
      GEO_DISPLAY_LIST(LAYER_TRANSPARENT, castle_inside_decal_dl_mesh),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, castle_inside_snow_slider_dl_mesh),
      GEO_ASM(  0, geo_painting_update),
      GEO_ASM(2, geo_painting_draw),
   GEO_CLOSE_NODE(),
   GEO_RETURN(),
};

const GeoLayout castle_geo_snow_slider_room[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, castle_inside_snow_slider_dl_mesh),
      GEO_ASM(  0, geo_painting_update),
      GEO_ASM(2, geo_painting_draw),
   GEO_CLOSE_NODE(),
   GEO_RETURN(),
};

const GeoLayout castle_geo_lobby_to_hallway[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, castle_inside_main_dl_mesh),
      GEO_DISPLAY_LIST(LAYER_TRANSPARENT, castle_inside_decal_dl_mesh),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, castle_inside_hallway_dl_mesh),
   GEO_CLOSE_NODE(),
   GEO_RETURN(),
};

const GeoLayout castle_geo_hallway[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, castle_inside_hallway_dl_mesh),
   GEO_CLOSE_NODE(),
   GEO_RETURN(),
};

const GeoLayout castle_geo[] = {
   GEO_NODE_SCREEN_AREA(10, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2),
   GEO_OPEN_NODE(),
      GEO_ZBUFFER(0),
      GEO_OPEN_NODE(),
         GEO_NODE_ORTHO(100),
         GEO_OPEN_NODE(),
            GEO_BACKGROUND_COLOR(0x0001),
         GEO_CLOSE_NODE(),
      GEO_CLOSE_NODE(),
      GEO_ZBUFFER(1),
      GEO_OPEN_NODE(),
         GEO_CAMERA_FRUSTUM_WITH_FUNC(45, 100, 7000, geo_camera_fov),
         GEO_OPEN_NODE(),
            GEO_CAMERA(13, 0, 2000, 6000, 0, 0, 0, geo_camera_main),
            GEO_OPEN_NODE(),
               GEO_SWITCH_CASE(3, geo_switch_area), //67 funny
               GEO_OPEN_NODE(),
                  GEO_BRANCH(1, castle_geo_lobby),
                  GEO_BRANCH(1, castle_geo_lobby_to_mountain),
                  GEO_BRANCH(1, castle_geo_lobby_to_mountain),
                  GEO_BRANCH(1, castle_geo_lobby_to_bowser),
                  GEO_BRANCH(1, castle_geo_lobby_to_bowser),
                  GEO_BRANCH(1, castle_geo_lobby_to_fire_bubble), // area 7
                  GEO_BRANCH(1, castle_geo_lobby_to_fire_bubble), // area 6
                  GEO_BRANCH(1, castle_geo_lobby_to_water_land),
                  GEO_BRANCH(1, castle_geo_lobby_to_snow_slider),
                  GEO_BRANCH(1, castle_geo_lobby_to_snow_slider),
                  GEO_BRANCH(1, castle_geo_lobby_to_hallway),
                  GEO_BRANCH(1, castle_geo_lobby_to_hallway),
                  GEO_BRANCH(1, castle_geo_lobby_to_water_land),
               GEO_CLOSE_NODE(),
               GEO_RENDER_OBJ(),
               GEO_ASM(0, geo_envfx_main),
            GEO_CLOSE_NODE(),
         GEO_CLOSE_NODE(),
      GEO_CLOSE_NODE(),
   GEO_CLOSE_NODE(),
   GEO_END(),
};
