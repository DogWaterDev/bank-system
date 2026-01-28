#include <stdio.h>
#include <uuid/uuid.h>
#include <stdlib.h>

#include "accounts.h"
#include "transfers.h"
#include "tui/menu.h"


int main() {

	const char *account_name = "Santa's Treasure";
	//const uint64_t new_balance = (uint64_t)1000;
	
	char account_uuid_str[37];
	uuid_t account_uuid;

	printf("Making an account with the name %s\n", account_name);
	add_account(&account_uuid, account_name);
	uuid_unparse(account_uuid, account_uuid_str);
	/*
	printf("\nIts UUID is: %s\n", account_uuid_str);
	
	printf("Current account balance: %ld\n", get_account_balance(account_uuid));
	printf("Changing balance to %ld\n", new_balance);
	set_account_balance(account_uuid, new_balance);
	printf("New account balance: %ld\n", get_account_balance(account_uuid));
	*/

	create_menus();
	display_menus();
	free_accounts();
	return 0;
}
