#define ACCOUNTS_H
#pragma once
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <uuid/uuid.h>

#define ACCOUNT_NAME_LIMIT 64

typedef struct {
	uint64_t balance;
	uuid_t uuid;
	uuid_t owner_uuid;
	int8_t frozen;
	int8_t negative;
	char name[];
} account_t;

void add_account(uuid_t *account_uuid_container, const char *name);
void remove_account(uuid_t account_uuid);

uint64_t get_account_balance(uuid_t account_uuid);
void get_account_owner_uuid(uuid_t *owner_uuid_container, uuid_t account_uuid);
int get_account_frozen(uuid_t account_uuid);
int get_account_negative(uuid_t account_uuid); // Negative refers to whether or not the account is in debt as a uint64_t is used to store larger numbers at the price of not storing negative ones.

int set_account_balance(uuid_t account_uuid, uint64_t balance);
int set_account_owner_uuid(uuid_t account_uuid, uuid_t owner_uuid);
int set_account_frozen(uuid_t account_uuid, int negative);
int set_account_negative(uuid_t account_uuid, int negative);

void free_accounts(void);
