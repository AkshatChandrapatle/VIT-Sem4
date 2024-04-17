#include <stdio.h>

struct Faculty {
    char faculty_name[30];
    int faculty_ID;
    char subject_codes[30];
    char class_names[30];
};

struct Faculty arr[5] = {
    {"Steve", 44, "physics101","class1"},
    {"Bob", 21, "maths101","class3"},
    {"Dustin", 33, "physics101", "class5"},
    {"Mike", 14, "history101", "class7"},
    {"Nancy", 5, "english101", "class10"}
};

int n = sizeof(arr) / sizeof(arr[0]);

void insertionSort(struct Faculty arr[], int n) {
    int i, j, key, swap_count = 0;
    for (i = 1; i < n; i++) {
        key = arr[i].faculty_ID;
        j = i-1;
        while (j >= 0 && arr[j].faculty_ID > key) {
            arr[j + 1].faculty_ID = arr[j].faculty_ID;
            j--;
            swap_count++;
        }
        arr[j + 1].faculty_ID = key;
    }
    printf("Number of swaps performed: %d\n", swap_count);
}

int main() {

    printf("\nBefore sorting \n");
    for (int i = 0; i < n; i++) {
        printf("%s\t%d\t%s\t%s\n", arr[i].faculty_name, arr[i].faculty_ID, arr[i].subject_codes, arr[i].class_names);
    }

    insertionSort(arr, n);

    printf("\nSorted Array :\n");
    for (int i = 0; i < n; i++) {
        printf("%s\t %d\n", arr[i].faculty_name, arr[i].faculty_ID);
    }

    return 0;
}
