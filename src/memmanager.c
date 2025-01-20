#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>
#include "n64fs.h"
#include "menu.h"
#include "mpk_hw.h"


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
                    menu_processor(view_pak_conents,i);
                }
                else if (keys.c[i].B)
                {
                    menu_processor(mempak_format,i);
                }
                else if (keys.c[i].L)
                {
                    menu_processor(mempak_copy_to_file,i);
                }
                else if (keys.c[i].R)
                {
                    menu_processor(mempak_copy_from_file,i);
                }
            }
        }
    }
}
