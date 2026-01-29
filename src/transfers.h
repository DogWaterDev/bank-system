#define TRANSFERS_H
#pragma once

#define MAX_TRANSFER_REASON_LENGTH 127

#include <stddef.h>
#include <uuid/uuid.h>
#include <time.h>

typedef struct {
	char reason[MAX_TRANSFER_REASON_LENGTH+1];
	int64_t amount;
	time_t time;
	uuid_t uuid;
	uuid_t sender_uuid;
	uuid_t receiver_uuid;
} transfer_t;

int make_transfer(uuid_t sender_uuid, uuid_t receiver_uuid, const char *reason, int64_t amount);
void free_transfer_list(void);
transfer_t get_transfer_properties(uuid_t transfer_uuid);
