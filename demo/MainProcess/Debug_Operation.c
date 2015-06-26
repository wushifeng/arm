#include<stdio.h>
void LOG(char* log)
{
    // 生成Log文件
    FILE* fp = fopen("Log.txt","a+");
    fprintf(fp, "%s\n",log);
    fclose(fp);
}
