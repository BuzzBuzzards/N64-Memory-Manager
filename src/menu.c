#include "menu.h"
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

int aftermarket_validation()
{
    console_clear();

    printf("Is your controller an aftermarket one? \n\n"
           "If it is, press button A\n\n"
           "If it's something else inserted in your \n"
           "controller (such as Rumble Pack), press button B\n");

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
                    return (1);
                    break;
                }
                else if (keys.c[i].B)
                {
                    return (0);
                    break;
                }
            }
        }
    }
}

void menu_processor(void (*action)(int i), int i)
{
    console_clear();

    /* Read accessories present, throwing away return.  If we don't do this, then
        initialization routines in the identify_accessory() call will fail once we
        remove and insert a new accessory while running */
    struct controller_data output;
    get_accessories_present(&output);

    /* Make sure they don't have a rumble pak inserted instead */
    switch (identify_accessory(i))
    {
    case ACCESSORY_NONE:
        printf("No accessory inserted!");
        break;
    case ACCESSORY_MEMPAK:
    {
        (*action)(i);
        break;
    }
    case ACCESSORY_RUMBLEPAK:

        switch (aftermarket_validation())
        {
        case 0:
            console_clear();
            printf("Cannot read data off of rumblepak!");
            break;

        case 1:
            (*action)(i);
            break;
        default:
            break;
        };
    }

    main_menu_prompt();

    console_render();
};
