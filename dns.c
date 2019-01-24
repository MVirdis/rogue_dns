#include <arpa/inet.h>
#include <sys/types.h>

#include "dns.h"

uint16_t getID(struct dns_header_t* header) {
	uint16_t id = ntohs(header->id);
	return id;
}

int getNumQuest(struct dns_header_t* header) {
	return ntohs(header->num_quest);
}

int getNumAnsw(struct dns_header_t* header) {
	return ntohs(header->num_answ);
}

int isResponse(struct dns_header_t* header) {
	uint16_t flags = ntohs(header->flags);
	if (flags & (uint16_t)(1 << 15)) return 1;
	else return 0;
}

int isRecursiveDesired(struct dns_header_t* header) {
	uint16_t flags = ntohs(header->flags);
	if (flags & (uint16_t)(1 << 8)) return 1;
	else return 0;
}

int isRecursiveAvailable(struct dns_header_t* header) {
	uint16_t flags = ntohs(header->flags);
	if (flags & (uint16_t)(1 << 7)) return 1;
	else return 0;
}

char* getQuests(char* dns_message, struct dns_header_t* header) {
	int num = getNumQuest(header);
	return dns_message + 12;
}

