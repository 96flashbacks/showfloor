const GeoLayout wf_geo_6[] = {
   GEO_CULLING_RADIUS(1800),
   GEO_OPEN_NODE(),
      GEO_RENDER_RANGE(-2048, 5000),
      GEO_OPEN_NODE(),
         GEO_DISPLAY_LIST(LAYER_OPAQUE, mountain_6_dl_mesh),
      GEO_CLOSE_NODE(),
      GEO_RENDER_RANGE(5000, 32767),
      GEO_OPEN_NODE(),
         GEO_DISPLAY_LIST(LAYER_OPAQUE, mountain_6_lod_dl_mesh),
      GEO_CLOSE_NODE(),
   GEO_CLOSE_NODE(),
   GEO_END(),
};
