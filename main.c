#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <math.h>
#include <ctype.h>
#include <memory.h>
#include <string.h>

// sql callback
static int callback(void *unused, int count, char **data, char **field_name) {
    for (int i = 0; i < count; i++) {

        field_name[i][0] = toupper(field_name[i][0]); // set the first character of the field to be 
                                                      // uppercase (e.g. "title" becomes "Title")

        // if the thing being printed is bytes then convert it to gib and print that instead...
        if ((i + 1) == 2) {
            double gibs = (double)strtoull(data[i], NULL, 0) / 1.074e9;
            printf("%s: %0.2f GiB (%s bytes)\n", field_name[i], gibs, data[i]);
        } else {
            printf("%s: %s\n", field_name[i], data[i]);
        }
    }

    // print a new line once the elements have been printed
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");

    return 0;
}

int main(void) {
    sqlite3 *db = NULL;
    char *error_msg = '\0';
    
    // open database
    if (sqlite3_open_v2("E:\\Torrents\\rarbg_db.sqlite", &db, SQLITE_OPEN_READONLY, NULL) != SQLITE_OK) {
        printf("Error opening database: %i\n", sqlite3_errcode(db));
        return 1;
    } else {
        printf("Database succesfully opened.\n\n");
        printf("If there is no data shown on screen, then no data is present.\n");
    }

    while (1) {
        // input
        char input[32] = {0};
        printf("Enter the IMDb identifier: ");
        scanf_s("%31s", input, (unsigned)sizeof(input));

        // create new sql query
        char query[100] = {0};
        sprintf_s(query, 99, "SELECT title, size, magnetLink FROM magnet_links WHERE imdb=\"%s\";", input);

        printf("\nExecuting the SQL query now...\n\n");

        // execute
        int result = sqlite3_exec(db, query, callback, 0, &error_msg);

        // clear input and ask the user if they want to go again
        memset(input, 0, sizeof(input));
        memset(query, 0, sizeof(query));

        while (1) {
            printf("Would you like to find another movie? (Y or N): ");
            scanf_s("%2s", input, (unsigned)sizeof(input));

            if (strncmp(input, "N", 2) == 0 || strncmp(input, "n", 2) == 0) {
                goto end; // jump instwuction????
            } else if (strncmp(input, "Y", 2) == 0 || strncmp(input, "y", 2) == 0) {
                break;
            } else {
                printf("Invalid option.\n");
            }
        }
    }

end:
    // cleanup
    sqlite3_free(error_msg);
    sqlite3_close(db);

    #ifdef _WIN32
    system("pause");
    #endif

    return 0;
}