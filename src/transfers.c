#include <time.h>
#include <uuid/uuid.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "accounts.h"
#include "transfers.h"

transfer_t *transfer_list;
size_t transfer_list_size = 0;
size_t transfer_count = 0;

static void grow_transfers(void) {
	transfer_list_size = transfer_list_size == 0 ? 8 : transfer_list_size * 2;
	transfer_list = realloc(transfer_list, transfer_list_size * sizeof(transfer_t));
	if (transfer_list == NULL) {
		printf("Unable to allocate memory for transfer.\nOUT OF MEMORY?\nAborting.\n");
		exit(EXIT_FAILURE);
	}
}

int make_transfer(uuid_t sender, uuid_t receiver, const char *reason, uint64_t amount) {
	if (transfer_count == transfer_list_size-1) {
		grow_transfers();
	}
	transfer_t *tmp = &(transfer_list[transfer_count == 0 ? 0 : transfer_count-1]);
	
	memcpy(tmp->sender_uuid, sender, sizeof(uuid_t));
	memcpy(tmp->receiver_uuid, receiver, sizeof(uuid_t));
	strncpy(tmp->reason, reason, MAX_TRANSFER_REASON_LENGTH);
	tmp->reason[MAX_TRANSFER_REASON_LENGTH] = '\0';
	tmp->amount = amount;
	transfer_count++;

	return 1;
}

int verify_transfer(uuid_t transfer_uuid) {
	uuid_t temporarily_hide_warning_uuid_lol;
	memcpy(temporarily_hide_warning_uuid_lol, transfer_uuid, sizeof(uuid_t));
	return 0;
}
