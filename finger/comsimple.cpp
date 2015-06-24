
//http://blog.sina.com.cn/s/blog_559f6ffc0100087v.html
#include <stdio.h> //标准输入输出定义
#include <fcntl.h> //文件控制定义
#include <errno.h> //错误代码定义
#include <unistd.h> //unix标准函数定义
#include <termios.h>//定义串口结构termios，并包含<termbits.h>
#include <string.h> //字符串函数定义
#define SERIAL_PORT "/dev/ttyS0" //使用串口1；1，2分别对应/dev/ttyS0和/dev/ttyS1
#define BAUDRATE B9600 //波特率为9600bps
#define MAX_SIZE 255
int main()
{
 int int_SerialFD;
 char Buff_Content[MAX_SIZE];
 int int_Flag;
 struct termios OldTerm,NewTerm; //数据结构termios,这个在文件include\asm-arm\termios.h中定义
 
 int_SerialFD = open(SERIAL_PORT,O_RDWR|O_NOCTTY);//O_RDWR表示以读写方式打开串口，
 //O_NOCTTY表示该程序不想成为该段口的控制终端，成功返回文件描述符，错误返回-1
 
 if (int_SerialFD < 0) //出错显示错误
  {
   printf("Open serial port %s error",SERIAL_PORT);
   perror("Error reason");
   return(-1); 
  }
 tcgetattr(int_SerialFD,&OldTerm); //得到串口的原始属性
 bzero(&NewTerm,sizeof(NewTerm)); //清空新的串口属性结构
 fcntl(int_SerialFD,F_SETFL,0); //阻塞read（）操作，读不到数据就阻塞
 NewTerm.c_cflag |= CLOCAL|CREAD; //使能接收，并设置本地状态
 NewTerm.c_cflag &= ~PARENB; //无校验位
 NewTerm.c_cflag &= ~CSTOPB; //一位停止位
 NewTerm.c_cflag &= ~CSIZE; //屏蔽字符大小
 NewTerm.c_cflag |= CS8;  //8位数据
 NewTerm.c_lflag &= ~(ICANON|ECHO|ECHOE); //选择原始输入法
 NewTerm.c_oflag &= ~OPOST; //选择原始数据输出
 NewTerm.c_cc[VMIN] = 8; //指定所要读取字符的最小数量
 NewTerm.c_cc[VTIME] = 1; //指定读取第一个字符的等待时间，时间的单位为n*100ms
 //如果设置VTIME=0，则无字符输入时read（）操作无限期的阻塞
 cfsetispeed(&NewTerm,BAUDRATE); //设置串口输入端波特率
 cfsetospeed(&NewTerm,BAUDRATE); //设置串口输出端波特率
 int_Flag = tcsetattr(int_SerialFD,TCSANOW,&NewTerm); //立即将新属性赋予串口
 if (int_Flag<0)
  {
    
  }
 Buff_Content[0] = 1; //设置控制器地址编号
 Buff_Content[1] = 0xfb; //设置取数据指令
 int_Flag = write(int_SerialFD,Buff_Content,2); //发取数据指令
 
 if (int_Flag<0)
  {
    
  }
 int_Flag = read(int_SerialFD,Buff_Content,255); //返回实际读取的字符数目
 printf("%d bytes read successfully.\n",int_Flag);
 for(int i=0;i<int_Flag;i++)
 {
  printf("%x\t",Buff_Content[i]);  //以16进制的形式输出字符
 }
 int_Flag = tcsetattr(int_SerialFD,TCSANOW,&OldTerm); //恢复原有串口的属性
 if (int_Flag<0)
  {
    
  }
 int_Flag = close(int_SerialFD); //关闭串口设备文件
 if (int_Flag<0)
  {
    
  }
 return(0);
}
