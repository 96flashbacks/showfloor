// 0x0E000D14
const GeoLayout chimney_geo[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x96, 70),
    GEO_OPEN_NODE(),
        GEO_OPEN_NODE(),
            GEO_OPEN_NODE(),
                GEO_DISPLAY_LIST(LAYER_OPAQUE, snow_slider_obj_00_mesh),
            GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(), //! more close than open nodes
    GEO_END(),
};
