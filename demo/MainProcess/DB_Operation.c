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

void DB_GetUserPermission(char* pUserID, unsigned char* pUserPermission)
{
    // select user_role.userid,role_permission.permissionid from user_role left join role_permission on user_role.roleid=role_permission.roleid where user_role.userid='admin'
    

    int ret ;
    ret = Sqlite_QueryPermissionByUserID(pUserID, pUserPermission);
    if( ret == -1 )
    {
        DEBUG_LOG("数据库打开错误");
        return false;
    }

    return true;
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
    if(!sql)
    {

        DEBUG_LOG("Not Enough Memory For sql in DB_GetConfigInfo");
        goto malloc_err;
    }
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
    if(!*result)
    {
        DEBUG_LOG("Not Enough Memory For *result in DB_GetConfigInfo");
        goto malloc_err;
    }
	memset(*result,0,sizeof(data));
	strcpy(*result, data);
    
    //DEBUG_LOG(result);
    
    free(sql);
    sql = NULL;
    return ret;

malloc_err:
    MALLOC_ERR;
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
    if(!sql)
    {
        DEBUG_LOG("Not Enough Memory For sql in DB_GetUserNameByUserID");
        goto malloc_err;
    }
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
    if(!*result)
    {
        DEBUG_LOG("Not Enough Memory For *result in DB_GetUserNameByUserID");
        goto malloc_err;
    }
    memset(*result,0,sizeof(data));
    strcpy(*result, data);
    
    //DEBUG_LOG(result);
    
    free(sql);
    sql = NULL;
    return ret;

malloc_err:
    MALLOC_ERR;
}

bool DB_GetUserIDByFPID(int fpID, char** result)
{
    int ret ;
    char* userid = NULL;
    ret = Sqlite_QueryUserIDByFPID(fpID, &userid);
    if( ret == -1 )
    {
        DEBUG_LOG("数据库打开错误");
        return false;
    }
    else if( ret == 0){
        DEBUG_LOG("该指纹没有对应的用户");
        *result = NULL;
        return false;
    }
    *result = (char*)malloc(sizeof(userid));    
    if(!*result)
    {
        DEBUG_LOG("Not Enough Memory For *result in DB_GetUserIDByFPID");
        goto malloc_err;
    }
    memset(*result,0,sizeof(userid));
    strcpy(*result,userid);

    return true;
    
malloc_err:
    MALLOC_ERR;
}

bool DB_OpenLoginPermission(char* pUserID)
{
    int ret;
    ret = Sqlite_InsertLoginPermission(pUserID);
    if( ret == -1 )
    {
        DEBUG_LOG("数据库打开错误");
        return false;
    }

    return true;
}

bool DB_CloseLoginPermission(char* pUserID)
{
    int ret;
    ret = Sqlite_DeleteLoginPermission(pUserID);
    if( ret == -1 )
    {
        DEBUG_LOG("数据库打开错误");
        return false;
    }

    return true;
}






