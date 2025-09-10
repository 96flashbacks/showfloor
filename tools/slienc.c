// slienc decompilation, modified for SMSR00
// https://github.com/klorfmorf/smsr00

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>

/* Globals */
FILE *fp;
unsigned char *bz;     /* input buffer */
int insize;

unsigned char *code;   /* code stream buffer (control+pair halfwords) */
int code_len;
int code_cap;

unsigned char *pbuf;   /* PBUF stream buffer (literal bytes) */
int dp;                /* pbuf length */
int pbuf_cap;

char skip[256];        /* BM skip table */

/* Function prototypes */
int main(int argc, char **argv);
int encode(void);
void search(unsigned int pos, int size, int *match_pos, unsigned int *match_len);
int mischarsearch(char *pattern, int pattern_len, char *text, int text_len);
void initskip(char *pattern, int pattern_len);
unsigned short flipshort(unsigned short value);
unsigned int flipint(unsigned int value);

/* Helpers for dynamic buffers */
static void ensure_code_capacity(int need)
{
    if (code_len + need <= code_cap) return;
    do { code_cap = code_cap ? (code_cap * 2) : 0x1000; } while (code_len + need > code_cap);
    code = (unsigned char*)realloc(code, code_cap);
    if (!code) { fprintf(stderr, "OOM: code realloc\n"); exit(1); }
}
static void ensure_pbuf_capacity(int need)
{
    if (dp + need <= pbuf_cap) return;
    do { pbuf_cap = pbuf_cap ? (pbuf_cap * 2) : 0x1000; } while (dp + need > pbuf_cap);
    pbuf = (unsigned char*)realloc(pbuf, pbuf_cap);
    if (!pbuf) { fprintf(stderr, "OOM: pbuf realloc\n"); exit(1); }
}

/* Control word packer state */
static uint16_t cur_ctrl = 0;
static int ctrl_bits_used = 0;   /* 0..16 */
static int ctrl_pos = -1;        /* index in code[] where current ctrl halfword is reserved */

/* Start a new 16-bit control group: reserve 2 bytes in code stream */
static void start_ctrl_group(void)
{
    ensure_code_capacity(2);
    ctrl_pos = code_len;
    code_len += 2;   /* reserve space for ctrl halfword */
    cur_ctrl = 0;
    ctrl_bits_used = 0;
}

/* Finalize current control word: store BE halfword at reserved spot */
static void finalize_ctrl_group(void)
{
    if (ctrl_pos >= 0) {
        code[ctrl_pos]     = (unsigned char)(cur_ctrl >> 8);
        code[ctrl_pos + 1] = (unsigned char)(cur_ctrl & 0xFF);
        /* leave ctrl_pos as-is; next emission will start a new group if needed */
    }
}

/* Emit one literal: set next ctrl bit=1 and append byte to PBUF */
static void emit_literal(unsigned char lit)
{
    if (ctrl_bits_used == 0) start_ctrl_group();
    /* set bit MSB->LSB: first bit is bit15 */
    int bit_index = ctrl_bits_used; /* 0..15 */
    cur_ctrl |= (uint16_t)(0x8000u >> bit_index);
    ctrl_bits_used++;

    ensure_pbuf_capacity(1);
    pbuf[dp++] = lit;

    if (ctrl_bits_used == 16) {
        finalize_ctrl_group();
        /* next emission will start a new group */
        ctrl_bits_used = 0;
        ctrl_pos = -1;
    }
}

/* Emit one match: set next ctrl bit=0 and append (len/dist) pair (BE) to code stream */
static void emit_match(int distance, unsigned int match_len)
{
    if (ctrl_bits_used == 0) start_ctrl_group();
    /* bit=0 means match; nothing to set in cur_ctrl */
    ctrl_bits_used++;

    uint16_t pair = (uint16_t)(((match_len - 3) << 12) | (distance & 0x0FFF));
    ensure_code_capacity(2);
    code[code_len++] = (unsigned char)(pair >> 8);
    code[code_len++] = (unsigned char)(pair & 0xFF);

    if (ctrl_bits_used == 16) {
        finalize_ctrl_group();
        ctrl_bits_used = 0;
        ctrl_pos = -1;
    }
}

