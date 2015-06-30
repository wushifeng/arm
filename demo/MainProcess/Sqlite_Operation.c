#include <stdio.h>
#include <string.h>
#include "sqlite3.h"

#define DB_FILE_PATH "/home/sis/www/web/home.db"
//数据库操作
int Sqlite_QueryOne(char* sql, char** result, int* pNrow, int *pNcolumn)
{
    char **data = "";
	char *errmsg = 0;
	int ret = 0;
	sqlite3 *db = 0;

	// OPEN DATABASE
	ret = sqlite3_open(DB_FILE_PATH,&db);
	if(ret != SQLITE_OK){
		//fprintf(stderr, "Cannot open db: %s\n",sqlite3_errmsg(db));
		return 0;
	}
	//printf("Open database\n");
	

	// QUERY DATA
	ret = sqlite3_get_table(db, sql, &data, pNrow, pNcolumn, &errmsg);
	if(ret == SQLITE_OK)
	{
		if(pNrow>0)
		{
            *result = (char*)malloc(sizeof(data[1]));
		    memset(*result,0,sizeof(data[1]));
            strcpy(*result, data[1]);
		}
	}

	// FREE MEMORY
	//sqlite3_free_table(data);
	sqlite3_free(errmsg);

	// CLOSE DATABASE
	sqlite3_close(db);	
	//printf("Close database\n");
	

	return 1;
}

int Sqlite_InsertOne(char* sql)
{
    int ret = 0;
	char *errmsg = 0;
    sqlite3 *db = 0;

	// OPEN DATABASE
	ret = sqlite3_open(DB_FILE_PATH,&db);
	if(ret != SQLITE_OK){
		//fprintf(stderr, "Cannot open db: %s\n",sqlite3_errmsg(db));
		return 0;
	}
	//printf("Open database\n");

    sqlite3_exec(db, sql, 0, 0, &errmsg);
    
	// CLOSE DATABASE
	sqlite3_close(db);	
	//printf("Close database\n");
	

	return 1;
}

int Sqlite_InsertLog(char* table, char* content)
{
	char sql[80]="";
	sqlite3* db = 0;
	sqlite3_stmt * stmt;  
	sprintf(sql, "insert into %s(content) values(?)", table);
    int ret = 0;
    ret = sqlite3_open(DB_FILE_PATH,&db);
	if(SQLITE_OK != ret)  
    {  
        return -1; 
    }  

    ret = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);  
    if(SQLITE_OK != ret)  
    {  
        sqlite3_finalize(stmt); 
        return -1; 
    }  
	sqlite3_bind_text( stmt, 1, content, -1, SQLITE_STATIC ); 
	ret = sqlite3_step( stmt );  
	if (( ret != SQLITE_DONE )&&( ret != SQLITE_ROW )) 
	{
		return -1;  
	}

	sqlite3_finalize( stmt );  
	sqlite3_close(db);	
}
