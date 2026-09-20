const GeoLayout wf_geo_000B10[] = { // RCP_HmsLift1 (modified)
    GEO_CULLING_RADIUS(650),
    GEO_OPEN_NODE(),
        // No shadow   
        GEO_DISPLAY_LIST(LAYER_OPAQUE, wf_seg7_dl_0700F018),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout wf_geo_000B38[] = { // RCP_HmsLift2 (modified)
    GEO_CULLING_RADIUS(650),
    GEO_OPEN_NODE(),
        // No shadow
        GEO_DISPLAY_LIST(LAYER_OPAQUE, wf_seg7_dl_0700F018),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

const GeoLayout wf_geo_000B60[] = { // RCP_HmsLift3 (modified)
    GEO_CULLING_RADIUS(650),
    GEO_OPEN_NODE(),
        // Different DL so the elevator platform can have a ? on it
        GEO_DISPLAY_LIST(LAYER_OPAQUE, extending_platform_decal_dl_mesh),
    GEO_CLOSE_NODE(),
    GEO_END(),
};
