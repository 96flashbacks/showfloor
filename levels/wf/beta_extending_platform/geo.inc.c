const GeoLayout wf_geo_000AF8[] = { // RCP_HmsLift0
    GEO_CULLING_RADIUS(650),
    GEO_OPEN_NODE(),
        // Same geolayout, but the platform model is different, it's not as wide as the leftover
        GEO_DISPLAY_LIST(LAYER_OPAQUE, beta_extending_platform_dl_mesh),
    GEO_CLOSE_NODE(),
    GEO_END(),
};
