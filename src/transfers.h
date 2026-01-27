#define TRANSFERS_H
#pragma once

#define MAX_TRANSFER_REASON_LENGTH 127

#include <stddef.h>
#include <uuid/uuid.h>
#include <time.h>

typedef struct {
	char reason[MAX_TRANSFER_REASON_LENGTH+1];
	uint64_t amount;
	time_t time;
	uuid_t uuid;
	uuid_t sender_uuid;
	uuid_t receiver_uuid;
} transfer_t;

int make_transfer(uuid_t sender_uuid, uuid_t receiver_uuid, const char *reason, uint64_t amount);
int verify_transfer(uuid_t transfer_uuid);
