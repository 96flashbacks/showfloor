const GeoLayout wf_geo_10[] = {
   GEO_CULLING_RADIUS(2700),
   GEO_OPEN_NODE(),
      GEO_RENDER_RANGE(-2048, 5000),
      GEO_OPEN_NODE(),
         GEO_DISPLAY_LIST(LAYER_OPAQUE, mountain_10_dl_mesh),
      GEO_CLOSE_NODE(),
      GEO_RENDER_RANGE(5000, 32767),
      GEO_OPEN_NODE(),
         GEO_DISPLAY_LIST(LAYER_OPAQUE, wf_seg7_dl_07008FE8),
      GEO_CLOSE_NODE(),
   GEO_CLOSE_NODE(),
   GEO_END(),
};
