#include <libdragon.h>
#include <unistd.h>
#include "mpk_hw.h"

static uint8_t mempak_data[128 * MEMPAK_BLOCK_SIZE];

bool dev = true;

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

void write_mempak_to_file(int controller_number, char device[])
{
    dfs_init(DFS_DEFAULT_LOCATION);

    char w_path[156];
    sprintf(w_path, "%s:/MEMPAKS/mempak_%i_%i.mpk",device, controller_number, get_mempak_free_space(controller_number));

    FILE *fp = fopen(w_path, "wb");
    if (!fp)
    {
        printf("\nError creating file!\n");
    }

    size_t bw = fwrite(mempak_data, MEMPAK_BLOCK_SIZE, 128, fp);
    printf("Number of elements written: %i\n", bw);

    fclose(fp);

    debug_close_sdfs();
}

void read_mempak_from_file(int controller_number, char device[])
{
    dfs_init(DFS_DEFAULT_LOCATION);

    

    /* This will initialize the SD filesystem using 'sd:/' to identify it */
    /* This path has to be the same used by the sprites when loading */

    char o_path[156];

    sprintf(o_path, "%s:/MEMPAKS/gold.mpk",device);

    // FILE *fp = fopen("rom:/MEMPAKS/gold.mpk", "r");
    FILE *fp = fopen(o_path, "r");
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

    char *device = malloc(4);

        if (!debug_init_sdfs("sd:/", -1) && !dev)
    {
        printf("Error opening SD!\n");
        return;
    } 
    else if (!debug_init_sdfs("sd:/", -1) && dev)
    {
        printf("Mounting ROM...\n");
        device = "rom";
    }
    else
    {
        device = "sd";
    }

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
        printf("Data loaded into RAM!\n\n");

        write_mempak_to_file(controller_number, device);
    }
}

void mempak_copy_from_file(int controller_number)
{
    console_clear();

    char *device = malloc(4);

        if (!debug_init_sdfs("sd:/", -1) && !dev)
    {
        printf("Error opening SD!\n");
        return;
    } 
    else if (!debug_init_sdfs("sd:/", -1) && dev)
    {
        printf("Mounting ROM...\n");
        device = "rom";
    }
    else
    {
        device = "sd";
    }
    
    read_mempak_from_file(controller_number, device);

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