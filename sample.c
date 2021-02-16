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

void write_cmd(u8 cmd)        //дָ��
{
  PORTC&=0xfb;                //ѡ��ָ��Ĵ���
  PORTC&=0xfd;                //����д����
  PORTB = cmd;                //��ָ�����Ƭ��
  delay(5);
  PORTC|=0x01;
  delay(5);
  PORTC&=0xfe;                //enΪʹ�ܶˣ���en�ɸߵ�ƽ�����׵�ƽʱ��Һ��ģ��ִ�����
}

void write_data(u8 dat)
{
  PORTC|=0x04;        //ѡ�����ݼĴ���
  PORTC&=0xfd;        //����д����
  PORTB = dat;        // �����ݴ�����Ƭ��
  delay(5);
  PORTC|=0x01;   	  //en =1 
  delay(5);      	  //delay
  PORTC&=0xfe;   	  //en =0
}

void lcd_init(void)           //��ʼ��
{
   write_cmd(0x02);           //��Ϊ
   write_cmd(0x06);           //�������뷽ʽ
   write_cmd(0x0e);           //������ʾ����
   write_cmd(0x38);           //��������
   write_cmd(0x01);    		  //����
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

void lcd_display(char text[],int num_text)             //��ʾ״̬��Ϣ
{
   int y;
   lcd_init();                             				//����
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
	   while(PORTD!=0x0f)              //�������У�ֱ���ɿ�����
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
  int status=0;              //�����ʼ  
  int b;
  start();                     //������ʼ״̬
  while(1)
  {
   switch(status)
   {
   	case 0:                     //��ʾ����
   	lcd_display("PASSWORD",7);
   	delay(500);
   	status = 1;
   	break;
  
  	case 1:                    //��ȡ����
  	read_key();
  	read_password();
  	status = 2;
  	break;
  
  	case 2:                          // �ж�����
  	b=judge();
  	switch(b)
  	{
   	 case 1:
   	 lcd_display("CORRECT",6);       //������ȷ
   	 status = 3;
   	 break;
   
   	 case 2:
   	 lcd_display("WRONG",4);         //�������������������
   	 delay(100);
   	 lcd_diaplay("Please Reenter");
   	 delay(500);
   	 pwd_now[6]={0,0,0,0,0,0};       //��������
   	 status = 1;
   	 break;
  	}

   case 3:                          //��������ʾHello world
   delay(3000);                     //�ӳ�����
   lcd_display("HELLO WORLD",10);
   break;
  }
 }    
}