#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>


#define DBNAME "twitter"
#define DBUSER "mkaiser4"
#define DBPASS "munt6502"

/* Compile with: clang tweet.c -o tweet -I /usr/include/postgresql -l readline -l pq */


void menu()
{
    printf("p) Post a tweet\n");
    printf("a) Read all tweets\n");
    printf("u) Read tweets from a particular user\n");
    printf("s) Search\n");
    printf("q) Quit\n");
}

int main(int argc, char *argv[])
{

    PGconn *db;
    PGresult *res;
    char *message; //the tweet to be posted
    char *keyword; //keyword to search for
    char *username; //this username is used when searching for tweets
    char *lat;
    char *lon;
    char *login; //this is the username used to log in
    char *passwd;
    char query[500];


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
    
	login = readline("Enter username: ");
	passwd = getpass("Enter password: ");

	sprintf(query, "select count (*) from twuser where username = '%s' and password = md5('%s')", login, passwd);
	//select count (*) from twuser where username = 'michelle' and password = (md5('abcde'))
	res = PQexec(db, query); 
	int ver = atof(PQgetvalue(res, 0, 0));
        printf("ver %d\n", ver); //remove this later
	printf("passwd %s\n", passwd); //remove this later
	
	if (ver == 1)  //valid login
        {
	    printf("Hello, %s!\n", login);
        }
        else //invalid login
        {
            printf("Incorrect username or password\n");
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

		lat = readline("Enter latitude: ");
		lon = readline("Enter longitude: ");
		

		if ((strlen(lat)==0) || (strlen(lon)==0)) //if user doesn't include lat and lon		
		{
		    //sprintf(query, "insert into tweet values (default, 'mkaiser4', '%s', now(), NULL, NULL)", message);
		    sprintf(query, "insert into tweet values (default, '%s', '%s', now(), NULL, NULL)", login, message);
		}
		else
		{
	            //sprintf(query, "insert into tweet values (default, 'mkaiser4', '%s', now(), '%s', '%s')", message, lat, lon);
	            sprintf(query, "insert into tweet values (default, '%s', '%s', now(), '%s', '%s')", login, message, lat, lon);
		}
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
		    //sprintf(query, "select msg, ts, lat, lon, username from tweet order by ts desc");
		    sprintf(query, "select twuser.username, twuser.displayname, msg, ts, lat, lon from tweet join twuser on twuser.username = tweet.username order by ts desc");
                    res = PQexec(db, query);
		    
		    int rows = PQntuples(res);
		    printf("\n");
		    for (int i = 0; i < rows; i++)
		    {
		        char *msg = PQgetvalue(res, i, 2);
		        char *tim = PQgetvalue(res, i, 3);
		        char *la = PQgetvalue(res, i, 4);
		        char *lo = PQgetvalue(res, i, 5);
			char *un = PQgetvalue(res, i, 0);
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
			
			//sprintf(query, "select msg, ts, lat, lon, username from tweet where username = '%s' order by ts desc", username);
			sprintf(query, "select twuser.username, twuser.displayname, msg, ts, lat, lon from tweet join twuser on twuser.username = tweet.username where twuser.username = '%s' order by ts desc", username);
                        res = PQexec(db, query);

                        int rows = PQntuples(res);
			printf("\n");
                        for (int i = 0; i < rows; i++)
                        {
			    char *un = PQgetvalue(res, i, 0);
			    printf("@%s\n", un);
			    char *msg = PQgetvalue(res, i, 2);
                            printf("%s\n", msg);
			    char *tim = PQgetvalue(res, i, 3);
			    char *la = PQgetvalue(res, i, 4);
			    char *lo = PQgetvalue(res, i, 5);
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

	    case 'S':
            case 's':
                // Handle 's' case: Search tweets for a particular keyword
                {
                    keyword = readline("Enter keyword: ");
//need to handle when search results are empty

                        sprintf(query, "select twuser.username, twuser.displayname, msg, ts, lat, lon from tweet join twuser on twuser.username = tweet.username where msg like '%%%s%%' or twuser.username = '%s'", keyword, keyword);
                        res = PQexec(db, query);
			////*********
                        int rows = PQntuples(res);
                        printf("\n");
                        for (int i = 0; i < rows; i++)
                        {
                            char *un = PQgetvalue(res, i, 0);
                            printf("@%s\n", un);
			    char *dn = PQgetvalue(res, i, 1);
                            printf("%s\n", dn);
                            char *msg = PQgetvalue(res, i, 2);
                            printf("%s\n", msg);
                            char *tim = PQgetvalue(res, i, 3);
                            char *la = PQgetvalue(res, i, 4);
                            char *lo = PQgetvalue(res, i, 5);
                            printf("%.16s", tim);
                            if(la[0]) //this might need some fixing here--maybe eliminate?
                            {
                                printf(" (%s, %s)", la, lo);
                            }
                            printf("\n\n");
                        }
                        printf("\n");

                }
                PQclear(res);
                break;

//

            case 'Q':
            case 'q':
               // Handle 'q' case
               exit(1);
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

