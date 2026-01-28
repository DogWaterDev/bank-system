#define MENU_H
#pragma once

#include "../ui-shared/menu_actions.h"

struct MenuItem;

struct MenuItem {
	char *display_text;
	ActionFunction action;	
	int *submenus;
	size_t id;
	size_t submenu_count;
};

void display_menus(void);

//struct MenuItem* create_menu_item(const char *display_text, void* (*action)(void*, void*, void*, void*), struct MenuItem **submenus);
void create_menus(void);
void free_menu_items_from_root(struct MenuItem *root_menu);
