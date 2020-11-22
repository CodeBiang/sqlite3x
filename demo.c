#include "sqlite3x.h"
#include <stdio.h>

const sqlite3x_map_t tmap[] = {
    {
        .field = "aid",
        .type = SQLITE3X_INTEGER,
        .cons = {
            .foreign = {
                .tbl = "table_a",
                .enable = true,
                .field = "id",
                .on_delete = SQLITE3X_CASCADE,
                .on_update = SQLITE3X_CASCADE
            }
        }
    }, {
        .field = "id",
        .key = true,
        .autoincrement = true,
        .type = SQLITE3X_INTEGER
    }, {
        .field = "0"
    }
};

const sqlite3x_tbl_t ttbl = {
    .name = "table_b",
    .maps = tmap
};

int main(void) {
    sqlite3* pdb;
    printf("??\n");
    sqlite3_open("./test.db3", &pdb);

    sqlite3x_init_table(pdb, &ttbl);

    sqlite3_close_v2(pdb);
    return 0;
}

