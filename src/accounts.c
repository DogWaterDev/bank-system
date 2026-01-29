#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>
#include "accounts.h"

static account_t *account_list;
static size_t account_count = 0;
static size_t account_list_size = 0;

static void grow_owners(void) {
	account_list_size = account_list_size == 0 ? 8 : account_list_size * 2;
account_list = realloc(account_list, account_list_size * sizeof(account_t));

	}

static int get_account_index(uuid_t account_uuid) {
	for (size_t i = 0; i < account_count; i++) {
		if (memcmp(account_list[i].uuid, account_uuid, sizeof(uuid_t)) == 0) {
			return (int)i;
		}
	}
	return (-1);
}

void add_account(uuid_t *account_uuid_container, const char *name) {
	if (account_count+1 > account_list_size) {
		grow_owners();
	}

	account_t *tmp = &(account_list[account_count]);
	
	tmp->balance = 0;
	uuid_generate_random(tmp->uuid);
	tmp->frozen = 0;
	strncpy(tmp->name, name, ACCOUNT_NAME_LIMIT-1);
	tmp->name[ACCOUNT_NAME_LIMIT-1] = '\0';
	memcpy(*account_uuid_container, tmp->uuid, sizeof(tmp->uuid));
	account_count++;
}

int64_t get_account_balance(uuid_t account_uuid) {
	int index = get_account_index(account_uuid);
	if (index != -1) {
		return account_list[index].balance;
	}
	return INT64_MIN; // if someone reaches this amount normally just fuck them ngl
}

int set_account_balance(uuid_t account_uuid, int64_t balance) {
	int index = get_account_index(account_uuid);
	if (index != -1) {
		account_list[index].balance = balance;
		return 0;
	}
	return -1;
}

void get_account_owner_uuid(uuid_t *owner_uuid_container, uuid_t account_uuid) {
	int index = get_account_index(account_uuid);
	if (index == -1) {
		return;
	}
	memcpy(*owner_uuid_container, account_list[index].owner_uuid, sizeof(uuid_t));
}

int set_account_owner_uuid(uuid_t account_uuid, uuid_t owner_uuid) {
	int index = get_account_index(account_uuid);
	if (index == -1) {
		return -1;
	}
	memcpy(account_list[index].owner_uuid, owner_uuid, sizeof(uuid_t));
	return 0;
}

int get_account_frozen(uuid_t account_uuid) {
	int index = get_account_index(account_uuid);
	if (index != -1) {
		return account_list[index].frozen;	
	}
	return -1;
}

int set_account_frozen(uuid_t account_uuid, int frozen) {
	int index = get_account_index(account_uuid);
	if (index != -1) {
		account_list[index].frozen = frozen;
		return 0;
	}
	return -1;
}

void free_accounts(void) {
/*	for (size_t i = 0; i < account_count; i++) {
		free(account_list[i].uuid);
	}
*/
	free(account_list);
}


