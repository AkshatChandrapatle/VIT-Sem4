#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *ptr;
} *temp , *new;


struct Node* insert(struct Node *first, int x){
    if(first==NULL){
        new = (struct Node*)malloc(sizeof(struct Node));
        new->data=x;
        new->ptr=NULL;
        return new;
    }
    else{
        new = (struct Node*)malloc(sizeof(struct Node));
        new->data=x;
        new->ptr=NULL;
        temp=first;
        while(temp->ptr!=NULL){
            temp=temp->ptr;
        }
        temp->ptr=new;
        return first;
    }
}

struct Node *append(struct Node *first, struct Node *second){
    temp=first;
    while(temp->ptr!=NULL){
        temp=temp->ptr;
    }
    temp->ptr=second;
    return first;

}

void printList(struct Node* first){

    temp=first;
    while(temp!=NULL){
        printf("%d %p\t",temp->data ,temp->ptr);
        temp=temp->ptr;
    }
    printf("\n");
}

int main()
{
    struct Node *a = (struct Node*)malloc(sizeof(struct Node));
    a->data = 10;
    a->ptr = NULL;

    a = insert(a,20);
    a = insert(a,30);
    printf("Address of a is :%p \n",a);
    printList(a);

    struct Node *b = (struct Node*)malloc(sizeof(struct Node));
    b->data = 11;
    b->ptr = NULL;

    b = insert(b,22);
    b = insert(b,33);
    printf("\nAddress of b is :%p \n",b);
    printList(b);

    a = append(a,b);
    printf("\nAfter appending a and b\n");
    printList(a);
    b=NULL;
    printList(b);
    return 0;
}

