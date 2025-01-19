#ifndef N64FS_H
#define N64FS_H

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <libdragon.h>

#define MAX_LIST            20

typedef struct
{
    uint32_t type;
    char filename[MAX_FILENAME_LEN+1];
} direntry_t;

extern void chdir( const char * const dirent );
extern int compare(const void * a, const void * b);
extern direntry_t *populate_dir(int *count);
extern void new_scroll_pos(int *cursor, int *page, int max, int count);
extern void display_dir(direntry_t *list, int cursor, int page, int max, int count);

#endif

