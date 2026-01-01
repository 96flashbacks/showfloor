#ifdef POWBUILD
const GeoLayout wf_geo_12[] = {
   GEO_CULLING_RADIUS(1600),
   GEO_OPEN_NODE(),
      GEO_RENDER_RANGE(-2048, 5000),
      GEO_OPEN_NODE(),
         GEO_DISPLAY_LIST(LAYER_OPAQUE, mountain_12_dl_mesh),
      GEO_CLOSE_NODE(),
      GEO_RENDER_RANGE(5000, 32767),
      GEO_OPEN_NODE(),
         GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, 0, 0, 0, 0, 180, 0, mountain_12_lod_dl_mesh),
      GEO_CLOSE_NODE(),
   GEO_CLOSE_NODE(),
   GEO_END(),
};
#else
const GeoLayout wf_geo_12[] = {
   GEO_CULLING_RADIUS(1600),
   GEO_OPEN_NODE(),
      GEO_RENDER_RANGE(-2048, 5000),
      GEO_OPEN_NODE(),
         GEO_DISPLAY_LIST(LAYER_OPAQUE, mountain_12_dl_mesh),
      GEO_CLOSE_NODE(),
   GEO_CLOSE_NODE(),
   GEO_END(),
};
#endif