/*
 * Main
 */
int main(int argc, char **argv)
{
    struct timeval start_time, end_time;
    double start_seconds, end_seconds;
    char filename[128];
    int i;

    /* check command line arguments */
    if (argc < 2) {
        fprintf(stderr, "slide [ファイル名]\n");
        exit(1);
    }

    /* copy input filename */
    strncpy(filename, argv[1], sizeof(filename) - 1);
    filename[sizeof(filename) - 1] = '\0';

    /* open input file */
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "FILE OPEN ERROR![%s]\n", filename);
        exit(1);
    }

    /* get file size */
    fseek(fp, 0, SEEK_END);
    insize = (int)ftell(fp);
    fseek(fp, 0, SEEK_SET);

    /* read input */
    bz = (unsigned char*)malloc(insize);
    if (!bz) { fprintf(stderr, "OOM: input buffer\n"); exit(1); }
    if ((int)fread(bz, 1, insize, fp) != insize) {
        fprintf(stderr, "READ ERROR![%s]\n", filename);
        exit(1);
    }
    fclose(fp);

    /* make output filename (.szp) */
    i = 0;
    while (filename[i] != '\0' && filename[i] != '.') i++;
    filename[i] = '\0';
    strcat(filename, ".szp");

    /* open output file */
    fp = fopen(filename, "wb");
    if (fp == NULL) {
        fprintf(stderr, "FILE CREATE ERROR![%s]\n", filename);
        exit(1);
    }

    /* time the encoding process */
    gettimeofday(&start_time, NULL);
    start_seconds = (double)start_time.tv_sec + (double)start_time.tv_usec / 1000000.0;

    /* perform compression */
    encode();

    /* calculate encoding time */
    gettimeofday(&end_time, NULL);
    end_seconds = (double)end_time.tv_sec + (double)end_time.tv_usec / 1000000.0;
    printf("encode time=[%f]\n", end_seconds - start_seconds);

    /* write header and streams */
    fwrite("SMSR00", 6, 1, fp);

    /* pad */
    fputc(0, fp);
    fputc(0, fp);

    uint32_t be_size = flipint((uint32_t)insize);

    /* align code stream to 4 bytes */
    int aligned_code_len = (code_len + 3) & ~3;
    int pad_len = aligned_code_len - code_len;

    /* PBUF offset is relative to code start (0x10), and is 4-byte aligned */
    uint32_t be_pbuf_offs = flipint((uint32_t)aligned_code_len);

    /* write uncompressed size */
    fwrite(&be_size, 4, 1, fp);

    /* write PBUF offset in bytes from code start */
    fwrite(&be_pbuf_offs, 4, 1, fp);

    /* write code stream */
    fwrite(code, 1, (size_t)code_len, fp);

    /* pad with zeros to 4-byte boundary */
    if (pad_len) {
        static const unsigned char z[4] = {0,0,0,0};
        fwrite(z, 1, (size_t)pad_len, fp);
    }

    /* write PBUF stream (literals) */
    fwrite(pbuf, 1, (size_t)dp, fp);

    fclose(fp);
    return 0;
}

/*
 * LZ-style encoder
 */
int encode(void)
{
    unsigned int pos = 0;

    /* init buffers */
    code = NULL; code_len = 0; code_cap = 0;
    pbuf = NULL; dp = 0; pbuf_cap = 0;

    /* init ctrl packer */
    cur_ctrl = 0;
    ctrl_bits_used = 0;
    ctrl_pos = -1;

    /* main loop */
    while (pos < (unsigned int)insize) {
        int match_pos, next_match_pos;
        unsigned int match_len = 0, next_match_len = 0;

        /* find match at pos */
        search(pos, insize, &match_pos, &match_len);

        if (match_len < 3) {
            /* literal */
            emit_literal(bz[pos]);
            pos++;
        } else {
            /* check lazy match: lookahead one byte */
            search(pos + 1, insize, &next_match_pos, &next_match_len);
            if (match_len + 1 < next_match_len) {
                /* emit current byte as literal */
                emit_literal(bz[pos]);
                pos++;
                /* adopt next match */
                match_len = next_match_len;
                match_pos = next_match_pos;
            }

            /* emit the match at current pos */
            int distance = (int)((pos - (unsigned int)match_pos) - 1u); /* 0..0xFFF */
            if (distance < 0 || distance > 0x0FFF) {
                /* fallback safety: if out of range, force literal and continue */
                emit_literal(bz[pos]);
                pos++;
                continue;
            }
            emit_match(distance, match_len);
            pos += match_len;
        }
    }

    /* finalize tail control word (if partially filled) */
    if (ctrl_bits_used != 0) {
        finalize_ctrl_group();
        ctrl_bits_used = 0;
        ctrl_pos = -1;
    }

    /* stats */
    fprintf(stderr, "IN=%d OUT=%d\n", insize, code_len + dp + 16);

    return code_len + dp + 16;
}

