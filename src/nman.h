#include <libdragon.h>

typedef struct
{
    entry_structure_t entry;
} all_entries_t;

void new_scroll_pos(int *cursor, int *page, int max, int count);
void display_notes(entry_structure_t list[16], int cursor, int page, int max, int count, int controller);
void delete_note(entry_structure_t *entry, int controller);
void nman_menu(int controller, entry_structure_t list[16]);
