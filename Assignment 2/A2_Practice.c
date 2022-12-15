#include <stdio.h>
#include <stdlib.h>

struct node {
  int value;
  struct node* next;
};

struct node* add_to_list(int val, struct node *head) {
	struct node* newNode = malloc(sizeof(struct node));
	newNode->value = val;
	newNode->next = NULL;

	if(head == NULL) {
		head = newNode;
	}

	else {
		struct node *temp = head;
		
		while(temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = newNode;
	}

	return head;
}

void print_list(struct node* head) {
	while(head!= NULL) {
		printf("%d ", head->value);

		head = head-> next;
	}
	
}

int main() {
	struct node *head = NULL;
	int n, val;

	scanf("%d", &n);

	for(int i=0; i<n; i++) {
		scanf("%d", &val);
		head = add_to_list(val, head);
	}

	print_list(head);
}