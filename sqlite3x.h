#ifndef __SQLITE3X_H_
#define __SQLITE3X_H_

#ifdef __GNUC__
#define SQLITE3X_USE_GNU
#endif

#ifdef SQLITE3X_USE_GNU
#define __SQLITE3X_NONNULL(params) __nonnull(params)
#endif

#include <stddef.h>
#include <stdbool.h>
#include "sqlite3/sqlite3.h"


#define SQLITE3X_ERR_CODE_MAP(XX)                       \
    XX(NONE, 0, "success")                              \
    XX(MEMORY, -1, "malloc failed")                     \
    XX(ARGS, -2, "args error")                          \
    XX(OVERFLOW, -3, "value overflow")                  \
    XX(SQLITE, -4, "sql exec err")

enum sqlite3x_err_code {
#define XX(name, code, desc)    ERR_SQLITE3X_##name = code,
    SQLITE3X_ERR_CODE_MAP(XX)
#undef XX
};

#define SQLITE3X_TYPE_MAP(XX)                           \
    XX(NULL, 0, "NULL")                                 \
    XX(INTEGER, 100, "INTEGER")                         \
    XX(REAL, 200, "REAL")                               \
    XX(TEXT, 300, "TEXT")                               \
    XX(BLOB, 400, "BLOB")                               

enum sqlite3x_type {
#define XX(name, code, desc)    SQLITE3X_##name = code,
    SQLITE3X_TYPE_MAP(XX)
#undef XX
};

typedef struct sqlite3x_map sqlite3x_map_t;
typedef struct sqlite3x_tbl sqlite3x_tbl_t;
typedef struct sqlite3x_constraint sqlite3x_cons_t;
typedef struct sqlite3x_foreign sqlite3x_foreign_t;


enum foreign_action {
    SQLITE3X_NO_ACTION = 0,
    SQLITE3X_RESTRICT,
    SQLITE3X_SET_NULL,
    SQLITE3X_SET_DEFAULT,
    SQLITE3X_CASCADE
};

struct sqlite3x_foreign {
    bool enable;
    char* tbl;                  /*!-- Reference table name --*/
    char* field;                /*!-- Reference field --*/
    enum foreign_action on_delete;  /*!-- Foreign key delete --*/
    enum foreign_action on_update;  /*!-- Foreign key update --*/
};

struct sqlite3x_constraint {
    bool notnull;
    bool unique;
    char* sql_field;            /*!-- Sql field reflection --*/
    char* check;                /*!-- Sql check constraint --*/
    char* sql_default;          /*!-- Sql default value --*/
    sqlite3x_foreign_t foreign; /*!-- Sql foreign key --*/
};

struct sqlite3x_map {
    const char* field;          /*!-- Data field --*/
    enum sqlite3x_type type;

    bool key;                   /*!-- Primary key --*/
    bool autoincrement;         /*!-- Autoincrement --*/
    sqlite3x_cons_t cons;       /*!-- Constraint --*/
};

struct sqlite3x_tbl {
    const char* name;           /*!-- Table name --*/
    const sqlite3x_map_t* maps; /*!-- Mappings --*/
};



/*** Private Definitions ***/

#define _sqlite3x_offset(type, field)             (size_t)(&(((type*)0)->field))
#define _sqlite3x_size(type, field)               (sizeof(((type*)0)->field))
#define _sqlite3x_ptr_item_size(type, field)      (sizeof(*(((type*)0)->field)))

const char* sqlite3x_type_str(enum sqlite3x_type type);

int sqlite3x_init_table(sqlite3* db, const sqlite3x_tbl_t* tbl);

#endif
