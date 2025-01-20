#ifndef MENU_H
#define MENU_H


#include <libdragon.h>

void show_main_menu();
int main_menu_prompt();
void menu_processor(void (*action)(int i), int i);

#endif