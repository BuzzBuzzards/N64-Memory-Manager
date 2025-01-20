#include <libdragon.h>

typedef struct
{
    entry_structure_t entry;
} all_entries_t;


void new_note_pos(int *cursor, int *page, int max, int count)
{
    /* Make sure windows too small can be calculated right */
    if(max > count) { max = count; }

    /* Bounds checking */
    if(*cursor >= count)
    {
        *cursor = count-1;
    }

    if(*cursor < 0)
    {
        *cursor = 0;
    }

    /* Scrolled up? */
    if(*cursor < *page)
    {
        *page = *cursor;
        return;
    }

    /* Scrolled down/ */
    if(*cursor >= (*page + max))
    {
        *page = (*cursor - max) + 1;
        return;
    }

    /* Nothing here, should be good! */
}

void display_notes(entry_structure_t list[16], int cursor, int page, int max, int count, int controller)
{

    printf("Note list for controller %i\n\n",controller);

    /* Page bounds checking */
    if(max > count)
    {
        max = count;
    }

    /* Cursor bounds checking */
    if(cursor >= (page + max))
    {
        cursor = (page + max) - 1;
    }

    if(cursor < page)
    {
        cursor = page;
    }

    printf("[%s]\t[%s]\t%s\n\n", "Note name","GameID","Pages");

    for(int i = page; i < 16; i++)
    {
        if(i == cursor)
        {
            printf("> ");
        }
        else
        {
            printf("  ");
        }

        if (list[i].valid) 
        {
            printf("[%s]\t[%i]\t%i\n", list[i].name,list[i].game_id,list[i].blocks);
        } else 
        {
            printf("------------\n");
        }   
    }

}

void delete_note(entry_structure_t *entry, int controller)
{
    delete_mempak_entry(controller, entry);
}

void nman_menu(int controller, entry_structure_t list[16])
{
    console_clear();

    console_render();

    int count = 0;
    int page = 0;
    int cursor = 0;

    display_notes(list,cursor,page,16,count,controller);

    console_render();

    while (1)
    {
        controller_scan();
        struct controller_data keys = get_keys_down();

        for (int i = 0; i < 4; i++)
        {
            if (keys.c[i].err == ERROR_NONE)
            {
                if (keys.c[i].A)
                {
                    delete_note(&list[cursor],controller);
                }
                if (keys.c[i].up)
                {
                    cursor--;
                    new_note_pos(&cursor, &page, 16, count);
                    console_clear();
                    display_notes(list,cursor,page,16,count,controller);
                    console_render();
                }
                if (keys.c[i].down)
                {
                    cursor++;
                    new_note_pos(&cursor, &page, 16, count);
                    console_clear();
                    display_notes(list,cursor,page,16,count,controller);
                    console_render();
                }
            }
        }
    }
}
