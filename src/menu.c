#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <libdragon.h>

void show_main_menu()
{
    console_clear();

    printf("Press A on a controller\n"
           "to read that controller's\n"
           "mempak.\n\n"
           "Press B to format mempak.\n\n"
           "Press L to save mempak to file.\n\n"
           "Press R to load mempak from file.");

    console_render();
}

int main_menu_prompt()
{
    printf(
        "\n\n\n"
        "To return to main menu press start\n\n");
    console_render();

    while (1)
    {
        controller_scan();
        struct controller_data keys = get_keys_down();

        for (int i = 0; i < 4; i++)
        {
            if (keys.c[i].err == ERROR_NONE)
            {
                if (keys.c[i].start)
                {
                    show_main_menu();
                    return (0);
                }
            }
        }
    }
}
