#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_BOOK 100

struct Book
{
    char title[50];
    char code[10];
    int quantity;
};

void saveBookData(struct Book books[], int bookCount)
{
    FILE *file = fopen("book_data.txt", "w");

    if (file == NULL)
    {
        printf("Failed to open file for writing.\n");
        exit(1);
    }

    fprintf(file, "Title,Code,Quantity\n");

    for (int i = 0; i < bookCount; i++)
    {
        fprintf(file, "%s,%s,%d\n", books[i].title, books[i].code, books[i].quantity);
    }

    fclose(file);
}

void readBookData(struct Book books[], int *bookCount)
{
    FILE *file = fopen("book_data.txt", "r");

    if (file == NULL)
    {
        printf("File book_data.txt not found. Create a file with initial data.\n");
        exit(1);
    }

    fscanf(file, "Title,Code,Quantity\n");

    while (fscanf(file, "%[^,],%[^,],%d\n", books[*bookCount].title, books[*bookCount].code, &books[*bookCount].quantity) == 3)
    {
        (*bookCount)++;
    }

    fclose(file);
}

void displayMenu()
{
    printf("\nPilih Transaksi:\n");
    printf("P - Pinjam Buku\n");
    printf("B - Kembalikan Buku\n");
    printf("Q - Keluar\n");
    printf("Masukkan Kode Transaksi: ");
}

void borrowBook(struct Book books[], int *totalBorrowed)
{
    char input[10];
    printf("Masukkan Kode Buku yang ingin dipinjam: ");
    scanf("%9s", input);

    // Memeriksa format input
    if (input[0] != 'B' && input[0] != 'b')
    {
        printf("Format kode buku tidak valid.\n");
        return;
    }

    // Memeriksa apakah karakter setelah 'B' adalah digit
    if (!isdigit(input[1]))
    {
        printf("Format kode buku tidak valid.\n");
        return;
    }

    // Ekstraksi kode buku
    char bookCode[10];
    strncpy(bookCode, input + 1, sizeof(bookCode) - 1);
    bookCode[sizeof(bookCode) - 1] = '\0';

    for (int i = 0; i < MAX_BOOK; i++)
    {
        if (strcmp(books[i].code, bookCode) == 0)
        {
            if (books[i].quantity > 0)
            {
                books[i].quantity--;
                (*totalBorrowed)++;
                printf("Buku '%s' berhasil dipinjam.\n", books[i].title);
            }
            else
            {
                printf("Maaf, stok buku '%s' habis.\n", books[i].title);
            }
            return;
        }
    }

    printf("Buku dengan kode '%s' tidak ditemukan.\n", bookCode);
}

void returnBook(struct Book books[], int *totalBorrowed, int *totalReturned)
{
    char bookCode[10];
    int quantityReturned;

    printf("Masukkan Kode Buku yang akan dikembalikan: ");
    scanf("%s", bookCode);

    for (int i = 0; i < MAX_BOOK; i++)
    {
        if (strcmp(books[i].code, bookCode) == 0)
        {
            printf("Masukkan Jumlah Buku yang akan dikembalikan: ");
            scanf("%d", &quantityReturned);

            if (quantityReturned <= *totalBorrowed)
            {
                books[i].quantity += quantityReturned;
                (*totalBorrowed) -= quantityReturned;
                (*totalReturned) += quantityReturned;
                printf("%d buku '%s' berhasil dikembalikan.\n", quantityReturned, books[i].title);
            }
            else
            {
                printf("Anda hanya meminjam %d buku '%s'.\n", books[i].quantity, books[i].title);
            }
            return;
        }
    }

    printf("Buku dengan kode '%s' tidak ditemukan.\n", bookCode);
}

int main()
{
    struct Book books[MAX_BOOK];
    int bookCount = 0;
    int totalBorrowed = 0;
    int totalReturned = 0;
    char transactionCode;

    readBookData(books, &bookCount);

    do
    {
        displayMenu();
        scanf(" %c", &transactionCode);

        switch (transactionCode)
        {
        case 'P':
        case 'p':
            borrowBook(books, &totalBorrowed);
            break;
        case 'B':
        case 'b':
            returnBook(books, &totalBorrowed, &totalReturned);
            break;
        case 'Q':
        case 'q':
            printf("Total Buku Dipinjam: %d\n", totalBorrowed);
            printf("Total Buku Dikembalikan: %d\n", totalReturned);
            saveBookData(books, bookCount);
            break;
        default:
            printf("Invalid transaction code. Try again.\n");
        }

    } while (transactionCode != 'Q' || transactionCode != 'q');

    return 0;
}
