#ifndef MPK_HW_H
#define MPK_HW_H

#include <libdragon.h>

void view_pak_conents(int controller_number);
void mempak_format(int controller_number);
void write_mempak_to_file(int controller_number, char device[]);
void read_mempak_from_file(int controller_number, char device[]);
void mempak_copy_to_file(int controller_number);
void mempak_copy_from_file(int controller_number);

#endif
