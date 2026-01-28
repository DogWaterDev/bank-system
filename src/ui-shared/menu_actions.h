#define MENU_ACTIONS_H
#pragma once

typedef void* (*ActionFunction)(void**);

struct ActionMap{
    const char *name;
    ActionFunction function;
};

ActionFunction get_action_function(const char *action_name);
