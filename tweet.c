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
    char *username;
    float lat=0.00;
    float lon=0.00;
    char *ll;

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
		int length = strlen(message);
		if (length > 140)
		{
		    message[140]='\0';
		}
		printf("Enter latitude, longitude:\n");
		{
		scanf("%f, %f", &lat, &lon);	
		}
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
		    sprintf(query, "select msg, ts, lat, lon, username from tweet order by ts desc");
                    res = PQexec(db, query);
		    
		    int rows = PQntuples(res);
		    printf("\n");
		    for (int i = 0; i < rows; i++)
		    {
		        char *msg = PQgetvalue(res, i, 0);
		        char *tim = PQgetvalue(res, i, 1);
		        char *la = PQgetvalue(res, i, 2);
		        char *lo = PQgetvalue(res, i, 3);
			char *un = PQgetvalue(res, i, 4);
			printf("@%s\n", un);
			printf("%s\n", msg);
		        printf("%.16s", tim);
		        if(la[0])
		        {
		        printf(" (%s, %s)", la, lo);
		        }
		        printf("\n\n");
		    }
		    printf("\n");
		}       
	        PQclear(res);
                break;

	    case 'U':
            case 'u':
                // Handle 'u' case: Read tweets from a particular user
 		{
		    username = readline("Enter user: ");
		    
		    struct passwd *pw;
		    pw = getpwnam(username);
		    if (!pw)
		    {
			printf("Unknown user.\n");
		    }
		    else
		    {
			
			sprintf(query, "select msg, ts, lat, lon, username from tweet where username = '%s' order by ts desc", username);
                        res = PQexec(db, query);

                        int rows = PQntuples(res);
			printf("\n");
                        for (int i = 0; i < rows; i++)
                        {
			    char *un = PQgetvalue(res, i, 4);
			    printf("@%s\n", un);
			    char *msg = PQgetvalue(res, i, 0);
                            printf("%s\n", msg);
			    char *tim = PQgetvalue(res, i, 1);
			    char *la = PQgetvalue(res, i, 2);
			    char *lo = PQgetvalue(res, i, 3);
			    printf("%.16s", tim);
			    if(la[0])
			    {
			        printf(" (%s, %s)", la, lo);
			    }
			    printf("\n\n");
                        }
                        printf("\n");

		    }		    
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

    // Clean up
    PQclear(res);
    PQfinish(db);
}

