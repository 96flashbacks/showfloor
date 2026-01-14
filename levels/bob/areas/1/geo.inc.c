// 0x0E000488
const GeoLayout bob_geo_000488[] = {
   GEO_NODE_SCREEN_AREA(10, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2),
   GEO_OPEN_NODE(),
      GEO_ZBUFFER(0),
      GEO_OPEN_NODE(),
         GEO_NODE_ORTHO(100),
         GEO_OPEN_NODE(),
            GEO_BACKGROUND(BACKGROUND_OCEAN_SKY, geo_skybox_main),
         GEO_CLOSE_NODE(),
      GEO_CLOSE_NODE(),
      GEO_ZBUFFER(1),
      GEO_OPEN_NODE(),
         GEO_CAMERA_FRUSTUM_WITH_FUNC(45, 100, 30000, geo_camera_fov),
         GEO_OPEN_NODE(),
            GEO_CAMERA(CAMERA_MODE_FREE_ROAM, 0, 2000, 6000, 3072, 0, -4608, geo_camera_main),
            GEO_OPEN_NODE(),
               GEO_DISPLAY_LIST(LAYER_OPAQUE, bobo_1_dl_mesh), //main level
               GEO_DISPLAY_LIST(LAYER_OPAQUE, bobo_2_dl_mesh), //spike
               GEO_DISPLAY_LIST(LAYER_OPAQUE, bobo_3_dl_mesh), //spike
               GEO_DISPLAY_LIST(LAYER_TRANSPARENT_DECAL, bobo_4_dl_mesh), //shadows
               GEO_DISPLAY_LIST(LAYER_ALPHA, bobo_5_dl_mesh), //fences
               GEO_RENDER_OBJ(),
               GEO_ASM(0, geo_envfx_main),
            GEO_CLOSE_NODE(),
         GEO_CLOSE_NODE(),
      GEO_CLOSE_NODE(),
      GEO_ZBUFFER(0),
      GEO_OPEN_NODE(),
         GEO_ASM(0, geo_cannon_circle_base),
      GEO_CLOSE_NODE(),
   GEO_CLOSE_NODE(),
   GEO_END(),
};
