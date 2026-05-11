// Improved Bank Account Management Program using Random-Access Files

#include <stdio.h>
#include <stdlib.h>

#define FILE_NAME "credit.dat"
#define MAX_ACCOUNTS 100

// structure definition
struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// function prototypes
unsigned int enterChoice(void);
void createTextFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void addRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void initializeFile(void);

int main(void)
{
    FILE *cfPtr;
    unsigned int choice;

    // create file if it does not exist
    initializeFile();

    // open file
    if ((cfPtr = fopen(FILE_NAME, "rb+")) == NULL)
    {
        printf("File could not be opened.\n");
        return 1;
    }

    while ((choice = enterChoice()) != 5)
    {
        switch (choice)
        {
        case 1:
            createTextFile(cfPtr);
            break;

        case 2:
            updateRecord(cfPtr);
            break;

        case 3:
            addRecord(cfPtr);
            break;

        case 4:
            deleteRecord(cfPtr);
            break;

        default:
            printf("Invalid choice.\n");
        }
    }

    fclose(cfPtr);

    printf("Program terminated.\n");

    return 0;
}

// initialize binary file with empty records
void initializeFile(void)
{
    FILE *file;

    if ((file = fopen(FILE_NAME, "rb")) == NULL)
    {
        file = fopen(FILE_NAME, "wb");

        if (file == NULL)
        {
            printf("Unable to create file.\n");
            exit(1);
        }

        struct clientData blankClient = {0, "", "", 0.0};

        for (int i = 0; i < MAX_ACCOUNTS; i++)
        {
            fwrite(&blankClient, sizeof(struct clientData), 1, file);
        }

        fclose(file);
    }
    else
    {
        fclose(file);
    }
}

// create formatted text file
void createTextFile(FILE *readPtr)
{
    FILE *writePtr;
    struct clientData client;

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        printf("Text file could not be created.\n");
        return;
    }

    rewind(readPtr);

    fprintf(writePtr, "%-6s%-16s%-11s%10s\n",
            "Acct", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr,
                    "%-6u%-16s%-11s%10.2f\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance);
        }
    }

    fclose(writePtr);

    printf("accounts.txt created successfully.\n");
}

// update existing record
void updateRecord(FILE *fPtr)
{
    struct clientData client;
    unsigned int account;
    double transaction;

    printf("Enter account number to update (1-100): ");
    scanf("%u", &account);

    if (account < 1 || account > MAX_ACCOUNTS)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
        return;
    }

    printf("\nCurrent Record:\n");
    printf("%-6u%-16s%-11s%10.2f\n",
           client.acctNum,
           client.lastName,
           client.firstName,
           client.balance);

    printf("Enter charge (+) or payment (-): ");
    scanf("%lf", &transaction);

    client.balance += transaction;

    fseek(fPtr, -(long)sizeof(struct clientData), SEEK_CUR);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Record updated successfully.\n");
}

// add new record
void addRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};
    unsigned int account;

    printf("Enter new account number (1-100): ");
    scanf("%u", &account);

    if (account < 1 || account > MAX_ACCOUNTS)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists.\n");
        return;
    }

    client.acctNum = account;

    printf("Enter lastname firstname balance:\n");
    scanf("%14s %9s %lf",
          client.lastName,
          client.firstName,
          &client.balance);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Record added successfully.\n");
}

// delete record
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0.0};

    unsigned int account;

    printf("Enter account number to delete (1-100): ");
    scanf("%u", &account);

    if (account < 1 || account > MAX_ACCOUNTS)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);

    printf("Record deleted successfully.\n");
}

// menu
unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n========== BANK MENU ==========\n");
    printf("1 - Create text file\n");
    printf("2 - Update account\n");
    printf("3 - Add new account\n");
    printf("4 - Delete account\n");
    printf("5 - Exit\n");
    printf("Enter choice: ");

    scanf("%u", &choice);

    return choice;
}
