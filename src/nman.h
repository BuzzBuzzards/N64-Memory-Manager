#include <libdragon.h>

void new_scroll_pos(int *cursor, int *page, int max, int count);
void display_notes(int cursor, int page, int max, int count, int controller);
void delete_note(entry_structure_t *entry, int controller);
void nman_menu(int controller);
void nman_entry(int controler);