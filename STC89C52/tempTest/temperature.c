#include <reg52.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define uchar unsigned char
#define uint unsigned int
sbit DS=P2^2;           //����DS18B20��������
sbit beep=P2^3;
uint temp;             // variable of temperature
float alarm=50.0;
uchar flag1,inbuff[10],buff[10],counter,start;            // ���崮�ڵ��շ�����
sbit dula=P2^6;
sbit wela=P2^7;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
unsigned char code table1[]={0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,
                        0x87,0xff,0xef};
//������ʱ����
void delay(uint count)      //delay
{
  uint i;
  while(count)
  {
    i=200;
    while(i>0)
    i--;
    count--;
  }
}
//����:���ڳ�ʼ��,������9600����ʽ1
void Init_Com(void)
{
     TMOD = 0x20;
     PCON = 0x00;
     SCON = 0x50;
     TH1 = 0xFd;
     TL1 = 0xFd;
     TR1 = 1;
	 ES=1;
}
//DS18B20���úͳ�ʼ��
void dsreset(void)       //  
{
  uint i;
  DS=0;
  i=103;
  while(i>0)i--;
  DS=1;
  i=4;
  while(i>0)i--;
}
//DS18B20��һ����λ
bit tmpreadbit(void)       
{
   uint i;
   bit dat;
   DS=0;i++;          //i++ for delay
   DS=1;i++;i++;
   dat=DS;
   i=8;while(i>0)i--;
   return (dat);
}
//DS18B20��һ���ֽ�
uchar tmpread(void)   
{
  uchar i,j,dat;
  dat=0;
  for(i=1;i<=8;i++)
  {
    j=tmpreadbit();
    dat=(j<<7)|(dat>>1);   //�������������λ����ǰ�棬�����պ�һ���ֽ���DAT��
  }
  return(dat);
}
//дһ���ֽڵ�DS18B20
void tmpwritebyte(uchar dat)  
{
  uint i;
  uchar j;
  bit testb;
  for(j=1;j<=8;j++)
  {
    testb=dat&0x01;
    dat=dat>>1;
    if(testb)     //д1
    {
      DS=0;
      i++;i++;
      DS=1;
      i=8;while(i>0)i--;
    }
    else
    {
      DS=0;       //д0
      i=8;while(i>0)i--;
      DS=1;
      i++;i++;
    }

  }
}
//DS18B20�¶ȶ�ȡ׼��
void tmpchange(void)  
{
  dsreset();
  delay(1);
  tmpwritebyte(0xcc);  // skip ROM����
  tmpwritebyte(0x44);  // �¶�ת������
}
//��ȡDS18B20�¶�ֵ
uint tmp()               
{
  float tt;
  uchar a,b;
  dsreset();
  delay(1);
  tmpwritebyte(0xcc);
  tmpwritebyte(0xbe);
  a=tmpread();
  b=tmpread();
  temp=b;
  temp<<=8;             //two byte  compose a int variable
  temp=temp|a;
  tt=temp*0.0625;
  temp=tt*10+0.5;
  return temp;
}

void readrom()          //read the serial
{
  uchar sn1,sn2;
  dsreset();
  delay(1);
  tmpwritebyte(0x33);
  sn1=tmpread();
  sn2=tmpread();
}


void delay10ms()            //delay
  {
    uchar a,b;
    for(a=10;a>0;a--)
      for(b=60;b>0;b--);
   }
//���ڷ���һ���ַ�
void putbyte(uchar c)
{
	SBUF=c;
	while(!TI);
	TI=0;
}
//���ڷ���һ���ַ���
void putstring(uchar *puts,uchar n)
{	
	int j;
	ES=0;
	for(j=0;j<n;j++)
		putbyte(puts[j]);
    ES=1;
	memset(puts,0x00,10);
}
//���÷������ݸ�ʽ
void sendData(uint temp)
{
	 float ser;
	 //float ser1[]={23.0,25.1,65.6,13.7,42.3};
   //int i;
   ser=(float)temp/10;
   sprintf(buff,"%f\n",ser);
   putstring(buff,strlen(buff));
  // SBUF=ser;
/*	
  for(i=0;i<5;i++){
   sprintf(buff,"%f\n",ser1[i]);
   SBUF=',';
   	while(!TI);
	TI=0;
   delay(1000);
   putstring(buff,strlen(buff));
   }
   */
   //
   SBUF=',';
   	while(!TI);
	TI=0;
}
//�������ʾ�¶�ֵ
void display(uint temp)			//��ʾ����
{
   uchar A1,A2,A2t,A3;
   float ser;
   ser=(float)temp/10;
   if(ser>alarm)
   	beep=0;
   else
    beep=1;
   //
   A1=temp/100;
   A2t=temp%100;
   A2=A2t/10;
   A3=A2t%10;
   dula=0;
   P0=table[A1];		//��ʾ��λ
   dula=1;
   dula=0;

   wela=0;
   P0=0x7e;
   wela=1;
   wela=0;
   delay(1);

   dula=0;
   P0=table1[A2];		//��ʾʮλ
   dula=1;
   dula=0;

   wela=0;
   P0=0x7d;
   wela=1;
   wela=0;
   delay(1);

   P0=table[A3];		//��ʾ��λ
   dula=1;
   dula=0;

   P0=0x7b;
   wela=1;
   wela=0;
   delay(1);
}

void init() 
{//��ʱ����ʼ�� 
 TMOD=0x01; 
 TH0=0xd8; 
 TL0=0xef;
 TR0=1;
 EA=1; 
 ET0=1;
 //
 start=0;
}  
//��ʱ��0�жϺ���
void timer0() interrupt 1 using 3
{//timer0�жϷ�������ж�һ����ʱ10ms 
 counter++;//���Ĵ�������  
 TH0=0xd8; //���嵥λ���ĵ���ʱ��С  
 TL0=0xef; 
} 
//�����жϺ���
void serial() interrupt 4
{
	int i=0;
	uchar a;
	if(RI)
	{
		RI=0;
		a=SBUF;
		if(a!='\n')
		{
			inbuff[i]=a;
			i++;
		}
		else
		{
			i=0;
			alarm=atof(inbuff);
			memset(inbuff,0x00,10);
		//	flag=1;
		}
	}
	
}

void main()
{
	 uchar a;
	 init();//��ʼ����ʱ��
	 Init_Com();//��ʼ������
	  do
	  {
	  //�趨�¶ȶ�ȡ������
	   if(counter>200)
	   {
		counter=0;
	   	start=1;
	   }
	   //��ȡ�¶�ֵ�������͵���λ��
	   if(start)
	   {
	    start=0;
	    tmpchange();
		sendData(tmp());
		}
	    //�������ʾ�¶�ֵ
	  for(a=10;a>0;a--)
	  {   
	    display(tmp());
	  }
	  }
	  while(1);
}