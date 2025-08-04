#if defined(_WIN32) || defined(_WIN64)
#include <conio.h>
#else
#include <termios.h>
#endif
#include <string.h>
#include "header.h"

char *USERS = "./data/users.txt";

void loginMenu(char a[50], char pass[50])
{
    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t Username:");
    scanf("%s", a);

    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");

#if defined(_WIN32) || defined(_WIN64)
    int i = 0;
    char ch;
    while ((ch = _getch()) != '\r' && i < 49) // Enter key
    {
        if (ch == '\b' && i > 0) // handle backspace
        {
            i--;
            printf("\b \b");
        }
        else if (ch != '\b')
        {
            pass[i++] = ch;
            printf("*");
        }
    }
    pass[i] = '\0';
    printf("\n");
#else
    struct termios oflags, nflags;
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;
    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        exit(1);
    }
    scanf("%s", pass);
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        exit(1);
    }
#endif
}

void registerMenu(char a[50], char pass[50])
{
    FILE *fp;
    struct User userChecker;
    int maxId = -1;
    char tempName[50];
    char tempPass[50];
    int userExists = 0;
    int retryOption = 0;

    do {
        userExists = 0;
        system("clear");
        printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Registration:\n");
        printf("Enter your username: ");
        scanf("%s", tempName);

        // Check if username already exists
        if ((fp = fopen("./data/users.txt", "r")) != NULL)
        {
            while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
            {
                if (strcmp(userChecker.name, tempName) == 0)
                {
                    userExists = 1;
                }
                if (userChecker.id > maxId) maxId = userChecker.id;
            }
            fclose(fp);
        }

        printf("Enter your password: ");
#if defined(_WIN32) || defined(_WIN64)
        int i = 0;
        char ch;
        while ((ch = _getch()) != '\r' && i < 49)
        {
            if (ch == '\b' && i > 0)
            {
                i--;
                printf("\b \b");
            }
            else if (ch != '\b')
            {
                tempPass[i++] = ch;
                printf("*");
            }
        }
        tempPass[i] = '\0';
        printf("\n");
#else
        struct termios oflags, nflags;
        tcgetattr(fileno(stdin), &oflags);
        nflags = oflags;
        nflags.c_lflag &= ~ECHO;
        nflags.c_lflag |= ECHONL;
        if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
        {
            perror("tcsetattr");
            exit(1);
        }
        scanf("%s", tempPass);
        if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
        {
            perror("tcsetattr");
            exit(1);
        }
#endif

        if (userExists) {
            printf("\nUsername already exists!\n");
            printf("Enter 1 to try registering again or 0 to return to the main menu: ");
            scanf("%d", &retryOption);
            if (retryOption == 1) {
                continue;
            } else {
                // Clear the arrays and return to main menu
                strcpy(a, "");
                strcpy(pass, "");
                return;
            }
        } else {
            // Username is unique, proceed with registration
            // Save new user
            fp = fopen("./data/users.txt", "a");
            fprintf(fp, "%d %s %s\n", maxId + 1, tempName, tempPass);
            fclose(fp);

            strcpy(a, tempName);
            strcpy(pass, tempPass);
            printf("\nRegistration successful!\n");
            return;
        }
    } while (1);
}

const char *getPassword(struct User u)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            char *buff = userChecker.password;
            return buff;
        }
    }

    fclose(fp);
    return "no user found";
}