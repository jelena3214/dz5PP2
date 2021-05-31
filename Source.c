#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#define MAX_SIZE 256

typedef struct nodeInfo {
	int stopNumber;
	char name[MAX_SIZE];
	double geoWidth;
	double geoHeight;
	int zone;
}Nodeinfo;

struct linkedListNode {
	Nodeinfo *info;
	struct linkedListNode* prev;
	struct linkedListNode* next;
};

Nodeinfo *readInput(Nodeinfo* info, FILE *in) {
	char* name, c;
	
	fscanf(in, "%d", &info->stopNumber);
	fgetc(in);

	name = info->name;
	while ((c = fgetc(in)) != '!') {
		if (c == EOF)return NULL;
		*name++ = c;
	}
	*name = '\0';
		
	fscanf(in, "%lf", &info->geoWidth);
	fgetc(in);
	fscanf(in, "%lf", &info->geoHeight);
	fgetc(in);
	fscanf(in, "%d", &info->zone);
	fgetc(in);

	return info;
}

struct linkedListNode* createNode(Nodeinfo *info) {
	struct linkedListNode* newnode = malloc(sizeof(struct linkedListNode));
	newnode->info = info;
	newnode->next = NULL;
	newnode->prev = NULL;
}

struct linkedlistNode* createLinkedList() {
	FILE* in = fopen("ulaz.txt", "r");
	struct linkedListNode* head = NULL;
	struct linkedListNode* p = NULL;
	while (1) {
		Nodeinfo* info = malloc(sizeof(Nodeinfo));
		info = readInput(info, in);
		if (info == NULL)break;

		struct linkedListNode* newnode = createNode(info);
		if (head == NULL) {
			head = newnode;
			p = head;
			continue;
		}
		p->next = newnode;
		newnode->prev = p;
		p = newnode;
		
	}

	fclose(in);
	return head;
}

void printLinkedlist(struct linkedListNode* head) {
	struct linkedListNode* p = head;
	while (p != NULL) {
		printf("%d =>", p->info->stopNumber);
		p = p->next;
	}
}


int main() {
	struct linkedListNode* begin = createLinkedList();
	printLinkedlist(begin);
	return 0;
}