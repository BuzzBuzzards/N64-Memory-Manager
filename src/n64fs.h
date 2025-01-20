#ifndef N64FS_H
#define N64FS_H

#include <libdragon.h>

#define MAX_LIST            20

typedef struct
{
    uint32_t type;
    char filename[MAX_FILENAME_LEN+1];
} direntry_t;

void chdir(const char * const dirent, char dir[512]);
extern int compare(const void * a, const void * b);
direntry_t *populate_dir(int *count, char dir[512]);
extern void new_scroll_pos(int *cursor, int *page, int max, int count);
extern void display_dir(direntry_t *list, int cursor, int page, int max, int count);
void free_dir(direntry_t *dir);

#endif

