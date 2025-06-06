#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct customer {
    int account_no;
    char first_name[25];
    char last_name[25];
    float balance;
};

void add_customer();
void display_customers();
void update_customer();
void deposit_amount();
void withdraw_amount();

int main() {
    int choice;

    while (1) {
        printf("\n=================================\n");
        printf("1. Add New Customer\n");
        printf("2. Display Customer Details\n");
        printf("3. Update Customer Information\n");
        printf("4. Deposit Amount\n");
        printf("5. Withdraw Amount\n");
        printf("6. Exit\n");
        printf("=================================\n");
        printf("Select an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_customer(); break;
            case 2: display_customers(); break;
            case 3: update_customer(); break;
            case 4: deposit_amount(); break;
            case 5: withdraw_amount(); break;
            case 6: exit(0);
            default: printf("Invalid option.\n");
        }
    }
}

void add_customer() {
    FILE *bin = fopen("customer.dat", "ab");
    FILE *txt = fopen("customer.txt", "a");
    struct customer c;
    int id, exists = 0;

    printf("Enter Account Number: ");
    scanf("%d", &id);

    FILE *check = fopen("customer.dat", "rb");
    if (check) {
        while (fread(&c, sizeof(c), 1, check)) {
            if (c.account_no == id) {
                exists = 1;
                break;
            }
        }
        fclose(check);
    }

    if (exists) {
        printf("Account already exists.\n");
        return;
    }

    if (!bin || !txt) {
        printf("File error.\n");
        return;
    }

    c.account_no = id;
    printf("First Name: ");
    scanf("%s", c.first_name);
    printf("Last Name: ");
    scanf("%s", c.last_name);
    printf("Initial Balance: ");
    scanf("%f", &c.balance);

    fwrite(&c, sizeof(c), 1, bin);
    fprintf(txt, "%d %s %s %.2f\n", c.account_no, c.first_name, c.last_name, c.balance);

    fclose(bin);
    fclose(txt);

    printf("Customer added.\n");
}

void display_customers() {
    FILE *bin = fopen("customer.dat", "rb");
    FILE *txt = fopen("customer.txt", "w");
    struct customer c;

    if (!bin || !txt) {
        printf("File error.\n");
        return;
    }

    printf("\nCustomer Records:\n-------------------------\n");
    while (fread(&c, sizeof(c), 1, bin)) {
        printf("Account: %d\n", c.account_no);
        printf("Name   : %s %s\n", c.first_name, c.last_name);
        printf("Balance: %.2f\n", c.balance);
        printf("-------------------------\n");

        fprintf(txt, "%d %s %s %.2f\n", c.account_no, c.first_name, c.last_name, c.balance);
    }

    fclose(bin);
    fclose(txt);
}

void update_customer() {
    FILE *bin = fopen("customer.dat", "rb+");
    struct customer c;
    int id, found = 0;

    if (!bin) {
        printf("File error.\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &id);

    while (fread(&c, sizeof(c), 1, bin)) {
        if (c.account_no == id) {
            found = 1;

            printf("New First Name: ");
            scanf("%s", c.first_name);
            printf("New Last Name : ");
            scanf("%s", c.last_name);

            fseek(bin, -sizeof(c), SEEK_CUR);
            fwrite(&c, sizeof(c), 1, bin);
            printf("Update successful.\n");
            break;
        }
    }

    fclose(bin);

    if (!found) {
        printf("Account not found.\n");
    } else {
        display_customers();
    }
}

void deposit_amount() {
    FILE *bin = fopen("customer.dat", "rb+");
    struct customer c;
    int id, found = 0;
    float amount;

    if (!bin) {
        printf("File error.\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &id);

    while (fread(&c, sizeof(c), 1, bin)) {
        if (c.account_no == id) {
            found = 1;

            printf("Amount to Deposit: ");
            scanf("%f", &amount);
            if (amount <= 0) {
                printf("Invalid amount.\n");
                break;
            }

            c.balance += amount;
            fseek(bin, -sizeof(c), SEEK_CUR);
            fwrite(&c, sizeof(c), 1, bin);
            printf("Deposit successful. Balance: %.2f\n", c.balance);
            break;
        }
    }

    fclose(bin);
    if (!found) printf("Account not found.\n");
}

void withdraw_amount() {
    FILE *bin = fopen("customer.dat", "rb+");
    struct customer c;
    int id, found = 0;
    float amount;

    if (!bin) {
        printf("File error.\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &id);

    while (fread(&c, sizeof(c), 1, bin)) {
        if (c.account_no == id) {
            found = 1;

            printf("Amount to Withdraw: ");
            scanf("%f", &amount);
            if (amount <= 0 || amount > c.balance) {
                printf("Invalid amount.\n");
                break;
            }

            c.balance -= amount;
            fseek(bin, -sizeof(c), SEEK_CUR);
            fwrite(&c, sizeof(c), 1, bin);
            printf("Withdrawal successful. Balance: %.2f\n", c.balance);
            break;
        }
    }

    fclose(bin);
    if (!found) printf("Account not found.\n");
}
