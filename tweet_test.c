#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>


#define DBNAME "mkaiser4"
#define DBUSER "mkaiser4"
#define DBPASS "munt6502"

/* Compile with: clang tweet.c -o tweet -I /usr/include/postgresql -l readline -l pq */


void menu()
{
    printf("p) Post a tweet\n");
    printf("a) Read all tweets\n");
    printf("u) Read tweets from a particular user\n");
    printf("q) Quit\n");
}

int main(int argc, char *argv[])
{

    PGconn *db;
    PGresult *res;
    char *message;
    char *user;
    float lat;
    float lon;

    // Connect to database
    db = PQsetdbLogin("localhost", "5432", NULL, NULL, DBNAME, DBUSER, DBPASS);
    if (PQstatus(db) == CONNECTION_OK)
    {
        printf("Connected!\n");
    } 
    else 
    {
        printf("Connection failed\n");
	exit(1);
    }
    
    while(1)
    {
        menu();  //display the menu and wait for the user to choose

        char *choice = readline("Your choice: ");
        switch(choice[0])
        {
            case 'P':
            case 'p':
                // Handle 'p' case: Post a Tweet
    		message = readline("Enter tweet: ");
		printf("Enter latitude, longitude:\n");
		scanf("%f,%f", &lat, &lon);	

	        // Construct query
	        char query[300];
	        sprintf(query, "insert into tweet values (default, 'mkaiser4', '%s', now(), '%f', '%f')", message, lat,lon);
	        res = PQexec(db, query);
	        if (PQresultStatus(res) == PGRES_COMMAND_OK)
	        {
		    printf("INSERT succeeded\n");
	        }
	        else
	        {
	            printf("INSERT failed\n");
	        }
	        PQclear(res);
                break;

            case 'A':
            case 'a':
               // Handle 'a' case: Read all tweets
               {
		     res = PQexec(db, "select msg from tweet");
		    int rows = PQntuples(res);
		    printf("\nGetting %d rows\n", rows);

		    for (int i = 0; i < rows; i++)
		    {
		        char *msg = PQgetvalue(res, i, 0);
		        printf("%s\n", msg);
		    }
			printf("\n");
		}       
	        PQclear(res);
                break;

	    case 'U':
            case 'u':
                // Handle 'u' case: Read tweets from a particular user
 		{
		    user = readline("Enter user: ");

		    sprintf(query, "select msg from tweet where username = '%s'", user);
                    res = PQexec(db, query);

                    int rows = PQntuples(res);
                    printf("\nGetting %d rows\n", rows);

                    for (int i = 0; i < rows; i++)
                    {
                        char *msg = PQgetvalue(res, i, 0);
                        printf("%s\n", msg);
                    }
                        printf("\n");
                }
	        PQclear(res);
                break;

            case 'Q':
            case 'q':
               // Handle 'q' case
               exit(0);
	       PQclear(res);
	       PQfinish(db); //close the connection
               break;

            default:
                printf("Invalid choice.\n");
               // Handle everything else

        }
}
}

/*

    // Clean up
    PQclear(res);

    PQfinish(db);
}
*/

