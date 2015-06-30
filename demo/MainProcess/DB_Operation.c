#include"main.h"
#include<stdbool.h>

#define SQLITE_TABLE_SBSETTING "sbsetting"

int DB_GetUserOperationStatus()
{
    int ret = USER_OPERATION_STATUS_NONE;
    DEBUG_LOG("USER_OPERATION_STATUS_NONE");
    return ret;
}

bool DB_CheckApprovalAvailable()
{
    bool ret = false;
    DEBUG_LOG("ApprovalAvailable == false");
    return ret;
}

void DB_LogWrite(int type, char* content)
{

    //insert into gllog(content) values('');
    //char* sql = 0;
    //char* insert = "insert into ";
    char* table ;
    //char* values = "(content) values('";
    //char* end = "');";
    //int len = 0;
    
    switch(type)
    {
        case LOG_TYPE_BUSINESS:
            table = LOG_TABLE_BUSINESS;
            break;
        case LOG_TYPE_ALARM:
            break;
        case LOG_TYPE_MANAGER:
            break;
        default:
            table = NULL;
            break;
    }
    //len = sizeof(insert) + sizeof(table) + sizeof(values) + sizeof(content) + sizeof(end) + 120;
    //sql = (char*)malloc(len*sizeof(char));
    //memset(sql, 0, sizeof(sql));
    //strcat(sql, insert);
    //strcat(sql, table);
    //strcat(sql, values);
    //strcat(sql, content);
    //strcat(sql, end);
    //DEBUG_LOG("sql == ");
    //DEBUG_LOG(sql);
    //Sqlite_InsertOne(sql);

    DEBUG_LOG("write log");
    //free(sql);

    Sqlite_InsertLog(table, content);
}

void DB_GetUserPermission()
{
}

int DB_GetConfigInfo(char* config, char** result)
{
    int ret = 0;
    char* data = "";
	int i,j,nrow, ncolumn,index;

    // 生成sql语句
    char *sql = NULL;
    char *table = DB_TABLE_NAME_CONFIG;
    int len = sizeof("select ")+sizeof(config)+sizeof(" from ")+sizeof(table)+sizeof(" where id=`set`")+6;
    sql = (char*)malloc(len*sizeof(char));
    memset(sql, 0, sizeof(sql));
    strcat(sql,"select ");
    strcat(sql,config);
    strcat(sql," from ");
    strcat(sql,table);
    strcat(sql," where id='set'");
    strcat(sql,"\0");
    //DEBUG_LOG(sql);
    Sqlite_QueryOne(sql, &data, &nrow, &ncolumn);
    //getch();
    *result = (char*)malloc(sizeof(data));
	memset(*result,0,sizeof(data));
	strcpy(*result, data);
    
    //DEBUG_LOG(result);
    
    free(sql);
    return ret;
}

int DB_GetUserNameByUserID(char* pUserID, char** result)
{
    int ret = 0;
    char* data = "";
    int i,j,nrow, ncolumn,index;

    // 生成sql语句
    char *sql = NULL;
    char *table = DB_TABLE_NAME_USER;
    int len = sizeof("select name") + sizeof(" from ") + sizeof(table) + sizeof(" where id='") + sizeof(pUserID) + sizeof("'")+6;
    sql = (char*)malloc(len*sizeof(char));
    memset(sql, 0, sizeof(sql));
    strcat(sql,"select name");
    strcat(sql," from ");
    strcat(sql,table);
    strcat(sql," where id='");
    strcat(sql,pUserID);
    strcat(sql,"'");
    strcat(sql,"\0");
    //DEBUG_LOG(sql);
    Sqlite_QueryOne(sql, &data, &nrow, &ncolumn);
    //getch();
    *result = (char*)malloc(sizeof(data));
    memset(*result,0,sizeof(data));
    strcpy(*result, data);
    
    //DEBUG_LOG(result);
    
    free(sql);
    return ret;
}
