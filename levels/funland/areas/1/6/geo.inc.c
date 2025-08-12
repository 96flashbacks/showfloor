const GeoLayout castle_grounds_tower_geo[] = {
   GEO_CULLING_RADIUS(1600),
   GEO_OPEN_NODE(),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, castle_grounds_tower_1_dl_mesh),
      GEO_DISPLAY_LIST(LAYER_ALPHA, castle_grounds_tower_2_dl_mesh),
   GEO_CLOSE_NODE(),
   GEO_END(),
};
