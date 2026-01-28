#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "menu.h"
#include "../ui-shared/menu_actions.h"

static const size_t KEYWORD_COUNT = 4;
static const char *KEYWORDS[] = {"ID","SUBMENU", "VALUE", "FUNCTION"};
static const size_t MAX_ENTRY_LENGTH = 64;
static const size_t MAX_SUBMENU_COUNT = 8;


static size_t menu_item_count = 0;

static struct MenuItem *menus;


static struct MenuItem* create_menu_item(int id, const char *display_text, ActionFunction action, int *submenus) {
    	struct MenuItem *item = malloc(sizeof(struct MenuItem));
    	if (item == NULL) {
        	printf("Unable to allocate memory for MenuItem.\nAborting...\n");
        	exit(EXIT_FAILURE);
    	}
	
	item->display_text = malloc(strlen(display_text) + 1);
    	if (item->display_text == NULL) {
        	printf("Unable to allocate memory for display_text.\nAborting...\n");
        	free(item);
        	exit(EXIT_FAILURE);
    	}
    	strcpy(item->display_text, display_text);
	
	item->action = action;
    	item->submenus = submenus;
    	item->id = id;
	menu_item_count++;
	printf("menu item count: %ld\n", menu_item_count);
	return item;
}

static struct MenuItem *get_menu_from_id(int id) {
	for (size_t i = 0; i < menu_item_count; i++) {
		if ((int) menus[i].id == id) {
			return &menus[i];
		}
	}
	return NULL;
}

static int validate_integer(const char *str) {
	if (*str == '\0') return EXIT_FAILURE; // i prefer this over 0 or 1 because it's not up to how the coder feels what is what lol, more readable

	while (*str){
		if (!isdigit(*str)) return 0;
		str++;
	}
	return EXIT_SUCCESS;
}

static int validate_keyword(const char *keyword) {
	for (size_t i = 0; i < KEYWORD_COUNT; i++) {
		if (strcmp(keyword, KEYWORDS[i]) == 0) {
			return EXIT_SUCCESS;
		}
	}
	return EXIT_FAILURE;
}

static void parse_line(char *keyword_container, char *value_container, int *submenu_ids_container, int *submenu_count_container, int *menu_id_container, char *action_function_name_container, const char *original_line) {

	char *token;
	char keyword[MAX_ENTRY_LENGTH];
	char value[MAX_ENTRY_LENGTH];

	int *submenu_ids = malloc(sizeof(int) * MAX_SUBMENU_COUNT);;
	int submenu_count = 0;
	char *line = strdup(original_line);

	token = strtok(line, "=");
	if (token != NULL) {
		strncpy(keyword, token, MAX_ENTRY_LENGTH-1);
		keyword[MAX_ENTRY_LENGTH-1] = '\0';
	}
	if (validate_keyword(keyword) == EXIT_FAILURE) {
		return;
	}
	token = strtok(NULL, "=");
	if (token != NULL) {
		strncpy(value, token, MAX_ENTRY_LENGTH-1);
		value[MAX_ENTRY_LENGTH-1] = '\0';
	}
	token = strtok(value, ";");
	while (token != NULL) {
		if (validate_integer(token) == EXIT_SUCCESS) {
			submenu_ids[submenu_count++] = atoi(token);
		}
		token = strtok(NULL, ";");
	}
	
	if (validate_integer(value) == EXIT_SUCCESS) {
		*menu_id_container = atoi(value);
	}
	else if (submenu_count != 0) {
		for (size_t i = 0; i < (size_t)submenu_count && i < MAX_SUBMENU_COUNT; i++) {
			submenu_ids_container[i] = submenu_ids[i];
		}
		*submenu_count_container = submenu_count;
	}
	else if (strcmp(keyword, "FUNCTION") == 0 && (size_t)strlen(value) < MAX_ENTRY_LENGTH) {
		strcpy(action_function_name_container, value);
	}
	strcpy(keyword_container, keyword);
	strcpy(value_container, value);


	free(line);
	free(submenu_ids);
}


void free_menu_item_from_root(struct MenuItem *root_menu) {
    if (root_menu == NULL) return;

    
    for (size_t i = 0; i < root_menu->submenu_count; i++) {
        	free_menu_item_from_root(get_menu_from_id(root_menu->submenus[i])); // Recursively free submenus (i <3 recursion)
    }

    
    free(root_menu->display_text);
    free(root_menu);
    menu_item_count--;
}

void create_menus(void) {
	char buffer[MAX_ENTRY_LENGTH*2];

	FILE *file = fopen("../resources/tui/menu_dialog", "r");
	
	if (file == NULL) {
		printf("Unable to open menu file\n");
		return;
	}
	int menu_id = -1;
	char *menu_text = malloc(sizeof(char) * MAX_ENTRY_LENGTH);
	char *action_function_name = malloc(sizeof(char) * MAX_ENTRY_LENGTH);
	int *submenus = malloc(sizeof(int) * MAX_SUBMENU_COUNT);
	int submenu_count = 0;
	
	int menu_count = 0;
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		char *token = strtok(buffer, "=");
		if (strcmp(token, "ID") == 0) { menu_count++; }
	}
	menus = malloc(menu_count * sizeof(struct MenuItem));

	rewind(file);
	while (fgets(buffer, sizeof(buffer), file) != NULL) {

		if (menu_id != -1 && submenu_count > 0 && submenus != NULL && 
    			strlen(menu_text) > 0 && strlen(action_function_name) > 0) {
    
    			create_menu_item(menu_id, menu_text, get_action_function(action_function_name), submenus);
    
    			menu_id = -1;
    			submenu_count = 0;
    			memset(submenus, 0, sizeof(int) * MAX_SUBMENU_COUNT);
    			strcpy(menu_text, ""); 
    			strcpy(action_function_name, ""); 

		}


		char *line_keyword = malloc(sizeof(char) * MAX_ENTRY_LENGTH);
		char *line_value = malloc(sizeof(char) * MAX_ENTRY_LENGTH);
		int *line_submenus =  malloc(sizeof(int) * MAX_SUBMENU_COUNT);
		int line_submenu_count = 0;
		int line_menu_id = 0;
		char *line_action_function_name = malloc(sizeof(char) * MAX_ENTRY_LENGTH);

		parse_line(line_keyword, line_value, line_submenus, &line_submenu_count, &line_menu_id, line_action_function_name, buffer);


		if (strcmp(line_keyword, "ID") == 0) {
			menu_id = line_menu_id;
		}
		else if (line_submenu_count > 0 && strcmp(line_keyword, "SUBMENU") == 0) {
			memcpy(submenus, line_submenus, line_submenu_count * sizeof(int));
		}
		else if (strcmp(line_keyword, "VALUE") == 0) {
			strcpy(menu_text, line_value);
			menu_text[MAX_ENTRY_LENGTH-1] = '\0';
			printf("menu_text: '%s'\n", menu_text);
		}
		else if (strcmp(line_keyword, "FUNCTION") == 0) {
			strcpy(action_function_name, line_value);
			action_function_name[MAX_ENTRY_LENGTH-1] = '\0';
			printf("action_function_name: '%s'\n", action_function_name);
		}
		free(line_keyword);
		free(line_value);
		free(line_action_function_name);
		free(line_submenus);
	}
	free(menu_text);
	free(action_function_name);
	free(submenus);
	fclose(file);
}

void display_menus(void) {
	for (size_t i = 0; i < menu_item_count; i++) {
		printf("%s\n", menus[i].display_text);
	}
}
