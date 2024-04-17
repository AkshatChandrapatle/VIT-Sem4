#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define size 4

int board[size][size];
int score = 0;
int add_new_2_4(){
    int r= rand()%size;
    int c=rand()%size;
    while (board[r][c]!=0){
        r= rand()%size;
        c=rand()%size;
    }
    board[r][c]=2;
}

void save(){
    	char name[50],c,d;

	FILE *fptr;
	fptr=(fopen("sankerecord2.txt","a+"));
	if(fptr==NULL) {
		printf("Error!");
		exit(1);
	}

		printf("\n\nenter the name for score %d\nEnter name: ",score);
		scanf("%s",name);

		fprintf(fptr,"\nName: %s \nscore:  %d \n",name,score);
	fclose(fptr);
	      system("cls");
	      fflush(stdin);
         printf("for previous score press y:");
         scanf("%c",&d);

         if(d=='y'){
                fptr=(fopen("sankerecord2.txt","r"));
                while (c!=EOF)      {

                 printf("%c",c);
       c=fgetc(fptr);

            }
   fclose(fptr);
   }
    }
void start_game(){
    for(int i=0;i<size;i++){
        for (int j = 0; j <size; j++){
            board[i][j]=0;
        }
    }
    add_new_2_4();
}

int get_current_state(){
    // 1-->won  2-->game not over   3-->game over(Lost)
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(board[i][j]== 2048){
                // printf("WON\n");
                return 1;
            }
        }
    }
    // checking if empty place available or not
    for(int m=0;m<size;m++){
        for(int n=0;n<size;n++){
            if(board[m][n] == 0){
                return 2;
            }
        }
    }
//Every row and coloumn except last row and last colomn[checking whether they can combine and leave 0]
    for(int i=0;i<size-1;i++){
        for(int j=0;j<size-1;j++){
            if(board[i][j]==board[i+1][j] || board[i][j]==board[i][j+1]){
                return 2;
            }
        }
    }// for last row
    for(int j=0;j<size-1;j++){
        if(board[size-1][j]==board[size-1][j+1]){
            return 2;
        }
    }// for last colomn
    for(int i=0;i<size-1;i++){
        if(board[i][size-1]==board[i+1][size-1]){
            return 2;
        }
    }

// if neither of the above returns any means board is full and player is lost
return 3;
}

// merging two blocks if they are same
void merge(){
    for(int i=0;i<size;i++){
        for(int j=0;j<size-1;j++){
            if(board[i][j]==board[i][j+1] && board[i][j]!=0){
                board[i][j]*=2;
                score += board[i][j];
                board[i][j+1]=0;
            }
        }
    }
}
//moving all zeroes of a row to one side
void compress(){
    int a[size][size];
    for(int i=0;i<size;i++){
        for (int j = 0; j <size; j++){
            a[i][j]=0;
        }
    }

    for(int i=0;i<size;i++){
        int pos=0;
        for(int j=0;j<size;j++){
            if(board[i][j]!=0){
                a[i][pos]=board[i][j];
                pos++;
            }
        }
    }

    for(int i=0;i<size;i++){
        for (int j = 0; j <size; j++){
            board[i][j]=a[i][j];
        }
    }
}

void reverse(){
    for(int i=0;i<size;i++){
        int s=0,e=size-1;
        while(s<=e){
            int temp=board[i][s];
            board[i][s]=board[i][e];
            board[i][e]=temp;
            // swap(board[i][s++],board[i][e--]);
            s++;
            e--;
        }
    }
}

void transpose(){
    int a[size][size];
    for (int i = 0;i < size;i++){
        for (int j = 0; j < size; j++){
            a[j][i] = board[i][j];
        }
    }

    for(int i=0;i<size;i++){
        for (int j = 0; j <size; j++){
            board[i][j]=a[i][j];
        }
    }
}

void move_left(){
    int change[size][size];
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            change[i][j]=board[i][j];
        }
    }
    compress();
    merge();
    compress();
    int k=0;
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(change[i][j]!=board[i][j]){
                k=1;
            }
        }
    }
    if(k){
        add_new_2_4();
    }
}

void move_right(){
    int change[size][size];
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            change[i][j]=board[i][j];
        }
    }
    reverse();
    compress();
    merge();
    compress();
    reverse();
    int k=0;
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(change[i][j]!=board[i][j]){
                k=1;
            }
        }
    }
    if(k){
        add_new_2_4();
    }
}

void move_up(){
    int change[size][size];
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            change[i][j]=board[i][j];
        }
    }
    transpose();
    compress();
    merge();
    compress();
    transpose();
    int k=0;
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(change[i][j]!=board[i][j]){
                k=1;
            }
        }
    }
    if(k){
        add_new_2_4();
    }
}

void move_down(){
    int change[size][size];
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            change[i][j]=board[i][j];
        }
    }
    transpose();
    reverse();
    compress();
    merge();
    compress();
    reverse();
    transpose();
    int k=0;
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(change[i][j]!=board[i][j]){
                k=1;
            }
        }
    }
    if(k){
        add_new_2_4();
    }
}

/*void printUI(){
    system("cls");
    for(int i=0;i<size;i++){
    printf("|");
        for(int j=0;j<size;j++){
            if(board[i][j]==0){
                printf(" |");
            }
            else{
                printf("%d|",board[i][j]);
            }
        }
    printf("\n");
    printf("---------\n");
    }

    printf("Score :%d\n",score);
    printf("n: new game, w: up, s: down, a: left, d: right, q:quit\n");
}*/

void printUI() {
    system("cls");
    printf("2048 Game\n");
    printf("Score: %d\n\n", score);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("+-----");
        }
        printf("+\n");

        for (int j = 0; j < size; j++) {
            if (board[i][j] == 0) {
                printf("|     ");
            } else {
                printf("|%4d ", board[i][j]);
            }
        }
        printf("|\n");
    }

    for (int j = 0; j < size; j++) {
        printf("+-----");
    }
    printf("+\n");

    printf("n: new game, w: up, s: down, a: left, d: right, q: quit\n");
}


int main(){
    start_game();
    char command;
    while (1){
        printUI();
        scanf("%c",&command);
        if(get_current_state()==1){
            printf("You Won\n");
            break;
        }
        else if(get_current_state()==3){
            printf("Block is Full\nTry next time\n");
            break;
        }
        else if(command=='n'){
            start_game();
        }
        else if(command=='q'){
            break;
        }
        else if(command=='s'){
            move_down();
        }
        else if(command=='a'){
            move_left();
        }
        else if(command=='d'){
            move_right();
        }
        else if(command=='w'){
            move_up();
        }

    }
    char c1;
fflush(stdin);
printf("if you want to save score enter y if not then n: ");
scanf("%c",&c1);
        if (c1 == 'y')
        {
               save();
        }

    return 0;
}
