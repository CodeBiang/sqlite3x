#include <assert.h>
#include <stdio.h>
#include "sqlite3/sqlite3.h"
#include "sqlite3x.h"

const char* sqlite3x_type_str(enum sqlite3x_type type) {
    switch (type) {
#define XX(name, code, str) case SQLITE3X_##name: return str;
        SQLITE3X_TYPE_MAP(XX)
#undef XX
    }
    return "UNKNOWN";
}

const char* sqlite3x_foreign_action_str(enum foreign_action action) {
    switch (action) {
        case SQLITE3X_NO_ACTION: return "NO ACTION";
        case SQLITE3X_RESTRICT: return "RESTRICT";
        case SQLITE3X_SET_NULL: return "SET NULL";
        case SQLITE3X_SET_DEFAULT: return "SET DEFAULT";
        case SQLITE3X_CASCADE: return "CASCADE";
    }
    return "UNKNOWN";
}

/**
 * 初始化数据表
 * 
 * @param db 数据库实例 
 * @param tbl 数据表映射
 * @return errcode
 */
int sqlite3x_init_table(sqlite3* db, const sqlite3x_tbl_t* tbl) {
    assert(tbl && tbl->name && tbl->maps);

    sqlite3_str* str = sqlite3_str_new(db);
    sqlite3_str_appendf(str, "CREATE TABLE IF NOT EXISTS %s (", tbl->name);

    for (int i = 0; tbl->maps[i].field[0] != '0'; i++) {
        const sqlite3x_map_t* map = &tbl->maps[i];
        sqlite3_str_appendf(str, "%s", i != 0 ? ",\n" : "\n");
        // field type 
        sqlite3_str_appendf(str, "%s %s", 
                map->cons.sql_field ? map->cons.sql_field : map->field,
                sqlite3x_type_str(map->type));
        // key
        if (map->key) {
            sqlite3_str_appendf(str, " PRIMARY KEY%s",
                    map->autoincrement ? " AUTOINCREMENT" : "");
        }
        // constraint
        sqlite3_str_appendf(str, "%s%s",
                map->cons.unique ? " UNIQUE" : "",
                map->cons.notnull ? " NOT NULL" : "");
        if (map->cons.check)
            sqlite3_str_appendf(str, " CHECK(%s)",
                    map->cons.check);
        if (map->cons.sql_default)
            sqlite3_str_appendf(str, " DEFAULT ",
                    map->cons.sql_default);
    }

    for (int i = 0; tbl->maps[i].field[0] != '0' && tbl->maps[i].cons.foreign.enable; i++) {
        const sqlite3x_map_t* map = &tbl->maps[i];
        sqlite3_str_appendf(str, ",\nFOREIGN KEY(%s) REFERENCES %s(%s) ON DELETE %s ON UPDATE %s",
                map->cons.sql_field ? map->cons.sql_field : map->field,
                map->cons.foreign.tbl,
                map->cons.foreign.field,
                sqlite3x_foreign_action_str(map->cons.foreign.on_delete),
                sqlite3x_foreign_action_str(map->cons.foreign.on_update));
    }
    
    sqlite3_str_appendf(str, "\n);");
    printf("%s\n", sqlite3_str_value(str));
    char* p = NULL;
    int ret;
    if ((ret = sqlite3_exec(db, sqlite3_str_value(str), NULL, NULL, &p)) != SQLITE_OK) {
        printf("ret : %d\n", ret);
        if (!p) {
            printf("err : %s\n", p);
            sqlite3_free(p);
        }
    }
    sqlite3_free(p);
    sqlite3_str_finish(str);
    return ERR_SQLITE3X_NONE;
}
