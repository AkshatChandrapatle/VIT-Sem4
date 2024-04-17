#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Product {
    int code;
    char name[50];
    double price;
    int stock;
    char date_received[11];
    char expiration_date[11];
    struct Product* left;
    struct Product* right;
};
int isValidDate(char* date_str) {
    // Check if the date is "N/A"

    // Check the length of the date string
    if (strlen(date_str) != 10) {
        return 0;
    }

    // Check that the date string contains dashes at the right positions
    if (date_str[4] != '-' || date_str[7] != '-') {
        return 0;
    }

    // Check that the year, month, and day parts of the date string are numeric
    int year, month, day;
    if (sscanf(date_str, "%4d-%2d-%2d", &year, &month, &day) != 3) {
        return 0;
    }

    // Check that the month and day parts of the date string are within the correct ranges
    if (month < 1 || month > 12 || day < 1 || day > 31) {
        return 0;
    }
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day == 31) {
        return 0;
    }
    if (month == 2) {
        if (day > 29) {
            return 0;
        }
        if (day == 29 && (year % 4 != 0 || (year % 100 == 0 && year % 400 != 0))) {
            return 0;
        }
    }

    return 1;  // The date is valid
}


struct Product* newProduct(int code, char name[], double price, int stock, char date_received[], char expiration_date[]) {
    struct Product* new_product = (struct Product*) malloc(sizeof(struct Product));
    new_product->code = code;
    strcpy(new_product->name, name);
    new_product->price = price;
    new_product->stock = stock;
    strcpy(new_product->date_received, date_received);
    strcpy(new_product->expiration_date, expiration_date);
    new_product->left = NULL;
    new_product->right = NULL;
    return new_product;
}

struct Product* insertProduct(struct Product* root, int code, char name[], double price, int stock, char date_received[], char expiration_date[]) {
    if (root == NULL) {
        return newProduct(code, name, price, stock, date_received, expiration_date);
    }
    if (code < root->code) {
        root->left = insertProduct(root->left, code, name, price, stock, date_received, expiration_date);
    } else if (code > root->code) {
        root->right = insertProduct(root->right, code, name, price, stock, date_received, expiration_date);
    } else {
        printf("Error: Product code already exists.\n");
    }
    return root;
}

struct Product* findProduct(struct Product* root, int code) {
    if (root == NULL) {
        return NULL;
    }
    if (root->code == code) {
        return root;
    } else if (code < root->code) {
        return findProduct(root->left, code);
    } else {
        return findProduct(root->right, code);
    }
}

void updateProduct(struct Product* product, char name[], double price, int stock, char date_received[], char expiration_date[]) {
    strcpy(product->name, name);
    product->price = price;
    product->stock = stock;
    strcpy(product->date_received, date_received);
    strcpy(product->expiration_date, expiration_date);
}

void inOrderTraversal(struct Product* root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        if (strcmp(root->expiration_date, "N/A") != 0) {
            printf("%d %s %.2f %d %s %s\n", root->code, root->name, root->price, root->stock, root->date_received, root->expiration_date);
        }
        inOrderTraversal(root->right);
    }
}

int main() {
    struct Product* root = NULL;
    int choice, code, stock;
    double price;
    char name[50], date_received[11], expiration_date[11];
    struct Product* product;
    printf("\n1. Insert a new product\n");
    printf("2. Find a product \n");
    printf("3. Update a product \n");
    printf("4. List products: \n");
    printf("5. Exit\n");
    while (1) {

        printf("Enter your choice: ");
        scanf("%d", &choice);
        printf("\n");

        switch (choice) {
    case 1:
        printf("Enter product code: ");
        scanf("%d", &code);
        printf("Enter product name: ");
        scanf(" %[^\n]", name);
        printf("Enter product price: ");
        scanf("%lf", &price);
        printf("Enter product stock: ");
        scanf("%d", &stock);
        printf("Enter date received (YYYY-MM-DD): ");
        scanf("%s", date_received);
        if (!isValidDate(date_received)) {
            printf("Error: Invalid date received.\n");
            break;
        }
        printf("Enter expiration date (YYYY-MM-DD or N/A): ");
        scanf("%s", expiration_date);
        if (!isValidDate(expiration_date) && strcmp(expiration_date, "N/A") != 0) {
            printf("Error: Invalid expiration date.\n");
            break;
        }
        if (strcmp(expiration_date, "N/A") != 0 && strcmp(date_received, expiration_date) >= 0) {
            printf("Error: Expiration date must be after date received.\n");
            break;
        }
        root = insertProduct(root, code, name, price, stock, date_received, expiration_date);
        printf("Product added successfully.\n");
        break;
    case 2:
        printf("Enter product code: ");
        scanf("%d", &code);
        product = findProduct(root, code);
        if (product == NULL) {
            printf("Product not found.\n");
            break;
        }
        printf("%d %s %.2f %d %s %s\n", product->code, product->name, product->price, product->stock, product->date_received, product->expiration_date);
        break;
    case 3:
        printf("Enter product code: ");
        scanf("%d", &code);
        product = findProduct(root, code);
        if (product == NULL) {
            printf("Product not found.\n");
            break;
        }
        printf("Enter new product name: ");
        scanf(" %[^\n]", name);
        printf("Enter new product price: ");
        scanf("%lf", &price);
        printf("Enter new product stock: ");
        scanf("%d", &stock);
        printf("Enter new date received (YYYY-MM-DD): ");
        scanf("%s", date_received);
        if (!isValidDate(date_received)) {
            printf("Error: Invalid date received.\n");
            break;
        }
        printf("Enter new expiration date (YYYY-MM-DD or N/A): ");
        scanf("%s", expiration_date);
        if (!isValidDate(expiration_date) && strcmp(expiration_date, "N/A") != 0) {
            printf("Error: Invalid expiration date.\n");
            break;
        }
        if (strcmp(expiration_date, "N/A") != 0 && strcmp(date_received, expiration_date) >= 0) {
            printf("Error: Expiration date must be after date received.\n");
            break;
        }
        updateProduct(product, name, price, stock, date_received, expiration_date);
        printf("Product updated successfully.\n");
        break;
    case 4:
        printf("Code Name\t Price Stock Date_Received Expiration_Date\n");
        inOrderTraversal(root);
        break;
    case 5:
        printf("Exiting...\n");
        exit(0);
    default:
        printf("Invalid choice.\n");
        break;

    }


}

return 0;
}
