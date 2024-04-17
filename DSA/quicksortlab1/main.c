#include <stdio.h>

struct Faculty {
    char name[32];
    int ID;
    char subject_codes[32];
    char class_names[32];
};

struct Faculty arr[] = {
    {"Steve", 44, "cs101","class1"},
    {"Bob", 21, "math101","class3"},
    {"Dustin", 33, "physics101", "class5"},
    {"Mike", 14, "history101", "class7"},
    {"Nancy", 5, "english101", "class10"}
};

int n = sizeof(arr) / sizeof(arr[0]);

int swap_count;

int partition(int low, int high) {
    int pivot = arr[low].ID;
    int i = low + 1;
    int j = high;

    while (1) {
        while (i <= j && arr[i].ID <= pivot)
            i++;
        while (arr[j].ID > pivot)
            j--;
        if (i <= j) {
            struct Faculty temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
            j--;
            swap_count++;
        } else {
            struct Faculty temp = arr[j];
            arr[j] = arr[low];
            arr[low] = temp;
            swap_count++;
            return j;
            break;
        }
    }

}

void quick_sort(int low, int high) {
    if (low < high) {
        int pi = partition(low, high);
        quick_sort(low, pi - 1);
        quick_sort(pi + 1, high);
    }
}

int main() {

    printf("Original list:\n");
    for (int i = 0; i < n; i++) {
        printf("%s\t%d\n", arr[i].name, arr[i].ID);
    }

    swap_count = 0;
    quick_sort( 0, n - 1);

    printf("\nSorted Array :\n");
    for (int i = 0; i < n; i++) {
        printf("%s\t %d\n", arr[i].name, arr[i].ID);
    }
    printf("Quick sort: Number of swaps = %d\n", swap_count);

    return 0;
}
