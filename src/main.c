#include <stdio.h>
#include <uuid/uuid.h>
#include <stdlib.h>

#include "accounts.h"
#include "transfers.h"

int main() {

	const char *account_1_name = "Santa's Treasure";
	const char *account_2_name = "The Grinch's Funds";
	const int64_t new_balance = 1000;
	const int64_t new_balance_2 = 337;
	
	char account_1_uuid_str[37];
	uuid_t account_1_uuid;

	char account_2_uuid_str[37];
	uuid_t account_2_uuid;



	printf("Making an account with the name %s\n", account_1_name);
	add_account(&account_1_uuid, account_1_name);
	uuid_unparse(account_1_uuid, account_1_uuid_str);
	printf("\nIts UUID is: %s\n", account_1_uuid_str);

	printf("Making another account with the name %s\n", account_2_name);
	add_account(&account_2_uuid, account_2_name);
	uuid_unparse(account_2_uuid, account_2_uuid_str);
	printf("\nIts UUID is: %s\n", account_2_uuid_str);

	printf("Current account 1 balance: %ld\n", get_account_balance(account_1_uuid));
	printf("Changing balance to %ld\n", new_balance);
	set_account_balance(account_1_uuid, new_balance);
	printf("New account 1 balance: %ld\n", get_account_balance(account_1_uuid));

	printf("Current account 2 balance: %ld\n", get_account_balance(account_2_uuid));
	printf("Changing balance to %ld\n", new_balance_2);
	set_account_balance(account_2_uuid, new_balance_2);
	printf("New account 2 balance: %ld\n", get_account_balance(account_2_uuid));

	make_transfer(account_1_uuid, account_2_uuid, "Peace offering", get_account_balance(account_1_uuid));
	
	printf("Balances post-transfer: A1: %ld; A2: %ld\n", get_account_balance(account_1_uuid), get_account_balance(account_2_uuid));

	free_accounts();
	free_transfer_list();
	return 0;
}
