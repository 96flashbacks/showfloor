
ALIGNED8 static const u8 Mountain_AreaMap_upper_image_txt[] = {
    #include "levels/wf/areamap_upper_image.rgba16.inc.c"
};

ALIGNED8 static const u8 Mountain_AreaMap_lower_image_txt[] = {
    #include "levels/wf/areamap_lower_image.rgba16.inc.c"
};

AreaMapData map_mountain = {
	Mountain_AreaMap_upper_image_txt, // top image
	Mountain_AreaMap_lower_image_txt, // bottom image
	// these seem to be related to the arrow's positioning, but in what way? - Toru
	-561.f,  // x offset
	-561.f,  // z offset
	5300.f,  // map scale
};

