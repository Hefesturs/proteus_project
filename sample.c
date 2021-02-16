#include"iom48v.h"
typedef unsigned int u16;
typedef unsigned int u8;
unsigned char password[6]={1,1,2,9,2,1};
unsigned char pwd_now[6]={0,0,0,0,0,0};

void delay(u16 num)
{
    u16 d,l;
        for (d=num;d>0;d--)
             for(l=110;l>0;l--);
}

void write_cmd(u8 cmd)        //写指令
{
  PORTC&=0xfb;                //选择指令寄存器
  PORTC&=0xfd;                //进行写操作
  PORTB = cmd;                //把指令传给单片机
  delay(5);
  PORTC|=0x01;
  delay(5);
  PORTC&=0xfe;                //en为使能端，当en由高电平跳到底电平时，液晶模块执行命令。
}

void write_data(u8 dat)
{
  PORTC|=0x04;        //选择数据寄存器
  PORTC&=0xfd;        //进行写操作
  PORTB = dat;        // 把数据传给单片机
  delay(5);
  PORTC|=0x01;   	  //en =1 
  delay(5);      	  //delay
  PORTC&=0xfe;   	  //en =0
}

void lcd_init(void)           //初始化
{
   write_cmd(0x02);           //归为
   write_cmd(0x06);           //设置输入方式
   write_cmd(0x0e);           //控制显示开关
   write_cmd(0x38);           //功能设置
   write_cmd(0x01);    		  //清屏
}


int judge()
{
 	int a,b;
 	for(a=0;a<6;a++)
	{
	 if(password[a]==pwd_now[a])
	 {
	  b=1;
	 }else
	 {
	  b=2;
	  break;
	 }
	}
	return b;
}

void lcd_display(char text[],int num_text)             //显示状态信息
{
   int y;
   lcd_init();                             				//清屏
   write_cmd(0x80); 
   for(y=0;y<num_text;y++)
   {
    write_data(text[y]);
   }
}

int read_key()
{
 	unsigned char i,k;
	for(i=0;i<6;i++)
	{
	 PORTD=0xf0;
	 while(PORTD==0xf0)
	 {
	   PORTD=0xf0;
	 }
	 if(PORTD!=0xf0)
	 {
	  delay(10);
	  if(PORTD!=0xf0)
	  {
	   PORTD=0xf0;
	   switch(PORTD)
	   {
	   	 case(0x0e):k=1;break;
		 case(0x0d):k=2;break;
		 case(0x0b):k=3;break;
		 case(0x07):k=4;break;
	   }
	   PORTD=0x0f;
	   
	   switch(PORTD)
	   {
	     case(0xe0):k=k;break;
		 case(0xd0):k=k+4;break;
		 case(0xb0):k=k+8;break;
		 case(0x70):k=k+12;break;
	   }
	   while(PORTD!=0x0f)              //保持运行，直到松开按键
	   {
	   	  PORTD=0x0f;
		  delay(10);
	   }
	  }
	 }
	}
	return k;
}

void read_password()    
{
  int j;
  for(j=0;j<6;j++)
  {
   	passwod_now[j] = read_key();
  }
 } 
 
void start()
{
  DDRC = 0xff;
  DDRB = 0xff;
  DDRD = 0xff;
  
  PORTC&=0xfd;
  PORTC&=0xfb;
  PORTC&=0xfe;
} 

void main()
{
  int status=0;              //程序初始  
  int b;
  start();                     //调整初始状态
  while(1)
  {
   switch(status)
   {
   	case 0:                     //显示开机
   	lcd_display("PASSWORD",7);
   	delay(500);
   	status = 1;
   	break;
  
  	case 1:                    //读取密码
  	read_key();
  	read_password();
  	status = 2;
  	break;
  
  	case 2:                          // 判断密码
  	b=judge();
  	switch(b)
  	{
   	 case 1:
   	 lcd_display("CORRECT",6);       //密码正确
   	 status = 3;
   	 break;
   
   	 case 2:
   	 lcd_display("WRONG",4);         //密码错误，重新输入密码
   	 delay(100);
   	 lcd_diaplay("Please Reenter");
   	 delay(500);
   	 pwd_now[6]={0,0,0,0,0,0};       //密码清零
   	 status = 1;
   	 break;
  	}

   case 3:                          //结束，显示Hello world
   delay(3000);                     //延迟三秒
   lcd_display("HELLO WORLD",10);
   break;
  }
 }    
}