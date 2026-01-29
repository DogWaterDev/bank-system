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

static transfer_t *get_transfer(uuid_t transfer_uuid) {
	for (size_t i = 0; i < transfer_count; i++) {
		if (memcmp(transfer_uuid, transfer_list[i].uuid, sizeof(uuid_t)) == 0) {
			return &(transfer_list[i]);
		}
	}
	return NULL;
}

static int verify_transfer(uuid_t transfer_uuid) {
	if (get_account_balance(get_transfer(transfer_uuid)->sender_uuid) < get_transfer(transfer_uuid)->amount) {
		return -1;
	}
	return 0;
}


transfer_t get_transfer_properties(uuid_t transfer_uuid) {
	return *get_transfer(transfer_uuid);
}

int make_transfer(uuid_t sender, uuid_t receiver, const char *reason, int64_t amount) {
	if (transfer_count >= transfer_list_size) {
		grow_transfers();
	}

	if (uuid_is_null(sender) || uuid_is_null(receiver)) {
		printf("Sender or Receiver == NULL pointer. Aborting make_transfer()\n");
		return -1;
	}
	transfer_t *tmp = &(transfer_list[transfer_count]);


	memcpy(tmp->sender_uuid, sender, sizeof(uuid_t));
	memcpy(tmp->receiver_uuid, receiver, sizeof(uuid_t));
	strncpy(tmp->reason, reason, MAX_TRANSFER_REASON_LENGTH);
	tmp->reason[MAX_TRANSFER_REASON_LENGTH] = '\0';
	tmp->amount = amount;
	uuid_generate_random(tmp->uuid);
	transfer_list[transfer_count++] = *tmp;
	

	if (verify_transfer(tmp->uuid) != 0) { // this is here due to me not finding a better solution
		transfer_count--; //allow this to not be read and instead overwritten

		return -1;
	}
	set_account_balance(sender, get_account_balance(sender)-amount);
	set_account_balance(receiver, get_account_balance(receiver)+amount);

	return 0;
}

void free_transfer_list(void) {
	free(transfer_list);
}
