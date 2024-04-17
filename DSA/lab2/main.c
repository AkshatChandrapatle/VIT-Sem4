#include <stdio.h>
#include <stdlib.h>

struct Node {
	int data;
	struct Node* next;
};


void printList(struct Node* n)
{
	while (n != NULL) {
		printf(" %d ", n->data);
		n = n->next;
	}
}

void Append(struct Node *head1, struct Node *head2)
{
    struct Node *p = head1;
    while (p->next != NULL)
    {
        p = p->next;
    }
    p->next = head2;
    head2=NULL;
}


int main()
{
	struct Node* head1;
	struct Node* one1 = NULL;
	struct Node* second1 = NULL;
	struct Node* third1 = NULL;


	one1 = (struct Node*)malloc(sizeof(struct Node));
	second1 = (struct Node*)malloc(sizeof(struct Node));
	third1 = (struct Node*)malloc(sizeof(struct Node));

	one1->data = 1;
	one1->next = second1;

	second1->data = 2;
	second1->next = third1;

	third1->data = 3;
	third1->next = NULL;

	head1=one1;

    printf("List 1 \n");
	printList(head1);

	struct Node* head2;
	struct Node* one2 = NULL;
	struct Node* second2 = NULL;
	struct Node* third2 = NULL;


	one2 = (struct Node*)malloc(sizeof(struct Node));
	second2 = (struct Node*)malloc(sizeof(struct Node));
	third2 = (struct Node*)malloc(sizeof(struct Node));

	one2->data = 7;
	one2->next = second2;

	second2->data = 8;
	second2->next = third2;

	third2->data = 9;
	third2->next = NULL;

	head2=one2;

	printf("\n");
	printf("List 2 \n");
	printList(head2);

	Append(head1,head2);
	printf("\n After appending two list \n");
	printList(head1);

	printf("\nSet list2 as null\n");
	printList(head2);

	return 0;
}
