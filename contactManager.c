#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTACTS 100
#define MAX_USERS 50
#define MAX_USERNAME_LEN 50
#define MAX_PASSWORD_LEN 50
#define MAX_NAME_LEN 100
#define MAX_EMAIL_LEN 150
#define MAX_PHONE_LEN 20

struct Contact
{
    char username[MAX_USERNAME_LEN];
    char name[MAX_NAME_LEN];
    char email[MAX_EMAIL_LEN];
    char phoneNumber[MAX_PHONE_LEN];
};

struct User
{
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
};

void addContact(struct Contact contacts[], int *count, const char *currentUsername)
{
    if (*count >= MAX_CONTACTS)
    {
        printf("Contact list is full. Cannot add more contacts.\n");
        return;
    }

    struct Contact newContact;
    strcpy(newContact.username, currentUsername);

    printf("Enter name: ");
    getchar();
    fgets(newContact.name, sizeof(newContact.name), stdin);
    newContact.name[strcspn(newContact.name, "\n")] = '\0';

    printf("Enter email: ");
    fgets(newContact.email, sizeof(newContact.email), stdin);
    newContact.email[strcspn(newContact.email, "\n")] = '\0';

    printf("Enter phone number: ");
    fgets(newContact.phoneNumber, sizeof(newContact.phoneNumber), stdin);
    newContact.phoneNumber[strcspn(newContact.phoneNumber, "\n")] = '\0';

    contacts[*count] = newContact;
    (*count)++;

    printf("Contact added successfully!\n");
}

void viewContacts(const struct Contact contacts[], int count, const char *currentUsername)
{
    if (count == 0)
    {
        printf("No contacts to display.\n");
        return;
    }

    printf("Contacts:\n");
    int serialNumber = 1;
    for (int i = 0; i < count; i++)
    {
        if (strcmp(currentUsername, contacts[i].username) == 0)
        {
            printf("%d. Name: %s\n", serialNumber, contacts[i].name);
            printf("   Email: %s\n", contacts[i].email);
            printf("   Phone: %s\n", contacts[i].phoneNumber);
            serialNumber++;
        }
    }
}

void deleteContact(struct Contact contacts[], int *count, const char *currentUsername)
{
    if (*count == 0)
    {
        printf("No contacts to delete.\n");
        return;
    }

    printf("Select a contact to delete by entering the serial number: ");
    int serialNumber;
    scanf("%d", &serialNumber);

    int found = 0;
    for (int i = 0; i < *count; i++)
    {
        if (strcmp(currentUsername, contacts[i].username) == 0)
        {
            if (serialNumber == 1)
            {
                for (int j = i; j < *count - 1; j++)
                {
                    contacts[j] = contacts[j + 1];
                }
                (*count)--;
                found = 1;
                printf("Contact deleted successfully!\n");
                break;
            }
            else
            {
                serialNumber--;
            }
        }
    }

    if (!found)
    {
        printf("Contact not found.\n");
    }
}

void saveContactsToFile(const struct Contact contacts[], int count)
{
    FILE *file = fopen("contacts.txt", "w");
    if (file == NULL)
    {
        printf("Error opening contacts file for writing.\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%s|%s|%s|%s\n", contacts[i].username, contacts[i].name, contacts[i].email, contacts[i].phoneNumber);
    }

    fclose(file);
}

void loadContactsFromFile(struct Contact contacts[], int *count)
{
    FILE *file = fopen("contacts.txt", "r");
    if (file == NULL)
    {
        return;
    }

    *count = 0;

    while (*count < MAX_CONTACTS && fscanf(file, "%[^|]|%[^|]|%[^|]|%[^\n]\n", contacts[*count].username,
                                           contacts[*count].name, contacts[*count].email, contacts[*count].phoneNumber) != EOF)
    {
        (*count)++;
    }

    fclose(file);
}

void saveUsersToFile(const struct User users[], int count)
{
    FILE *file = fopen("users.txt", "w");
    if (file == NULL)
    {
        printf("Error opening users file for writing.\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%s|%s\n", users[i].username, users[i].password);
    }

    fclose(file);
}

