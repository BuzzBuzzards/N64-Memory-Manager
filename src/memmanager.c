#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>
#include "n64fs.h"
#include "menu.h"

#define u8 unsigned char
#define MAX_SUPPORTED_PATH_LEN 256
#define ED64_FIRMWARE_PATH "ED64P"

static uint8_t mempak_data[128 * MEMPAK_BLOCK_SIZE];

int filesize(FILE *pFile)
{
    fseek(pFile, 0, SEEK_END);
    int lSize = ftell(pFile);
    rewind(pFile);

    return lSize;
}


void view_pak_conents(int controller_number)
{
    console_clear();

    int err;
    if ((err = validate_mempak(controller_number)))
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
        for (int j = 0; j < 16; j++)
        {
            entry_structure_t entry;

            get_mempak_entry(controller_number, j, &entry);

            if (entry.valid)
            {
                printf("%s - %d blocks\n", entry.name, entry.blocks);
            }
            else
            {
                printf("(EMPTY)\n");
            }
        }

        printf("\nFree space: %d blocks", get_mempak_free_space(controller_number));
    }
}

void mempak_format(int controller_number)
{
    console_clear();

    if (format_mempak(controller_number))
    {
        printf("Error formatting mempak!");
    }
    else
    {
        printf("Memory card formatted!");
    }
}

void write_mempak_to_file(int controller_number)
{
    dfs_init(DFS_DEFAULT_LOCATION);

    /* This will initialize the SD filesystem using 'sd:/' to identify it */
    /* This path has to be the same used by the sprites when loading */
    if (!debug_init_sdfs("sd:/", -1))
    {
        printf("Error opening SD!\n");
    }

    char path[156];
    sprintf(path, "sd:/MEMPAKS/mempak_%i_%i.mpk", controller_number, get_mempak_free_space(controller_number));

    FILE *fp = fopen(path, "wb");
    if (!fp)
    {
        printf("\nError creating file!\n");
    }

    size_t bw = fwrite(mempak_data, MEMPAK_BLOCK_SIZE, 128, fp);
    printf("Number of elements written: %i\n", bw);

    fclose(fp);

    debug_close_sdfs();
}

void read_mempak_from_file(int controller_number)
{
    dfs_init(DFS_DEFAULT_LOCATION);

    /* This will initialize the SD filesystem using 'sd:/' to identify it */
    /* This path has to be the same used by the sprites when loading */
    if (!debug_init_sdfs("sd:/", -1))
    {
        printf("Error opening SD!\n");
    }

    // char path[156];

    // printf(path, "rom:/MEMPAKS/gold.mpk");

    FILE *fp = fopen("sd:/MEMPAKS/gold.mpk", "r");
    if (!fp)
    {
        printf("\nError opening file!\n");
    }

    fread(&mempak_data,MEMPAK_BLOCK_SIZE, 128, fp);

    fclose(fp);
    debug_close_sdfs();

    
    // direntry_t *list;
    // int count = 0;
    // int page = 0;
    // int cursor = 0; 

    // console_clear();

    // list = populate_dir(&count);

    // console_clear();
    // display_dir(list, cursor, page, MAX_LIST, count);
    // console_render();
}

void mempak_copy_to_file(int controller_number)
{
    console_clear();

    int err = 0;

    for (int j = 0; j < 128; j++)
    {
        err |= read_mempak_sector(controller_number, j, &mempak_data[j * MEMPAK_BLOCK_SIZE]);
    }

    if (err)
    {
        printf("Error loading data!");
    }
    else
    {
        console_set_render_mode(RENDER_AUTOMATIC);
        printf("Data loaded into RAM!\n\n");

        write_mempak_to_file(controller_number);

        console_set_render_mode(RENDER_MANUAL);
    }
}

void mempak_copy_from_file(int controller_number)
{
    
    read_mempak_from_file(controller_number);

    int err = 0;

    for (int j = 0; j < 128; j++)
    {
        err |= write_mempak_sector(controller_number, j, &mempak_data[j * MEMPAK_BLOCK_SIZE]);
    }

    if (err)
    {
        printf("Error saving data!");
    }
    else
    {
        printf("Data saved into mempak!\n\n");
    }

}

int aftermarket_validation()
{
    console_clear();

    printf("Is your controller an aftermarket one? \n\n"
           "If it is, press button A\n\n"
           "If it's something else inserted in your \n"
           "controller (such as Rumble Pack), press button B");

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

int main(void)
{
    /* Initialize peripherals */
    console_init();
    controller_init();

    console_set_render_mode(RENDER_MANUAL);

    show_main_menu();

    /* Main loop test */
    while (1)
    {
        /* To do initialize routines */

        controller_scan();

        struct controller_data keys = get_keys_down();

        for (int i = 0; i < 4; i++)
        {
            if (keys.c[i].err == ERROR_NONE)
            {
                if (keys.c[i].A)
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
                        view_pak_conents(i);
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
                            view_pak_conents(i);
                            break;
                        default:
                            break;
                        };
                    }

                    main_menu_prompt();

                    console_render();
                }
                else if (keys.c[i].B)
                {
                    console_clear();

                    /* Make sure they don't have a rumble pak inserted instead */
                    switch (identify_accessory(i))
                    {
                    case ACCESSORY_NONE:
                        printf("No accessory inserted!");
                        break;
                    case ACCESSORY_MEMPAK:
                        mempak_format(i);
                        break;
                    case ACCESSORY_RUMBLEPAK:
                        switch (aftermarket_validation())
                        {
                        case 0:
                            printf("Cannot format rumblepak!");
                            break;
                        case 1:
                            mempak_format(i);
                            break;
                        default:
                            break;
                        };
                    }

                    main_menu_prompt();

                    console_render();
                }
                else if (keys.c[i].L)
                {
                    console_clear();

                    /* Make sure they don't have a rumble pak inserted instead */
                    switch (identify_accessory(i))
                    {
                    case ACCESSORY_NONE:
                        printf("No accessory inserted!");
                        break;
                    case ACCESSORY_MEMPAK:
                    {
                        mempak_copy_to_file(i);
                        break;
                    }
                    case ACCESSORY_RUMBLEPAK:
                        switch (aftermarket_validation())
                        {
                        case 0:
                            printf("Cannot copy data from rumblepak!");
                            break;
                        case 1:
                            mempak_copy_to_file(i);
                            break;
                        default:
                            break;
                        }
                    }

                    main_menu_prompt();

                    console_render();
                }
                else if (keys.c[i].R)
                {
                    console_clear();

                    /* Make sure they don't have a rumble pak inserted instead */
                    switch (identify_accessory(i))
                    {
                    case ACCESSORY_NONE:
                        printf("No accessory inserted!");
                        break;
                    case ACCESSORY_MEMPAK:
                    {
                        mempak_copy_from_file(i);
                        break;
                    }
                    case ACCESSORY_RUMBLEPAK:
                        switch (aftermarket_validation())
                        {
                        case 0:
                            printf("Cannot write data to rumblepak!");
                            break;

                        case 1:
                            mempak_copy_from_file(i);
                            break;
                        
                        default:
                            break;
                        }
                        
                    }

                    main_menu_prompt();
                    console_render();
                }
            }
        }
    }
}
