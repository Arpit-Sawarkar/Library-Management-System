                        //========== Library Managment System ===========//
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//Structure for book details
struct Book{
    int id;
    char name[50];
    char author[50];
    int available;
};
//Function Decleration
void addBook();
void viewBooks();
void issueBook();
void returnBook();
void deleteBook();
//Main Function
int main(){
    int choice;
    while(1){
        printf("\n========== Library Managment System ==========\n");
        printf("1. Add Book\n");
        printf("2. View All Book\n");
        printf("3. Issue Book\n");
        printf("4. Return Book\n");
        printf("5. Delete Book\n");
        printf("6. Exit\n");
        printf("Enter Your Choice: ");
        scanf("%d",&choice);

        switch(choice){
            case 1: addBook(); break;
            case 2: viewBooks(); break;
            case 3: issueBook(); break;
            case 4: returnBook(); break;
            case 5: deleteBook(); break;
            case 6: 
              printf("\nThank you for using the Library Management System.\n");
              printf("Have a great day!\n");
              exit(0);
            default : printf("Invalid Choice! Please try again \n");
        }
    }
    return 0;
}
//Function to Add a New Book
void addBook(){
    struct Book b;
    FILE *fp = fopen("library.txt","ab");  //append binary mode
    if(!fp){
        printf("Error opening file!\n");
        return;
    }
    printf("Enter Book ID: ");
    scanf("%d", &b.id);
    getchar();  // clear the leftover newline from buffer

    printf("Enter Book Name: ");
    fgets(b.name, sizeof(b.name), stdin);
    b.name[strcspn(b.name, "\n")] = 0;  // remove trailing newline

    printf("Enter Author Name: ");
    fgets(b.author, sizeof(b.author), stdin);
    b.author[strcspn(b.author, "\n")] = 0;
    b.available = 1;  //initally avaiable

    fwrite(&b, sizeof(b), 1, fp);
    fclose(fp);
    printf("Book added successfully!\n");
}
//Function to View All Books
void viewBooks() {
    struct Book b;
    FILE *fp = fopen("library.txt","rb");
    if(!fp){
        printf("No Book Found!\n");
        return;
    }
    printf("\n-------Add Books--------\n");
    while(fread(&b, sizeof(b), 1, fp)){
        printf("ID : %d | Name : %s | Author : %s | %s\n", 
            b.id, b.name, b.author, b.available ? "Available":"Issued");
    }
    fclose(fp);
}
//Function Issue a Books
void issueBook() {
    int id, found=0;
    struct Book b;
    FILE *fp = fopen("library.txt","rb+");
    if(!fp){
        printf("No Book Found!\n");
        return;
    }
    printf("Enter Book id to issue: ");
    scanf("%d",&id);

    while(fread(&b, sizeof(b), 1, fp)){
        if(b.id==id){
            found =1;
            if (b.available){
                b.available = 0;
                fseek(fp, -sizeof(b), SEEK_CUR);
                fwrite(&b, sizeof(b), 1, fp);
                fflush(fp);    //ensuring data is writtten to a file
                printf("Book issued succesfully!\n");
            }else{
                printf("Book Already issued!\n");
            }
            break;
        }
    }
    if(!found){
        printf("Book not found!\n");
    }
    fclose(fp);
}
//Function to Return Book
void returnBook(){
    int id,found=0;
    struct Book b;
    FILE *fp = fopen("library.txt","rb+");
    if (!fp){
        printf("No Book Found!\n");
        return;
    }
    printf("Enter Book ID to return: ");
    scanf("%d",&id);

    while(fread(&b, sizeof(b), 1, fp)){
        if(b.id==id){
            found=1;
            if (!b.available){
                b.available = 1;
                fseek(fp, -sizeof(b),SEEK_CUR);
                fwrite(&b, sizeof(b), 1, fp);
                printf("Book Returned Succesffully!\n");
            }else{
                printf("This book was not issued!\n");
            }
            break;
        }
    }
    if (!found){
        printf("Book not Found!\n");
    }
    fclose(fp);
}
//Function for Delete book
void deleteBook() {
    int id, found = 0;
    struct Book b;

    FILE *fp = fopen("library.txt", "rb");
    if (!fp) {
        printf("No Book Found!\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "wb");
    if (!temp) {
        printf("Unable to create temporary file!\n");
        fclose(fp);
        return;
    }

    printf("Enter Book ID to delete: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(b), 1, fp)) {
        if (b.id == id) {
            found = 1;
            // Skip writing this book to temp file -> this deletes it
            continue;
        }
        fwrite(&b, sizeof(b), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove("library.txt");              // Delete old file
        rename("temp.txt", "library.txt"); // Rename temp file to original
        printf("Book with ID %d deleted successfully.\n", id);
    } else {
        remove("temp.txt"); // Cleanup temp file if not found
        printf("Book with ID %d not found.\n", id);
    }
}        