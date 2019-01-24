#include <sys/socket.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

#include "dns.h"

#define DEFAULT_DNS_PORT 53
#define MAX_DNS_MESSAGE_LEN 128
#define REAL_DNS_IP "8.8.8.8"
#define STAT_STRING "\rRequests received %d, answered %d"
#define DEFAULT_INTERFACE "eth0"

int main(int argc, char** argv) {

	char* interface = (argc > 1 ? argv[0] : DEFAULT_INTERFACE);
	char dns_message[MAX_DNS_MESSAGE_LEN];
	struct dns_header_t dns_header;
	socklen_t ssize = sizeof(struct sockaddr);

	struct sockaddr_in addr;
	struct sockaddr_in sender_addr;
	struct sockaddr_in real_dns_addr;
	memset(&addr, 0, sizeof(addr));
	memset(&real_dns_addr, 0, sizeof(real_dns_addr));
	real_dns_addr.sin_family = AF_INET;
	real_dns_addr.sin_port = htons(DEFAULT_DNS_PORT);
	inet_pton(AF_INET, REAL_DNS_IP, &real_dns_addr.sin_addr);

	// Little procedure to select my interface ip
	struct ifaddrs* ifap,* ifa;
	getifaddrs(&ifap);
	for(ifa = ifap; ifa; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr->sa_family == AF_INET && 
			strcmp(ifa->ifa_name, interface) == 0) {
			addr = *((struct sockaddr_in*) ifa->ifa_addr);
			break;
		}
	}
	addr.sin_port = htons(DEFAULT_DNS_PORT);
	printf("Current IP: %s\n", inet_ntoa(addr.sin_addr));
	freeifaddrs(ifap);

	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	bind(sock, (struct sockaddr*)&addr, sizeof(addr));

	int ssock = socket(AF_INET, SOCK_DGRAM, 0);
	connect(ssock, (struct sockaddr*)&real_dns_addr, sizeof(real_dns_addr));

	int recv_count = 0, sent_count = 0;

	while(1) {
		int rlen = recvfrom(sock, &dns_message, MAX_DNS_MESSAGE_LEN, 0,
				(struct sockaddr*)&sender_addr, &ssize);
		recv_count++;
		printf(STAT_STRING, recv_count, sent_count);
		fflush(stdout);
		memcpy(&dns_header, dns_message, 12);
		int slen = send(ssock, dns_message, rlen, 0);
		rlen = recv(ssock, dns_message, MAX_DNS_MESSAGE_LEN, 0);
		sendto(sock, dns_message, rlen, 0, (struct sockaddr*)&sender_addr, sizeof(sender_addr));
		sent_count++;
		printf(STAT_STRING, recv_count, sent_count);
		fflush(stdout);
	}

	return 0;
}

