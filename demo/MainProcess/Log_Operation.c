#include"main.h"
// 业务日志：<操作人员><执行的操作><结构>
void Log_GenerateYW(char* pUserID, char* pActionType, char* pResultType, char**retContent)
{
	//char *pContent = NULL;
	char* pUserName = NULL;
	int len = 0;

	DB_GetUserNameByUserID(pUserID, &pUserName);

	DEBUG_LOG("pUserName == ");
	DEBUG_LOG(pUserName);

	len = sizeof(pUserName) + sizeof(pActionType) + sizeof(pResultType) + 20;

	*retContent = (char*) malloc (len);
	memset(*retContent, 0, len);
	strcat(*retContent,pUserName);
	strcat(*retContent,pActionType);
	strcat(*retContent,pResultType);

	DEBUG_LOG("YWLOG = ");
	DEBUG_LOG(*retContent);
}
// 报警日志：<报警类型>
void Log_GenerateBJ()
{

}
// 管理日志：<操作人员><执行操作><操作对象><结果>
void Log_GenerateGL()
{

}