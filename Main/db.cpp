
#include <string>
#include <sqlite3.h>
#include "db.h"

int index=0;
int callback(void *data, int argc, char **argv, 
                    char **azColName) {
    
    printf("%s\n",(const char *)data);
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("-------------%d-------------\n",index++);
    return 0;
}

int booking_test(int option) {

    sqlite3 *db;
    int rc;
    char *err;
    const char *sql;
    const char *data="callback function called";
    rc=sqlite3_open("booking.db",&db);
    if(rc!=SQLITE_OK) printf("db not opened!\n");

    if(option==DB_SHOW) {
        index=0;
        sql="select * from users;";
        rc=sqlite3_exec(db,sql,callback,(void *)data,&err);
        if(rc!=SQLITE_OK) { printf("err3 %s\n",err); sqlite3_free(err); }
        sqlite3_close(db);
        return 0;

    }
    if(option==DB_DROP) {
        sql="drop table users;";
        rc=sqlite3_exec(db,sql,0,0,&err);
        if(rc!=SQLITE_OK) { printf("err3 %s\n",err); sqlite3_free(err); }
        else printf("success drop table users\n");
        sqlite3_close(db);
        return 0;


    }
    if(option==DB_TABLE) {
        sql="create table if not exists users (id integer primary key,name text,age int);";
        rc=sqlite3_exec(db,sql,0,0,&err);
        if(rc!=SQLITE_OK) { printf("err %s\n",err); sqlite3_free(err); }
        sqlite3_close(db);
        return 0;

    }
    if(option==IS_USERS) {

        
        
        sql="SELECT EXISTS ( SELECT  name FROM   sqlite_schema   WHERE   type='table' AND  name='users1' );";
        //sql="PRAGMA table_info(users);";
        //sql="SELECT COUNT(*) FROM sqlite_master WHERE name='users';";
        //sql="select name from sqlite_master where name='users1'";
        rc=sqlite3_exec(db,sql,callback,0,&err);
        if(rc!=SQLITE_OK) { printf("err2 %s\n",err); sqlite3_free(err); }
        printf("is users %d\n",rc);
        sqlite3_close(db);
        return 0;
    }

    if(option==DB_DELETE) {
        sql="delete from users where id=3;";
        rc=sqlite3_exec(db,sql,0,0,&err);
        if(rc!=SQLITE_OK) { printf("DB_DELETE %s\n",err); sqlite3_free(err); }
        sqlite3_close(db);
        return 0;


    }
    if(option==DB_LAST) {
        sql="select * from sqlite_sequence where name= 'users';";
        rc=sqlite3_exec(db,sql,callback,0,&err);
        if(rc!=SQLITE_OK) { printf("DB_LAST %s\n",err); sqlite3_free(err); }
        sqlite3_close(db);
        return 0;


    }
    if(option==DB_UPDATE) {

        const char *sql;
        sql="update users set name='arya' where id=3;"
            "select * from users;";

        printf("update ----\n");

        rc=sqlite3_exec(db,sql,callback,(void *)data,&err);
        if(rc!=SQLITE_OK) { printf("DB_UPDATE %s\n",err); sqlite3_free(err); }
        printf("update rc=%d\n",rc);
        sqlite3_close(db);
        return 0;


    }

    if(option==DB_INSERT) {

        // sql="insert into users values(1,'a',1);"
        //     "insert into users values(2,'b',1);"
        //     "insert into users values(3,'c',1);";
        char sql[128];
        static int index=1;
        static int age=10;
        char name[6];
        sprintf(name,"Wizard%d",index++);
        //sprintf(sql,"insert into users values(%d,'%s',%d);",index++,name,age++);
        sprintf(sql,"insert into users(name,age) values('%s',%d);",name,age++);

        rc=sqlite3_exec(db,sql,0,0,&err);
        if(rc!=SQLITE_OK) { 
            printf("err3 %s\n",err); sqlite3_free(err);
            if(index) index--; 
            if(age) age--;
        
        }

        int last_id=sqlite3_last_insert_rowid(db);
        printf("The last row is %d\n",last_id);
        sqlite3_close(db);
        return 0;

    }
    return 0;
}