void loadUsersFromFile(struct User users[], int *count)
{
    FILE *file = fopen("users.txt", "r");
    if (file == NULL)
    {
        return;
    }

    *count = 0;

    while (*count < MAX_USERS && fscanf(file, "%[^|]|%[^\n]\n", users[*count].username, users[*count].password) != EOF)
    {
        (*count)++;
    }

    fclose(file);
}

int findUserIndex(const struct User users[], int userCount, const char *username)
{
    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(username, users[i].username) == 0)
        {
            return i;
        }
    }
    return -1;
}

int registerUser(struct User users[], int *userCount)
{
    if (*userCount >= MAX_USERS)
    {
        printf("Maximum number of users reached.\n");
        return 0;
    }

    struct User newUser;

    printf("Enter an username: ");
    scanf("%s", newUser.username);

    if (findUserIndex(users, *userCount, newUser.username) != -1)
    {
        printf("Username already exists. Please choose another username.\n");
        return 0;
    }

    printf("Enter a password: ");
    scanf("%s", newUser.password);

    users[*userCount] = newUser;
    (*userCount)++;

    printf("User registered successfully!\n");
    return 1;
}

int authenticateUser(const struct User users[], int userCount, const char *username, const char *password)
{
    int userIndex = findUserIndex(users, userCount, username);
    if (userIndex != -1 && strcmp(password, users[userIndex].password) == 0)
    {
        return 1;
    }
    return 0;
}

int main()
{
    struct Contact contacts[MAX_CONTACTS];
    int contactCount = 0;

    struct User users[MAX_USERS];
    int userCount = 0;

    int loggedIn = 0;
    char currentUser[MAX_USERNAME_LEN];

    int choice;

    loadContactsFromFile(contacts, &contactCount);
    loadUsersFromFile(users, &userCount);

    while (1)
    {
        printf("\nContact Manager Menu:\n");
        if (!loggedIn)
        {
            printf("1. Register\n");
            printf("2. Login\n");
        }
        else
        {
            printf("1. Add Contact\n");
            printf("2. View Contacts\n");
            printf("3. Delete Contact\n");
            printf("4. Logout\n");
        }
        printf("0. Exit\n");

        if (loggedIn)
        {
            printf("Logged in as: %s\n", currentUser);
        }

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            if (!loggedIn)
            {
                if (registerUser(users, &userCount))
                {
                    saveUsersToFile(users, userCount);
                }
            }
            else
            {
                if (loggedIn)
                {
                    addContact(contacts, &contactCount, currentUser);
                    saveContactsToFile(contacts, contactCount);
                }
                else
                {
                    printf("You need to be logged in to add a contact.\n");
                }
            }
            break;
        case 2:
            if (!loggedIn)
            {
                char username[MAX_USERNAME_LEN], password[MAX_PASSWORD_LEN];
                printf("Enter username: ");
                scanf("%s", username);
                printf("Enter password: ");
                scanf("%s", password);

                if (authenticateUser(users, userCount, username, password))
                {
                    printf("Login successful!\n");
                    strcpy(currentUser, username);
                    loggedIn = 1;
                }
                else
                {
                    printf("Login failed. Invalid username or password.\n");
                }
            }
            else
            {
                viewContacts(contacts, contactCount, currentUser);
            }
            break;
        case 3:
            if (loggedIn)
            {
                viewContacts(contacts, contactCount, currentUser);
                deleteContact(contacts, &contactCount, currentUser);
                saveContactsToFile(contacts, contactCount);
            }
            else
            {
                printf("You need to be logged in to delete a contact.\n");
            }
            break;
        case 4:
            if (loggedIn)
            {
                loggedIn = 0;
                currentUser[0] = '\0';
                printf("Logged out successfully.\n");
            }
            else
            {
                printf("You are not logged in.\n");
            }
            break;
        case 0:
            printf("Exiting Contact Manager. Goodbye!\n");
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
