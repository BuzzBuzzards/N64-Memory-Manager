#include <libdragon.h>
#include "nman.h"

entry_structure_t all_entries[16];

entry_structure_t note_buffer;

int w_res;
int g_res;
int r_res;

void nman_entry(int controler)
{
    console_clear();

    int err;
    if ((err = validate_mempak(controler)))
    {
        if (err == -3)
        {
            printf("Mempak is not formatted!");
        }
        else
        {
            printf("Mempak bad or removed during read!");
        }
    }
    else
    {
        nman_menu(controler);
    }
}

void get_note_data(int controller_number)
{
    for (int j = 0; j < 16; j++)
    {
        entry_structure_t entry;

        get_mempak_entry(controller_number, j, &entry);

        all_entries[j] = entry;
    }
}

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

void display_notes(int cursor, int page, int max, int count, int controller)
{

    console_clear();

    printf("Up/Down - select, Z - delete, Start - exit\n"
    // "L - copy selected note, R - paste\n\n"
    );

    // printf("Note in buffer: %s. w_res:%i g_res:%i r_res:%i\n",note_buffer.name, w_res, g_res, r_res);

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

        if (all_entries[i].valid) 
        {
            printf("[%s]\t[%i]\t%i\n", all_entries[i].name,all_entries[i].game_id,all_entries[i].blocks);
        } else 
        {
            printf("------------\n");
        }   
    }

    printf("\nFree space: %d blocks\n", get_mempak_free_space(controller));

    console_render();

}

void delete_note(entry_structure_t *entry, int controller)
{
    delete_mempak_entry(controller, entry);
}

void copy_note(int controller, int cursor) 
{
    get_mempak_entry(controller,cursor,&note_buffer);
};

// void paste_note(int controller, int cursor) 
// {
//     uint8_t entry_data;
//     r_res = read_mempak_entry_data(controller,&note_buffer,&entry_data);
//     entry_structure_t selected_entry;
//     g_res = get_mempak_entry(controller,cursor,&selected_entry);
//     w_res = write_mempak_entry_data(controller,&selected_entry,&entry_data);
// };

void nman_menu(int controller)
{

    get_note_data(controller);

    int count = 16;
    int page = 0;
    int cursor = 0;

    display_notes(cursor,page,16,count,controller);

    while (1)
    {
        controller_scan();
        struct controller_data keys = get_keys_down();

        for (int i = 0; i < 4; i++)
        {
            if (keys.c[i].err == ERROR_NONE)
            {
                if (keys.c[i].Z)
                {
                    delete_note(&all_entries[cursor],controller);
                    get_note_data(controller);
                    display_notes(cursor,page,16,count,controller);
                }
                if (keys.c[i].up)
                {
                    cursor--;
                    new_note_pos(&cursor, &page, 16, count);
                    display_notes(cursor,page,16,count,controller);
                }
                if (keys.c[i].down)
                {
                    cursor++;
                    new_note_pos(&cursor, &page, 16, count);
                    display_notes(cursor,page,16,count,controller);
                }
                if (keys.c[i].L)
                {
                    copy_note(controller,cursor);
                    display_notes(cursor,page,16,count,controller);
                }
                // if (keys.c[i].R)
                // {
                //     paste_note(controller,cursor);
                //     display_notes(cursor,page,16,count,controller);
                // }
                if (keys.c[i].start)
                {
                    console_clear();
                    console_render();
                    return;
                }
            }
        }
    }
}
