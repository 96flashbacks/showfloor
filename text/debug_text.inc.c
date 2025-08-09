// == debug table ==

// (this wasn't translated for US, and was removed in EU)

static const u8 Debug0[] = {
    _("ＳＴＡＧＥ　ＳＥＬＥＣＴ\n"
      "　ＣＯＮＴＩＮＵＥ？\n"
      "　１　ＭＯＵＮＴＡＩＮ\n"
      "　２　ＦＩＲＥ\n"
      "　３　ＳＮＯＷ\n"
      "　４　ＷＡＴＥＲ\n"
      "　　　ＢＯＷＳＥＲ\n"
      "　ＢＡＣＫ")
};

static const u8 Debug1[] = {
    _("ＰＡＵＳＥ　　　　\n"
      "　ＣＯＮＴＩＮＵＥ？\n"
      "　ＱＵＩＴ？")
};

static const struct DialogEntry debug_text_entry_0 = {
    1, 8, 52, 200, Debug0
};

static const struct DialogEntry debug_text_entry_1 = {
    1, 3, 100, 150, Debug1
};

const struct DialogEntry *const seg2_debug_text_table[] = {
    &debug_text_entry_0, &debug_text_entry_1, NULL,
};
