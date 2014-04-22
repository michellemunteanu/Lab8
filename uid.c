#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

void menu()
{
    printf("i) Print out my information\n");
    printf("u) Print out another user's information\n");
    printf("q) Quit\n");
}

void print_my_info()
{
    // Get user's ID
    uid_t uid = getuid();

    // Get user information
    struct passwd *pw;
    pw = getpwuid(uid);

    // Print out user information
    printf("Username: %s\n", pw->pw_name);
    printf("User ID: %d\n", pw->pw_uid);
    printf("Full name: %s\n", pw->pw_gecos);
    printf("Home directory: %s\n", pw->pw_dir);
}

int main()
{
    while(1)
    {
        menu();

        char *choice = readline("Your choice: ");
        switch(choice[0])
        {
            case 'I':
            case 'i':
                // Handle 'i' case
                print_my_info();
                break;

            case 'U':
            case 'u':
               // Handle 'u' case
               {
                   char *username;
                   username = readline("Username to look up: ");

                   // Get user information
                   struct passwd *pw;
                   pw = getpwnam(username);

                   if (!pw)
                   {
                       printf("Unknown user.\n");
                   }
                   else
                   {
                       // Print out user information
                       printf("User ID: %d\n", pw->pw_uid);
                       printf("Full name: %s\n", pw->pw_gecos);
                       printf("Home directory: %s\n", pw->pw_dir);
                   }
                }
                break;

            case 'Q':
            case 'q':
               // Handle 'q' case
               exit(0);
               break;

            default:
                printf("Invalid choice.\n");
               // Handle everything else

        }
    }

    return 0;
}
