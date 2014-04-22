#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>
#include <readline/readline.h>
#include <readline/history.h>

#define DBNAME "mkaiser4"
#define DBUSER "mkaiser4"
#define DBPASS "munt6502"

/* Compile with: clang tweet.c -o tweet -I /usr/include/postgresql -l readline -l pq */

int main(int argc, char *argv[])
{

    PGconn *db;
    PGresult *res;

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

    // Get tweet from user
    char *message;
    message = readline("Enter tweet: ");

    // Construct query
    char query[300];
    sprintf(query, "insert into tweet values (default, 'mkaiser4', '%s', now(), null, null)", message);
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

    // Get all tweets from database
    res = PQexec(db, "select msg from tweet");
    int rows = PQntuples(res);
    printf("Getting %d rows\n", rows);

    for (int i = 0; i < rows; i++)
    {
        char *msg = PQgetvalue(res, i, 0);
	printf("%s\n", msg);
    }

    // Clean up
    PQclear(res);
    PQfinish(db);
}
