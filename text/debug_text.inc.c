// == debug table ==

// (this wasn't translated for US, and was removed in EU)

static const u8 Debug0[] = {
    _("STAGE SELECT\n"
      "  Continue?\n"
      "  1 o Muntain\n"
      "  2  Fire Bubble\n"
      "  3  Snow Slider\n"
      "  4  Water Land\n"
      "     Bowser Stage 1\n"
      "  Back")
};

static const u8 Debug1[] = {
    _("PAUSE\n"
      "  Continue?\n"
      "  Quit?")
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
