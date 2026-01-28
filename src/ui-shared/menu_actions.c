#include <string.h>
#include <stdio.h>
#include <stddef.h>

#include "menu_actions.h"

void *testfunction_one(void **args) {
	printf("Message from testfunction_one\n");
	return NULL;
}

struct ActionMap action_map[] = {
	{"testfunction_one", testfunction_one},
};

ActionFunction get_action_function(const char *action_name) {
	for (size_t i = 0; i < sizeof(action_map) / sizeof(action_map[0]); i++) {
		if (strcmp(action_name, action_map[i].name) == 0) {
			return action_map[i].function;
		}
	}
	return NULL;
}
