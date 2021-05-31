#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
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
	info->geoWidth = info->geoWidth * 3.14 / 180;
	fgetc(in);
	fscanf(in, "%lf", &info->geoHeight);
	info->geoHeight = info->geoHeight * 3.14 / 180;
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

struct linkedlistNode* createLinkedList(char *filename) {
	FILE* in = fopen(filename, "r");
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

double distance(struct linkedListNode* node1, struct linkedListNode* node2) {
	int r = 6371;

	double part1 = (node1->info->geoWidth - node2->info->geoWidth) / 2;
	double part2 = (node1->info->geoHeight - node2->info->geoHeight) / 2;

	double t1 = sin(part1) * sin(part1);
	double t2 = sin(part2) * sin(part2);

	double part3 = asin(sqrt(t1 + t2 * cos(node1->info->geoWidth) * cos(node2->info->geoWidth)));

	double d = 2.0 * r * part3;

	return d;

}

void printLinkedlist(struct linkedListNode* head) {
	struct linkedListNode* p = head;
	while (p != NULL) {
		printf("%d =>", p->info->stopNumber);
		p = p->next;
	}
}

void writetoFile(char *filename, struct linkedListNode *begin) {
	FILE* write = fopen(filename, "w");
	double Alldistance = 0.0, temp;
	struct linkedListNode* p = begin;
	struct linkedListNode* s = p->next;
	while (s != NULL) {
		temp = distance(p, s);
		Alldistance += temp;
		fprintf(write, "%s!%s!%.2lf\n", p->info->name, s->info->name, temp);
		p = p->next;
		s = p->next;
	}
	fprintf(write, "%.2lf", Alldistance);
	fclose(write);
}

void freeList(struct linkedListNode* head) {
	struct linkedListNode* p = head;
	struct linkedListNode* temp = head;
	while (p != NULL) {
		temp = p;
		p = p->next;
		free(temp->info);
		free(temp);
	}
}
//free
//2_dirA_distance.txt 5_dirA_distance.txt
//dirA 2 5 7 10
int main() {
	char* name = malloc(sizeof(char) * 5);
	scanf("%s", name);
	char k = getchar();

	char *number = malloc(sizeof(char));
	char* filename = NULL;
	char temp;
	int i = 0, numSize = 1;
	char* outfile = NULL;

	while ((temp = getchar()) != '\n') {
		if (temp == ' ') {
			char* filename = malloc(sizeof(char) * 50);
			char* outfile = malloc(sizeof(char) * 50);
			filename[0] = '\0';
			outfile[0] = '\0';
			strcat(filename, number);
			strcat(filename, "_\0");
			strcat(filename, name);
			strcat(filename, ".txt");
			strcat(outfile, number);
			strcat(outfile, "_\0");
			strcat(outfile, name);
			strcat(outfile, "_distance.txt");

			struct linkedListNode* begin = createLinkedList(filename);
			writetoFile(outfile, begin);

			freeList(begin);
			free(outfile);
			free(filename);
			free(number);
			number = malloc(sizeof(char));
			numSize = 1;
			continue;
		}
		number = realloc(number, sizeof(char) * (numSize + 1));
		number[numSize - 1] = temp;
		number[numSize++] = '\0';
	}

	filename = malloc(sizeof(char) * 50);
	outfile = malloc(sizeof(char) * 50);
	filename[0] = '\0';
	outfile[0] = '\0';
	strcat(filename, number);
	strcat(filename, "_\0");
	strcat(filename, name);
	strcat(filename, ".txt");
	strcat(outfile, number);
	strcat(outfile, "_\0");
	strcat(outfile, name);
	strcat(outfile, "_distance.txt");

	struct linkedListNode* begin = createLinkedList(filename);
	writetoFile(outfile, begin);

	freeList(begin);
	free(outfile);
	free(filename);
	free(number);

	return 0;
}