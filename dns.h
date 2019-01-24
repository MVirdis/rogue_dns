struct dns_header_t {
	uint16_t id;
	uint16_t flags;
	uint16_t num_quest;
	uint16_t num_answ;
	uint16_t num_auth;
	uint16_t num_addit;
};

uint16_t getID(struct dns_header_t* header);
int getNumQuest(struct dns_header_t* header);
int getNumAnsw(struct dns_header_t* header);
int isResponse(struct dns_header_t* header);
int isRecursiveDesired(struct dns_header_t* header);
int isRecursiveAvailable(struct dns_header_t* header);
char* getQuests(char* dns_message, struct dns_header_t* header);
