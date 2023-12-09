#include <stdio.h>
#include <stdbool.h>

#define MAX_SHIPMENTS 50

struct Shipment
{
    int id;
    int initialStock;
    int stockSent;
    int stockHasntSent;
    int finalStock;
};

void clearBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int readShipments(struct Shipment shipments[], const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 0;
    }

    int count = 0;
    while (fscanf(file, "Id: %d\nInitialStock: %d\nStockSent: %d\nStockHasntSent: %d\nFinalStock: %d\n#\n",
                  &shipments[count].id, &shipments[count].initialStock, &shipments[count].stockSent,
                  &shipments[count].stockHasntSent, &shipments[count].finalStock) == 5)
    {
        count++;
        if (count >= MAX_SHIPMENTS)
        {
            break; // Avoid exceeding the array size
        }
    }

    fclose(file);
    return count;
}

void writeShipment(struct Shipment shipments[], int size, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < size; i++)
    {
        fprintf(file, "Id: %d\n", shipments[i].id);
        fprintf(file, "InitialStock: %d\n", shipments[i].initialStock);
        fprintf(file, "StockSent: %d\n", shipments[i].stockSent);
        fprintf(file, "StockHasntSent: %d\n", shipments[i].stockHasntSent);
        fprintf(file, "FinalStock: %d\n", shipments[i].finalStock);
        fprintf(file, "#\n");
    }

    fclose(file);
}

void printShipment(struct Shipment shipment[], int size)
{
    for (int i = 0; i < size; i++)
    {
        shipment[i].finalStock = shipment[i].initialStock - shipment[i].stockSent;

        printf("ID: %d\n", shipment[i].id);
        printf("Jumlah Barang Awal: %d\n", shipment[i].initialStock);
        printf("Jumlah Barang Terkirim: %d\n", shipment[i].stockSent);
        printf("Jumlah Barang Yang Belum Terkirim: %d\n", shipment[i].stockHasntSent);
        printf("Jumlah Barang Akhir: %d\n\n", shipment[i].finalStock); // Corrected the Final stock formula with FinalStock = InitialStock - StockSent
    }
}

void modifyShipment(struct Shipment shipment[], int index, int select)
{
    int changes;
    switch (select)
    {
    case 1:
        printf("Jumlah Barang Awal: %d  ---->  ", shipment[index - 1].initialStock);
        scanf("%d", &changes);
        shipment[index - 1].initialStock = changes;
        printf("\nData berhasil diubah menjadi:\n\n");
        printf("ID: %d\n", shipment[index - 1].id);
        printf("Jumlah Barang Awal: %d\n", shipment[index - 1].initialStock);
        clearBuffer(); // Membersihkan buffer masukan
        break;
    case 2:
        printf("Jumlah Barang Terkirim: %d  ---->  ", shipment[index - 1].stockSent);
        scanf("%d", &changes);
        shipment[index - 1].stockSent = changes;
        printf("\nData berhasil diubah menjadi:\n\n");
        printf("ID: %d\n", shipment[index - 1].id);
        printf("Jumlah Barang Terkirim: %d\n", shipment[index - 1].stockSent);

        clearBuffer(); // Membersihkan buffer masukan
        break;
    case 3:
        printf("Jumlah Barang Belum Terkirim: %d  ---->  ", shipment[index - 1].stockHasntSent);
        scanf("%d", &changes);
        shipment[index - 1].stockHasntSent = changes;
        printf("\nData berhasil diubah menjadi:\n\n");
        printf("ID: %d\n", shipment[index - 1].id);
        printf("Jumlah Barang Yang Belum Terkirim: %d\n", shipment[index - 1].stockHasntSent);
        clearBuffer(); // Membersihkan buffer masukan
        break;
    default:
        break;
    }
}

int main()
{
    struct Shipment shipments[MAX_SHIPMENTS];
    int shipmentCount = readShipments(shipments, "DataPengiriman.txt");
    int option;
    int isSaved = 1;

    if (shipmentCount == 0)
    {
        printf("No shipments found or error reading file.\n");
        return 1;
    }

    do
    {
        printf("==========================\n");
        printf("PROGRAM LAPORAN PENGIRIMAN\n");
        printf("==========================\n\n");
        printf("Menu\n");
        printf("1. Lihat detail laporan pengiriman\n");
        printf("2. Ubah detail pengiriman\n");
        printf("3. Simpan Perubahan\n");
        printf("4. Keluar\n");
        printf("Masukan Pilihan (1-4): ");
        scanf("%d", &option);

        switch (option)
        {
        case 1:
        {
            int temp;
            printShipment(shipments, shipmentCount);
            printf("Berikut adalah detail Laporan Pengiriman, apakah anda ingin kembali ke Menu (1 = Ya)? ");
            scanf("%d", &temp);
            if (temp != 1)
            {
                printShipment(shipments, shipmentCount);
                printf("Berikut adalah detail Laporan Pengiriman, apakah anda ingin kembali ke Menu (1 = Ya)? ");
                scanf("%d", &temp);
            }
            else
            {
                break;
            }
            break;
        }
        case 2:
        {
            int temp;
            int id;

            printShipment(shipments, shipmentCount);

            do
            {
                printf("Masukan ID pengiriman yang ingin anda ubah (Masukan 0 untuk kembali): ");
                scanf("%d", &id);

                if (id == 0)
                {
                    break; // Keluar dari loop jika ID adalah 0
                }

                if (id < 1 || id > shipmentCount)
                {
                    printf("ID tidak valid. Silakan masukkan ID yang valid.\n");
                }

            } while (id < 1 || id > shipmentCount);

            if (id == 0)
            {
                break; // Keluar dari switch case jika ID adalah 0
            }

            printf("\nData apa yang ingin anda ubah?\n");
            printf("1. Ubah Jumlah Barang Awal\n");
            printf("2. Ubah Jumlah Barang Terkirim\n");
            printf("3. Ubah Jumlah Barang Belum Terkirim\n");
            printf("4. Kembali\n");
            printf("Masukan pilihan: ");
            scanf("%d", &temp);

            switch (temp)
            {
            case 1:
                modifyShipment(shipments, id, temp);
                isSaved = 0;
                break;
            case 2:
                modifyShipment(shipments, id, temp);
                isSaved = 0;
                break;
            case 3:
                modifyShipment(shipments, id, temp);
                isSaved = 0;
                break;
            default:
                printf("Input tidak valid. Harap masukkan input yang valid\n");
                break;
            }
            break; // Keluar dari switch case untuk kembali ke menu utama
        }

        case 3:
            if (!isSaved)
            {
                writeShipment(shipments, shipmentCount, "DataPengiriman.txt");
                printf("Perubahan berhasil disimpan.\n");
                isSaved = 1; // Setelah disimpan, dianggap sudah disimpan
            }
            break;

        case 4:
            if (!isSaved)
            {
                printf("Anda belum menyimpan perubahan, apakah anda ingin menyimpan perubahan (1 = Ya || 0 = Tidak)? ");
                scanf("%d", &isSaved);
                if (isSaved)
                {
                    writeShipment(shipments, shipmentCount, "DataPengiriman.txt");
                    printf("Perubahan berhasil disimpan");
                }
                break;
            }
            break;

        default:
            printf("\nInput tidak valid. Harap masukkan input yang valid\n");
            break;
        }
    } while (option != 4);

    return 0;
}
