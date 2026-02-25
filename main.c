#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Account {
    int accountNumber;
    char name[50];
    float balance;
};

void clearScreen() {
    system("cls");   // Use "clear" for Linux/Mac
}

void pauseScreen() {
    printf("\nPress Enter to continue...");
    getchar();
    getchar();
}

void login();
void createAccount();
void depositMoney();
void withdrawMoney();
void checkBalance();
void displayAllAccounts();
void deleteAccount();

int main() {
    login();

    int choice;

    while (1) {
        clearScreen();

        printf("\n");
        printf("=================================================\n");
        printf("              MY BANKING SYSTEM                  \n");
        printf("=================================================\n");
        printf("  1.  Create New Account\n");
        printf("  2.  Deposit Money\n");
        printf("  3.  Withdraw Money\n");
        printf("  4.  Check Account Balance\n");
        printf("  5.  Display All Accounts\n");
        printf("  6.  Delete Account\n");
        printf("  7.  Exit\n");
        printf("=================================================\n");
        printf("Enter your choice (1-7): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break;
            case 2: depositMoney(); break;
            case 3: withdrawMoney(); break;
            case 4: checkBalance(); break;
            case 5: displayAllAccounts(); break;
            case 6: deleteAccount(); break;
            case 7:
                clearScreen();
                printf("\nThank you for using My Banking System!\n\n");
                exit(0);
            default:
                printf("\nInvalid choice!\n");
                pauseScreen();
        }
    }

    return 0;
}

void login() {
    char username[20], password[20];

    while (1) {
        clearScreen();
        printf("====================================\n");
        printf("        ADMIN LOGIN REQUIRED        \n");
        printf("====================================\n");

        printf("Username: ");
        scanf("%s", username);

        printf("Password: ");
        scanf("%s", password);

        if (strcmp(username, "admin") == 0 && strcmp(password, "1234") == 0) {
            printf("\nLogin Successful!\n");
            pauseScreen();
            break;
        } else {
            printf("\nInvalid Credentials! Try Again.\n");
            pauseScreen();
        }
    }
}

void createAccount() {
    clearScreen();
    FILE *file = fopen("accounts.dat", "ab");
    struct Account acc;

    printf("=========== CREATE NEW ACCOUNT ===========\n");

    printf("Enter Account Number: ");
    scanf("%d", &acc.accountNumber);

    printf("Enter Account Holder Name: ");
    scanf(" %[^\n]", acc.name);

    printf("Enter Initial Balance: ");
    scanf("%f", &acc.balance);

    fwrite(&acc, sizeof(acc), 1, file);
    fclose(file);

    printf("\nAccount Created Successfully!\n");
    pauseScreen();
}

void depositMoney() {
    clearScreen();
    FILE *file = fopen("accounts.dat", "rb+");
    struct Account acc;
    int accNo;
    float amount;
    int found = 0;

    printf("=========== DEPOSIT MONEY ===========\n");

    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.accountNumber == accNo) {
            printf("Enter Amount to Deposit: ");
            scanf("%f", &amount);

            acc.balance += amount;
            fseek(file, -sizeof(acc), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, file);

            printf("\nDeposit Successful!\n");
            printf("Updated Balance: %.2f\n", acc.balance);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("\nAccount Not Found!\n");

    fclose(file);
    pauseScreen();
}

void withdrawMoney() {
    clearScreen();
    FILE *file = fopen("accounts.dat", "rb+");
    struct Account acc;
    int accNo;
    float amount;
    int found = 0;

    printf("=========== WITHDRAW MONEY ===========\n");

    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.accountNumber == accNo) {
            printf("Enter Amount to Withdraw: ");
            scanf("%f", &amount);

            if (amount > acc.balance) {
                printf("\nInsufficient Balance!\n");
            } else {
                acc.balance -= amount;
                fseek(file, -sizeof(acc), SEEK_CUR);
                fwrite(&acc, sizeof(acc), 1, file);
                printf("\nWithdrawal Successful!\n");
                printf("Remaining Balance: %.2f\n", acc.balance);
            }
            found = 1;
            break;
        }
    }

    if (!found)
        printf("\nAccount Not Found!\n");

    fclose(file);
    pauseScreen();
}

void checkBalance() {
    clearScreen();
    FILE *file = fopen("accounts.dat", "rb");
    struct Account acc;
    int accNo;
    int found = 0;

    printf("=========== CHECK BALANCE ===========\n");

    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.accountNumber == accNo) {
            printf("\nAccount Holder: %s\n", acc.name);
            printf("Current Balance: %.2f\n", acc.balance);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("\nAccount Not Found!\n");

    fclose(file);
    pauseScreen();
}

void displayAllAccounts() {
    clearScreen();
    FILE *file = fopen("accounts.dat", "rb");
    struct Account acc;

    printf("=========== ALL BANK ACCOUNTS ===========\n\n");
    printf("%-15s %-20s %-15s\n", "Account No", "Name", "Balance");
    printf("----------------------------------------------------------\n");

    while (fread(&acc, sizeof(acc), 1, file)) {
        printf("%-15d %-20s %-15.2f\n",
               acc.accountNumber,
               acc.name,
               acc.balance);
    }

    fclose(file);
    pauseScreen();
}

void deleteAccount() {
    clearScreen();
    FILE *file = fopen("accounts.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    struct Account acc;
    int accNo;
    int found = 0;

    printf("=========== DELETE ACCOUNT ===========\n");

    printf("Enter Account Number to Delete: ");
    scanf("%d", &accNo);

    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.accountNumber == accNo) {
            found = 1;
        } else {
            fwrite(&acc, sizeof(acc), 1, temp);
        }
    }

    fclose(file);
    fclose(temp);

    remove("accounts.dat");
    rename("temp.dat", "accounts.dat");

    if (found)
        printf("\nAccount Deleted Successfully!\n");
    else
        printf("\nAccount Not Found!\n");

    pauseScreen();
}
