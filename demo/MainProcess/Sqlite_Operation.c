#include <stdio.h>
#include <string.h>
#include "sqlite3.h"
#include "main.h"

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
			if(!*result)
			{
				DEBUG_LOG("Not Enough Memory For result in Sqlite_QueryOne");
				goto malloc_err;
			}
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

malloc_err:
	MALLOC_ERR;
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

int Sqlite_QueryUserIDByFPID(int fpID, char** result)
{
	char sql[80]="";
	sqlite3* db = 0;
	char *rowData ;
	sqlite3_stmt * stmt;  
    sprintf(sql, "select userid from user_fingerprint where fingerprintid=(?)");
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
		sqlite3_close(db);	
        return -1; 
    }  
	sqlite3_bind_int( stmt, 1, fpID ); 
	ret = sqlite3_step( stmt );  
	if ( ret == SQLITE_ROW )
	{
		rowData = (char *)sqlite3_column_text(stmt, 0);
		*result = (char*)malloc(sizeof(rowData));
		memset(*result,0,sizeof(rowData));
		strcpy(*result,rowData);
	}
	else if( ret == SQLITE_DONE )
	{
		*result = NULL;
		return 0;
	}
	if (( ret != SQLITE_DONE )&&( ret != SQLITE_ROW )) 
	{
		return -1;  
	}

	sqlite3_finalize( stmt );  
	sqlite3_close(db);	
	return 1;
}

int Sqlite_QueryPermissionByUserID(char* pUserID, unsigned char* pUserPermission)
{
	char sql[200]="";
	sqlite3* db = 0;
	char *rowData ;
	sqlite3_stmt * stmt;  
    sprintf(sql, "select user_role.userid,role_permission.permissionid from user_role left join role_permission on user_role.roleid=role_permission.roleid where user_role.userid=(?)");
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
		sqlite3_close(db);	
        return -1; 
    }  
	sqlite3_bind_text( stmt, 1, pUserID, -1, SQLITE_STATIC ); 
	while(1)
	{
		ret = sqlite3_step( stmt );  
		if ( ret == SQLITE_ROW )
		{
			rowData = (char *)sqlite3_column_text(stmt, 1);
			if(strcmp(rowData, PERMISSION_OPEN_ID) == 0){
				*pUserPermission|= PERMISSION_OPEN;
			}
			else if(strcmp(rowData, PERMISSION_STATUS_INFO_ID) == 0){

			}
			else if(strcmp(rowData, PERMISSION_JUDGE_OFFLINE_ID) == 0){

			}
			else if(strcmp(rowData, PERMISSION_SUPERVISE_OPEN_ID) == 0){
				*pUserPermission|= PERMISSION_SUPERVISE_OPEN;
			}
			else if(strcmp(rowData, PERMISSION_PARAMS_SETTING_ID) == 0){
				*pUserPermission|= PERMISSION_LOGIN;
			}
			else if(strcmp(rowData, PERMISSION_USER_MANAGER_ID) == 0){
				*pUserPermission|= PERMISSION_LOGIN;
			}
			else if(strcmp(rowData, PERMISSION_NETWORK_SETTING_ID) == 0){
				*pUserPermission|= PERMISSION_LOGIN;
			}
			else if(strcmp(rowData, PERMISSION_PERMISSION_ALLOC_ID) == 0){
				*pUserPermission|= PERMISSION_LOGIN;
			}
			else if(strcmp(rowData, PERMISSION_ALARM_LOG_AUDIT_ID) == 0){
				*pUserPermission|= PERMISSION_LOGIN;
			}
			else if(strcmp(rowData, PERMISSION_BUSINESS_LOG_AUDIT_ID) == 0){
				*pUserPermission|= PERMISSION_LOGIN;
			}
			else if(strcmp(rowData, PERMISSION_SECURITY_OFFICER_LOG_AUDIT_ID) == 0){
				*pUserPermission|= PERMISSION_LOGIN;
			}
			else if(strcmp(rowData, PERMISSION_SYSTEM_ADMIN_LOG_AUDIT_ID) == 0){
				*pUserPermission|= PERMISSION_LOGIN;
			}
			//printf("%s\n",rowData);
		}
		else if( ret == SQLITE_DONE )
		{
			break;
			//return 0;
		}
		if (( ret != SQLITE_DONE )&&( ret != SQLITE_ROW )) 
		{
			return -1;  
		}
		//free(rowData);
	}
	

	sqlite3_finalize( stmt );  
	sqlite3_close(db);	
	return 1;
}

int Sqlite_InsertLoginPermission(char* pUserID)
{
	char sql[80]="";
	sqlite3* db = 0;
	sqlite3_stmt * stmt;  
	sprintf(sql, "insert into loginpermission (userid) values(?)");
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
	sqlite3_bind_text( stmt, 1, pUserID, -1, SQLITE_STATIC ); 
	ret = sqlite3_step( stmt );  
	if (( ret != SQLITE_DONE )&&( ret != SQLITE_ROW )) 
	{
		return -1;  
	}

	sqlite3_finalize( stmt );  
	sqlite3_close(db);	
}


int Sqlite_DeleteLoginPermission(char* pUserID)
{
	char sql[80]="";
	sqlite3* db = 0;
	sqlite3_stmt * stmt;  
	sprintf(sql, "delete from loginpermission where userid=(?)");
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
	sqlite3_bind_text( stmt, 1, pUserID, -1, SQLITE_STATIC ); 
	ret = sqlite3_step( stmt );  
	if (( ret != SQLITE_DONE )&&( ret != SQLITE_ROW )) 
	{
		return -1;  
	}

	sqlite3_finalize( stmt );  
	sqlite3_close(db);	
}