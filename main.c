#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 100
#define MAX_NAME 50

typedef struct Book {
    char title[MAX_NAME];
    int quantity;
    double price;
    struct Book *left, *right;
} Book;

Book *root = NULL;
int bookIndex = 0;
char titles[MAX_BOOKS][MAX_NAME];
double prices[MAX_BOOKS];
int quantities[MAX_BOOKS];

Book *createBook(char *title, double price, int quantity) {
    Book *newBook = (Book *)malloc(sizeof(Book));
    strcpy(newBook->title, title);
    newBook->price = price;
    newBook->quantity = quantity;
    newBook->left = newBook->right = NULL;
    return newBook;
}

Book *insertBook(Book *node, char *title, double price, int quantity) {
    if (!node) return createBook(title, price, quantity);
    if (strcmp(title, node->title) < 0)
        node->left = insertBook(node->left, title, price, quantity);
    else if (strcmp(title, node->title) > 0)
        node->right = insertBook(node->right, title, price, quantity);
    return node;
}

Book *findMin(Book *node) {
    while (node->left) node = node->left;
    return node;
}

Book *deleteBook(Book *node, char *title) {
    if (!node) return node;
    if (strcmp(title, node->title) < 0)
        node->left = deleteBook(node->left, title);
    else if (strcmp(title, node->title) > 0)
        node->right = deleteBook(node->right, title);
    else {
        if (!node->left) {
            Book *temp = node->right;
            free(node);
            return temp;
        } else if (!node->right) {
            Book *temp = node->left;
            free(node);
            return temp;
        }
        Book *temp = findMin(node->right);
        strcpy(node->title, temp->title);
        node->price = temp->price;
        node->quantity = temp->quantity;
        node->right = deleteBook(node->right, temp->title);
    }
    return node;
}

Book *searchBook(Book *node, char *title) {
    if (!node || strcmp(title, node->title) == 0) return node;
    if (strcmp(title, node->title) < 0)
        return searchBook(node->left, title);
    return searchBook(node->right, title);
}

void addBook(char *title, double price, int quantity) {
    root = insertBook(root, title, price, quantity);
    strcpy(titles[bookIndex], title);
    prices[bookIndex] = price;
    quantities[bookIndex++] = quantity;
}

void updateQuantity(char *title, int newQuantity) {
    Book *book = searchBook(root, title);
    if (book) {
        book->quantity = newQuantity;
        for (int i = 0; i < bookIndex; i++) {
            if (strcmp(titles[i], title) == 0) {
                quantities[i] = newQuantity;
                break;
            }
        }
    }
}

void checkAvailability(char *title) {
    Book *book = searchBook(root, title);
    if (book) printf("Title: %s, Price: %.2f, Quantity: %d\n", book->title, book->price, book->quantity);
    else printf("Book not available.\n");
}

void generateBill() {
    char title[MAX_NAME];
    int qty;
    double total = 0;
    printf("Enter book title and quantity (type 'end' to finish): \n");
    while (1) {
        scanf("%s", title);
        if (strcmp(title, "end") == 0) break;
        scanf("%d", &qty);
        Book *book = searchBook(root, title);
        if (book && book->quantity >= qty) {
            double cost = qty * book->price;
            printf("Title: %s, Quantity: %d, Cost: %.2f\n", book->title, qty, cost);
            total += cost;
            book->quantity -= qty;
        } else {
            printf("Book not available or insufficient stock.\n");
        }
    }
    printf("Total Amount: %.2f\n", total);
}

void loadBooks() {
    FILE *file = fopen("books.txt", "r");
    char title[MAX_NAME];
    double price;
    int quantity;
    while (fscanf(file, "%s %lf %d", title, &price, &quantity) != EOF) {
        addBook(title, price, quantity);
    }
    fclose(file);
}

int main() {
    int choice;
    char title[MAX_NAME];
    double price;
    int quantity;
    loadBooks();
    printf("Admin Login\n");
    while (1) {
        printf("\n1. Add Book\n2. Delete Book\n3. Update Quantity\n4. Check Availability\n5. Generate Bill\n6. Exit\n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("Enter title, price, quantity: ");
                scanf("%s %lf %d", title, &price, &quantity);
                addBook(title, price, quantity);
                break;
            case 2:
                printf("Enter title to delete: ");
                scanf("%s", title);
                root = deleteBook(root, title);
                break;
            case 3:
                printf("Enter title and new quantity: ");
                scanf("%s %d", title, &quantity);
                updateQuantity(title, quantity);
                break;
            case 4:
                printf("Enter title to check availability: ");
                scanf("%s", title);
                checkAvailability(title);
                break;
            case 5:
                generateBill();
                break;
            case 6:
                exit(0);
        }
    }
    return 0;
}