/*
 * Search for the longest match within the sliding window (<= 0x1000 back, len max 0x12)
 */
void search(unsigned int pos, int size, int *match_pos, unsigned int *match_len)
{
    unsigned int window_start = 0;
    unsigned int max_match_len = 3;
    unsigned int remaining = (unsigned int)size - pos;
    unsigned int search_pos;
    unsigned int found_pos;
    char *current_ptr;

    if (pos > 0x1000) {
        window_start = pos - 0x1000;
    }
    if (remaining > 0x12) {
        remaining = 0x12; /* max len 18 */
    }
    if (remaining < 3) {
        *match_len = 0;
        *match_pos = 0;
        return;
    }

    for (search_pos = window_start; search_pos < pos; ) {
        found_pos = (unsigned int)mischarsearch((char*)bz + pos, (int)max_match_len,
                                               (char*)bz + search_pos, (int)((pos + max_match_len) - search_pos));
        if (found_pos >= pos - search_pos) {
            break;
        }

        *match_pos = (int)(search_pos + found_pos);

        /* extend match */
        if ((max_match_len < remaining) &&
            (bz[*match_pos + max_match_len] == bz[pos + max_match_len])) {
            current_ptr = (char*)bz + pos + max_match_len;
            do {
                current_ptr++;
                max_match_len++;
                if (remaining <= max_match_len) break;
            } while (bz[*match_pos + max_match_len] == *(unsigned char*)current_ptr);
        }

        if (max_match_len == remaining) {
            *match_len = max_match_len;
            return;
        }

        max_match_len++;
        search_pos = found_pos + 1 + search_pos;
    }

    if (max_match_len < 4) {
        *match_len = 0;
    } else {
        *match_len = max_match_len - 1;
    }
}

/*
 * Boyer-Moore mismatch-char search
 */
int mischarsearch(char *pattern, int pattern_len, char *text, int text_len)
{
    int i, j, text_pos;
    unsigned int skip_dist;

    if (text_len < pattern_len) {
        return text_len;
    }

    initskip(pattern, pattern_len);
    text_pos = pattern_len - 1;

    while (1) {
        j = pattern_len - 2;
        i = text_pos;

        if ((unsigned char)text[text_pos] == (unsigned char)pattern[pattern_len - 1]) {
            while (1) {
                text_pos--;
                if (j < 0) {
                    return i;  /* match */
                }
                if ((unsigned char)text[text_pos] != (unsigned char)pattern[j]) {
                    break;
                }
                j--;
                i = text_pos;
            }
            skip_dist = (unsigned char)skip[(unsigned char)text[text_pos]];
            if (skip_dist < (unsigned int)(pattern_len - j)) {
                skip_dist = (unsigned int)(pattern_len - j);
            }
        } else {
            skip_dist = (unsigned char)skip[(unsigned char)text[text_pos]];
        }
        text_pos += (int)skip_dist;
    }
}

void initskip(char *pattern, int pattern_len)
{
    int i;
    memset(skip, pattern_len, 256);
    for (i = 0; i < pattern_len; i++) {
        skip[(unsigned char)pattern[i]] = (char)(pattern_len - i - 1);
    }
}

unsigned short flipshort(unsigned short value)
{
    return (unsigned short)(((value & 0xFF) << 8) | ((value >> 8) & 0xFF));
}

unsigned int flipint(unsigned int value)
{
    return (value << 24) | ((value & 0xFF00) << 8) | ((value >> 8) & 0xFF00) | (value >> 24);
}

