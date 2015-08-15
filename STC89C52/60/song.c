#include<reg52.h> 
#include<string.h>  
#include "Serial.h"        
#define uchar unsigned char 
#define uint unsigned int 
#define SONGNUM 6

sfr  T2MOD=0xC9;
sbit sound=P2^3; 
uint counter=0,sound_pace,record=0,k=0,count,m=1; 
uchar flag=0,len; 
uchar sound_signalhigh,sound_signallow;//��ʱ���ĸ�8λ���8λ 
//������ļ����� 
sbit  wei=P2^6;
sbit  duan=P2^7;
sbit lcden=P3^4;
sbit lcdrs=P3^5;
uchar code *song[]={"Molihua","Huluwa","Throw","ReadBoy","TwoTiger","HappyNewYear"};
uchar code *info[]={"Curr:","   Next:"};
short code index[]={0,68,167,271,352,385};
//
void lcdelay(int n);
void writeCom(uchar com);
void writeData(uchar value);
void init();
void delay(uchar n);
void sound_delay(uchar sound_signalhigh,uchar sound_signallow);
uint getNext(uint x);
uint getForward(uint x);
//

#define NOTE_END 0x0000
#define NOTE_SONGEND 0x0001
//����E���ĸߡ��С�����
#define NOTE_E0 0xffff
#define NOTE_E1 0xfa8c
#define NOTE_E2 0xfadc
#define NOTE_E3 0xfa6c
#define NOTE_E4 0xfaec
#define NOTE_E5 0xfc5e
#define NOTE_E6 0xfcc4
#define NOTE_E7 0xfd1e

#define NOTE_EL1 0xf518
#define NOTE_EL2 0xf5b7
#define NOTE_EL3 0xf6d1
#define NOTE_EL4 0xf7db
#define NOTE_EL5 0xf8be
#define NOTE_EL6 0xf987
#define NOTE_EL7 0xfa3c

#define NOTE_EH1 0xfd47
#define NOTE_EH2 0xfd6e
#define NOTE_EH3 0xfdb6
#define NOTE_EH4 0xfdf6
#define NOTE_EH5 0xfe2f
#define NOTE_EH6 0xfe62
#define NOTE_EH7 0xfe8f
//����D���ĸߡ��С�����
#define NOTE_D0 0xffff
#define NOTE_D1 0xf9e1
#define NOTE_D2 0xfa8c
#define NOTE_D3 0xfadc
#define NOTE_D4 0xfb6c
#define NOTE_D5 0xfbec
#define NOTE_D6 0xfc5e
#define NOTE_D7 0xfcc4

#define NOTE_DL1 0xf3c2
#define NOTE_DL2 0xf518
#define NOTE_DL3 0xf5b7
#define NOTE_DL4 0xf6d1
#define NOTE_DL5 0xf7db
#define NOTE_DL6 0xf8be
#define NOTE_DL7 0xf987

#define NOTE_DH1 0xfcf2
#define NOTE_DH2 0xfd47
#define NOTE_DH3 0xfd6e
#define NOTE_DH4 0xfdb6
#define NOTE_DH5 0xfdf6
#define NOTE_DH6 0xfe2f
#define NOTE_DH7 0xefee

//����C���ĸߡ��С�����
#define NOTE_C0 0xffff
#define NOTE_C1 0xf922
#define NOTE_C2 0xf9e1
#define NOTE_C3 0xfa8c
#define NOTE_C4 0xfadc
#define NOTE_C5 0xfb6c
#define NOTE_C6 0xfbec
#define NOTE_C7 0xfc5e

#define NOTE_CL1 0xf243
#define NOTE_CL2 0xf3c2
#define NOTE_CL3 0xf518
#define NOTE_CL4 0xf5b7
#define NOTE_CL5 0xf6d1
#define NOTE_CL6 0xf7db
#define NOTE_CL7 0xf8be

#define NOTE_CH1 0xfc93
#define NOTE_CH2 0xfcf2
#define NOTE_CH3 0xfd47
#define NOTE_CH4 0xfd6e
#define NOTE_CH5 0xfdb6
#define NOTE_CH6 0xfdf6
#define NOTE_CH7 0xfe2f


uint code tune[]= 
{
 //����
  NOTE_E3,NOTE_E3,NOTE_E5,NOTE_E6,NOTE_EH1,NOTE_EH1,NOTE_E6,NOTE_E5,NOTE_E5,NOTE_E6,
  NOTE_E5,NOTE_E3,NOTE_E3,NOTE_E5,NOTE_E6,NOTE_EH1,NOTE_EH1,NOTE_E6,
  
  NOTE_E5,NOTE_E5,NOTE_E6,NOTE_E5,NOTE_E5,NOTE_E5,NOTE_E5,NOTE_E3,NOTE_E5,NOTE_E6,
  NOTE_E6,NOTE_E5,
  
  NOTE_E3,NOTE_E2,NOTE_E3,NOTE_E5,NOTE_E3,NOTE_E2,NOTE_E1,NOTE_E1,NOTE_E2,NOTE_E1,
  NOTE_E3,NOTE_E2,NOTE_E1,NOTE_E3,NOTE_E2,NOTE_E3,
  
  NOTE_E5,NOTE_E6,NOTE_EH1,NOTE_E5,NOTE_E2,NOTE_E3,NOTE_E5,NOTE_E2,NOTE_E3,NOTE_E1,
  NOTE_EL6,NOTE_EL5,NOTE_EL6,NOTE_E1,
  
  NOTE_E2,NOTE_E3,NOTE_E1,NOTE_E2,NOTE_E1,NOTE_EL6,NOTE_EL5,NOTE_SONGEND,
  //��«��
  NOTE_DH1,NOTE_D6,NOTE_D5,NOTE_D6,NOTE_D0,
  NOTE_DH1,NOTE_D6,NOTE_D5,NOTE_DH1,NOTE_D6,NOTE_D0,NOTE_D6,
  NOTE_D6,NOTE_D6,NOTE_D5,NOTE_D6,NOTE_D0,NOTE_D6,
  NOTE_DH1,NOTE_D6,NOTE_D5,NOTE_DH1,NOTE_D6,NOTE_D0,
  
  NOTE_D1,NOTE_D1,NOTE_D3,
  NOTE_D1,NOTE_D1,NOTE_D3,NOTE_D0,
  NOTE_D6,NOTE_D6,NOTE_D6,NOTE_D5,NOTE_D6,
  NOTE_D5,NOTE_D1,NOTE_D3,NOTE_D0,
  NOTE_DH1,NOTE_D6,NOTE_D6,NOTE_D5,NOTE_D6,
  NOTE_D5,NOTE_D1,NOTE_D2,NOTE_D0,
  NOTE_D7,NOTE_D7,NOTE_D5,NOTE_D3,
  NOTE_D5,
  NOTE_DH1,NOTE_D0,NOTE_D6,NOTE_D6,NOTE_D5,NOTE_D5,NOTE_D6,NOTE_D6,
  NOTE_D0,NOTE_D5,NOTE_D1,NOTE_D3,NOTE_D0,
  NOTE_DH1,NOTE_D0,NOTE_D6,NOTE_D6,NOTE_D5,NOTE_D5,NOTE_D6,NOTE_D6,
  NOTE_D0,NOTE_D5,NOTE_D1,NOTE_D2,NOTE_D0,
  NOTE_D3,NOTE_D3,NOTE_D1,NOTE_DL6,
  NOTE_D1,
  NOTE_D3,NOTE_D5,NOTE_D6,NOTE_D6,
  NOTE_D3,NOTE_D5,NOTE_D6,NOTE_D6,
  NOTE_DH1,NOTE_D0,NOTE_D7,NOTE_D5,
  NOTE_D6,NOTE_SONGEND,
 //���־�
  NOTE_E5,NOTE_E6,NOTE_E5,NOTE_E3,NOTE_E2,NOTE_E3,NOTE_E5,NOTE_E0,
  NOTE_EH1,NOTE_EH2,NOTE_EH1,NOTE_E6,NOTE_E5,NOTE_E6,NOTE_EH1,NOTE_E0,
  NOTE_E5,NOTE_E6,NOTE_E5,NOTE_E6,NOTE_E5,NOTE_E3,NOTE_E2,NOTE_E3,NOTE_E5,
  NOTE_E5,NOTE_E2,NOTE_E3,NOTE_E2,NOTE_E3,
  NOTE_E5,NOTE_E6,NOTE_E5,NOTE_E3,NOTE_E2,NOTE_E3,NOTE_E5,NOTE_E0,
  NOTE_EH1,NOTE_EH2,NOTE_EH1,NOTE_E6,NOTE_E5,NOTE_E6,NOTE_EH1,NOTE_E0,
  NOTE_E5,NOTE_E6,NOTE_E5,NOTE_E6,NOTE_E5,NOTE_E3,NOTE_E2,NOTE_E3,NOTE_E5,
  NOTE_E5,NOTE_E2,NOTE_E3,NOTE_E2,NOTE_E1,
  NOTE_E5,NOTE_E0,NOTE_E5,NOTE_E0,
  NOTE_E5,NOTE_E3,NOTE_E2,NOTE_E3,NOTE_E5,NOTE_E0,
  NOTE_E5,NOTE_E5,NOTE_E3,NOTE_E6,NOTE_E5,
  NOTE_E3,NOTE_E5,NOTE_E3,NOTE_E2,NOTE_E1,NOTE_E2,
  NOTE_E3,NOTE_E5,NOTE_E3,NOTE_E2,NOTE_E1,NOTE_E2,
  NOTE_E3,NOTE_E6,NOTE_E5,NOTE_E6,NOTE_E5,
  NOTE_E2,NOTE_E3,NOTE_E5,NOTE_E6,NOTE_E5,NOTE_E6,NOTE_E5,
  NOTE_E2,NOTE_E3,NOTE_E1,NOTE_E1,NOTE_SONGEND,
 //������
  NOTE_EL6,NOTE_E1,NOTE_EL6,NOTE_E5,NOTE_EL6,NOTE_E1,NOTE_EL6,
  NOTE_EL6,NOTE_EL6,NOTE_E1,NOTE_E2,NOTE_E3,NOTE_E2,NOTE_E1,NOTE_EL6,
  NOTE_EL6,NOTE_E1,NOTE_E3,NOTE_E3,NOTE_E2,NOTE_E3,NOTE_E5,NOTE_E3,
  NOTE_E3,NOTE_E5,NOTE_E6,NOTE_E6,NOTE_E6,NOTE_E5,NOTE_E3,NOTE_E2,
  NOTE_E6,NOTE_E6,NOTE_E6,NOTE_E6,NOTE_E6,NOTE_E6,NOTE_E5,NOTE_E3,NOTE_E2,
  NOTE_EL6,NOTE_E1,NOTE_EL6,NOTE_E5,NOTE_EL6,NOTE_E1,NOTE_EL6,
  NOTE_EL6,NOTE_EL6,NOTE_E1,NOTE_E2,NOTE_E3,NOTE_E2,NOTE_E1,NOTE_EL6,
  NOTE_EL6,NOTE_E1,NOTE_E3,NOTE_E3,NOTE_E2,NOTE_E3,NOTE_E5,NOTE_E3,
  NOTE_E3,NOTE_E5,NOTE_E6,NOTE_E6,NOTE_E6,NOTE_E5,NOTE_E3,NOTE_E2,
  NOTE_E6,NOTE_E6,NOTE_E6,NOTE_E6,NOTE_E6,NOTE_E6,NOTE_E5,NOTE_E3,NOTE_E2,NOTE_SONGEND,
 //��ֻ�ϻ�
  NOTE_C1,NOTE_C2,NOTE_C3,NOTE_C1,NOTE_C1,NOTE_C2,NOTE_C3,NOTE_C1,
  NOTE_C3,NOTE_C4,NOTE_C5,NOTE_C3,NOTE_C4,NOTE_C5,
  NOTE_C5,NOTE_C6,NOTE_C5,NOTE_C4,NOTE_C3,NOTE_C1,NOTE_C5,NOTE_C6,NOTE_C5,NOTE_C4,NOTE_C3,NOTE_C1,
  NOTE_C1,NOTE_C5,NOTE_C1,NOTE_C1,NOTE_C5,NOTE_C1,NOTE_SONGEND,
 //�����
  NOTE_E1,NOTE_E1,NOTE_E1,NOTE_EL5,NOTE_E3,NOTE_E3,NOTE_E3,NOTE_E1,
  NOTE_E1,NOTE_E3,NOTE_E5,NOTE_E5,NOTE_E4,NOTE_E3,NOTE_E2,
  NOTE_E2,NOTE_E3,NOTE_E4,NOTE_E4,NOTE_E3,NOTE_E2,NOTE_E3,NOTE_E1,
  NOTE_E1,NOTE_E3,NOTE_E2,NOTE_EL5,NOTE_EL7,NOTE_E2,NOTE_E1,NOTE_END//NOTE_SONGEND
  };
  //
float code duration[]=
{
	//����
  1,0.5,0.5,0.5,0.5,0.5,0.5,1,0.5,0.5,2,1,0.5,0.5,0.5,0.5,0.5,0.5,
  1,0.5,0.5,2,1,1,1,0.5,0.5,1,1,2,
  1,0.5,0.5,1,0.5,0.5,1,0.5,0.5,2,0.5,0.5,0.5,0.5,1.5,0.5,
  1,0.5,0.5,2,1,0.5,0.5,0.5,0.5,0.5,0.5,2,1,1,
  1.5,0.5,0.5,0.5,0.5,0.5,3,1,
  //��«��
  1,1,0.5,0.5,1,
  0.5,0.5,0.5,0.5,1,0.5,0.5,
  0.5,1,0.5,1,0.5,0.5,
  0.5,0.5,0.5,0.5,1,1,
  
  1,1,1+1,
  0.5,1,1+0.5,1,
  1,1,0.5,0.5,1,
  0.5,1,1+0.5,1,
  0.5,0.5,0.5,0.5,1+1,
  0.5,1,1+0.5,1,
  1+1,0.5,0.5,1,
  1+1+1+1,
  0.5,0.5,0.5+0.25,0.25,0.5+0.25,0.25,0.5+0.25,0.25,
  0.5,1,0.5,1,1,
  0.5,0.5,0.5+0.25,0.25,0.5+0.25,0.25,0.5+0.25,0.25,
  0.5,1,0.5,1,1,
  1+1,0.5,0.5,1,
  1+1+1+1,
  0.5,1,0.5,1+1,
  0.5,1,0.5,1+1,
  1+1,0.5,0.5,1,
  1+1+1+1,1,
//���־�
  0.25,0.25,0.25,0.25,0.5,0.5,1,1,
  0.25,0.25,0.25,0.25,0.5,0.5,1,1,
  0.25,0.25,0.25,0.25,0.5,0.5,0.5,0.5,1,
  0.5,0.5,0.5,0.5,2,
  0.25,0.25,0.25,0.25,0.5,0.5,1,1,
  0.25,0.25,0.25,0.25,0.5,0.5,1,1,
  0.25,0.25,0.25,0.25,0.5,0.5,0.5,0.5,1,
  0.5,0.5,0.5,0.5,2,
  1,1,1,1,
  0.5+0.5,0.25,0.5,0.5,1,1,
  0.5,1,0.5,1,1,
  0.5+0.5,0.25,0.5,0.5,1,1,
  1,1,0.5,0.5,0.5,0.5,
  2,0.5,0.5,0.5,0.5,
  0.5,0.5,1,0.5,0.5,0.5,0.5,
  1,1,2,2,1,
 //������
  1,0.25,0.5,0.5,1,0.25,1,
  0.5,0.25,0.25,0.5,0.5,0.5,0.5,1,
  1,0.25,1,0.5,0.25,0.25,0.5,0.5,
  0.25,0.25,0.25,0.25,0.25,0.25,0.5,2,
  0.5,0.25,0.25,0.5,0.5,0.5,0.5,0.5,0.5,
  1,0.25,0.5,0.5,1,0.25,1,
  0.5,0.25,0.25,0.5,0.5,0.5,0.5,1,
  1,0.25,1,0.5,0.25,0.25,0.5,0.5,
  0.25,0.25,0.25,0.25,0.25,0.25,0.5,2,
  0.5,0.25,0.25,0.5,0.5,0.5,0.5,0.5,0.5,1,
 //��ֻ�ϻ�
  1,1,1,1,1,1,1,1,
  1,1,2,1,1,2,
  1,0.25,1,0.25,1,1,1,0.25,1,0.25,1,1,
  1,1,2,1,1,2,1,
 //�����
   0.5,0.5,1,1,0.5,0.5,1,1,
   0.5,0.5,1,1,0.5,0.5,2,
   0.5,0.5,1,1,0.5,0.5,1,1,
   0.5,0.5,1,1,0.5,0.5,2,1
  };
  
void main() 
{  
 init(); 
 SerialInit();
 while(1) 
   { 
      k=0; 
	  init();
	  sound=1;
   while(tune[k]!=NOTE_END)//��Ϊ�����Ľ�β����Ϊ�ļ����� ������ʾ��  0  
   { 
   //��⴮���Ƿ��Ϳ�����Ϣ����������У�ִ������������
   	if(SerailRevCheck())
	{
		switch(getIndex())
		{
			case 200:sound=1;continue;//��ͣ���Ÿ���
			case 201:sound=1;SerailSetCheck();break;//�ָ����Ÿ���
			case 202:sound=0;continue;//����������
			case 203:sound=1;SerailSetCheck();break;//������ֹͣ����
			case 204:P1=0x00;SerailSetCheck();break;//���Ƶ���
			case 205:P1=0xff;SerailSetCheck();break;//���Ƶ���
			case 206:sound=1;k=getNext(k);SerailSetCheck();break;//��һ�׸�
			case 207:sound=1;k=getForward(k);SerailSetCheck();break;//ǰһ�׸�
			default:
				sound=1;
				k=index[getIndex()];
				SerailSetCheck();
				//����LCD��ʾ����
				m=getIndex();
				writeCom(0x01);//����
				writeCom(0x80);
				//
				 len=strlen(info[0]);
				 for(count=0;count<len;count++)
				 {
				 	writeData(info[0][count]);	
					delay(20);
				 }
				 len=strlen(song[m]);
				 for(count=0;count<len;count++)
				 {
				 	writeData(song[m][count]);	
					delay(20);
				 }
				 //
				 m++;
				 m=m%SONGNUM;
				 writeCom(0x80+0x40);
				 len=strlen(info[1]);
				 for(count=0;count<len;count++)
				 {
				 	writeData(info[1][count]);	
					delay(20);
				 }
				 len=strlen(song[m]);
				 for(count=0;count<len;count++)
				 {
				 	writeData(song[m][count]);	
					delay(20);
				 }

		}
	}
	/*�ӱ���ȡ�� �������ߣ�Ҳ������������ǵ������������Ǹ�
��,�����������������ǾͿ���ȷ��������Ӧ�Ķ�ʱ����ֵ�ڱ��е�λ���ˡ� */
    if(tune[k]==NOTE_SONGEND)
	{
		k++;
		sound=1;
		writeCom(0x01);//����
		writeCom(0x80);
		//
		 len=strlen(info[0]);
		 for(count=0;count<len;count++)
		 {
		 	writeData(info[0][count]);	
			delay(20);
		 }
		 len=strlen(song[m]);
		 for(count=0;count<len;count++)
		 {
		 	writeData(song[m][count]);	
			delay(20);
		 }
		 //
		 m++;
		 m=m%SONGNUM;
		 writeCom(0x80+0x40);
		 len=strlen(info[1]);
		 for(count=0;count<len;count++)
		 {
		 	writeData(info[1][count]);	
			delay(20);
		 }
		 len=strlen(song[m]);
		 for(count=0;count<len;count++)
		 {
		 	writeData(song[m][count]);	
			delay(20);
		 }
	
	}
    sound_signalhigh=0xff&(tune[k]>>8); 
    sound_signallow=0xff&tune[k]; 
    //�ӱ���ȡ�� ���Ĵ�С  
    sound_pace=40*duration[k];//600Ϊһ�ĵ�ʱ�� ,�ڴ��޸�һ�ĵ�ʱ���� 
	k++;
    TR2=1; 
   //��������δ�ﵽʱ�򣬼���ѭ��������������������  
   while(counter!=sound_pace) 
      {  
       sound=~sound; 
       sound_delay(sound_signalhigh,sound_signallow); 
	  // pwm_delay(temp);
      } 
     counter=0;//���ļ�������0��������һ������  
     TR2=0; 
    } 
   delay(100); //�����ݳ���Ϻ�,��ʱһ��ʱ��     }  
} 
}
//
void init() 
{//��ʱ����ʼ�� 
	 TMOD=0x21; 
	// TH0=0xd8; 
	// TL0=0xef;  
	 ET0=1;
	// ET1=1;
	//��ʱ��2
	RCAP2H=(65536-10000)/256;
	RCAP2L=(65536-10000)%256;
	T2CON=0;
	T2MOD=0;

	ET2=1;
	EA=1;
	//TR2=1;
	 duan=0;
	wei=0;
	lcden=0;
	count=0;
	writeCom(0x38);
	writeCom(0x0c);
	writeCom(0x06);
	writeCom(0x01);//����
	writeCom(0x80);
//
 len=strlen(info[0]);
 for(count=0;count<len;count++)
 {
 	writeData(info[0][count]);	
	delay(20);
 }
 len=strlen(song[0]);
 for(count=0;count<len;count++)
 {
 	writeData(song[0][count]);	
	delay(20);
 }
 //
 writeCom(0x80+0x40);
 len=strlen(info[1]);
 for(count=0;count<len;count++)
 {
 	writeData(info[1][count]);	
	delay(20);
 }
 len=strlen(song[1]);
 for(count=0;count<len;count++)
 {
 	writeData(song[1][count]);	
	delay(20);
 }
 P1=0xff;
}  

void timer0() interrupt 1 using 3
{//timer0�жϷ�������ж�һ����ʱ10ms 
/*
 counter++;//���Ĵ�������  
 TH0=0xd8; //���嵥λ���ĵ���ʱ��С  
 TL0=0xef; 
 */
	flag=1;
} 
/*
void timer1() interrupt 3
{//timer1�жϷ������
 flag=1;
} 
*/
/*********************************
*
*********************************/
void timer2() interrupt 5
{
	TF2=0;
	counter++;	
}

void delay(uchar n) 
{ //��ʱn����
 uchar i; 
 while(n--) 
  for(i=0;i<125;i++);  
} 

void sound_delay(uchar sound_signalhigh,uchar sound_signallow)//����һ��Ƶ�ʵ����� 
{  
 TH0=sound_signalhigh; 
 TL0=sound_signallow; 
 TR0=1; 
 while(!flag);
 	flag=0;
	TR0=0; 
} 

uint getNext(uint x)
{
	uchar i;
	for(i=0;i<SONGNUM;i++)
	{
		if(x<index[i])
		{
		//����LCD��ʾ����
		m=i;
		writeCom(0x01);//����
		writeCom(0x80);
		//
		 len=strlen(info[0]);
		 for(count=0;count<len;count++)
		 {
		 	writeData(info[0][count]);	
			delay(20);
		 }
		 len=strlen(song[m]);
		 for(count=0;count<len;count++)
		 {
		 	writeData(song[m][count]);	
			delay(20);
		 }
		 //
		 m++;
		 m=m%SONGNUM;
		 writeCom(0x80+0x40);
		 len=strlen(info[1]);
		 for(count=0;count<len;count++)
		 {
		 	writeData(info[1][count]);	
			delay(20);
		 }
		 len=strlen(song[m]);
		 for(count=0;count<len;count++)
		 {
		 	writeData(song[m][count]);	
			delay(20);
		 }

			return index[i];
			}
	}
		//
		m=0;
		writeCom(0x01);//����
		writeCom(0x80);
		//
		 len=strlen(info[0]);
		 for(count=0;count<len;count++)
		 {
		 	writeData(info[0][count]);	
			delay(20);
		 }
		 len=strlen(song[m]);
		 for(count=0;count<len;count++)
		 {
		 	writeData(song[m][count]);	
			delay(20);
		 }
		 //
		 m++;
		 m=m%SONGNUM;
		 writeCom(0x80+0x40);
		 len=strlen(info[1]);
		 for(count=0;count<len;count++)
		 {
		 	writeData(info[1][count]);	
			delay(20);
		 }
		 len=strlen(song[m]);
		 for(count=0;count<len;count++)
		 {
		 	writeData(song[m][count]);	
			delay(20);
		 }


	return 0;
}

uint getForward(uint x)
{
	uchar i;
	for(i=SONGNUM-1;i>0;i--)
	{
		if(x>index[i])
		{
			//����LCD��ʾ����
			m=i-1;
			writeCom(0x01);//����
			writeCom(0x80);
			//
			 len=strlen(info[0]);
			 for(count=0;count<len;count++)
			 {
			 	writeData(info[0][count]);	
				delay(20);
			 }
			 len=strlen(song[m]);
			 for(count=0;count<len;count++)
			 {
			 	writeData(song[m][count]);	
				delay(20);
			 }
			 //
			 m++;
			 m=m%SONGNUM;
			 writeCom(0x80+0x40);
			 len=strlen(info[1]);
			 for(count=0;count<len;count++)
			 {
			 	writeData(info[1][count]);	
				delay(20);
			 }
			 len=strlen(song[m]);
			 for(count=0;count<len;count++)
			 {
			 	writeData(song[m][count]);	
				delay(20);
			 }

			return index[i-1];
		}
	}
			//
			m=SONGNUM-1;
			writeCom(0x01);//����
			writeCom(0x80);
			//
			 len=strlen(info[0]);
			 for(count=0;count<len;count++)
			 {
			 	writeData(info[0][count]);	
				delay(20);
			 }
			 len=strlen(song[m]);
			 for(count=0;count<len;count++)
			 {
			 	writeData(song[m][count]);	
				delay(20);
			 }
			 //
			 m++;
			 m=m%SONGNUM;
			 writeCom(0x80+0x40);
			 len=strlen(info[1]);
			 for(count=0;count<len;count++)
			 {
			 	writeData(info[1][count]);	
				delay(20);
			 }
			 len=strlen(song[m]);
			 for(count=0;count<len;count++)
			 {
			 	writeData(song[m][count]);	
				delay(20);
			 }

	return index[SONGNUM-1];
}
//
void lcdelay(int n)
{
	int x,y;
	for(x=n;x>0;x--)
		for(y=110;y>0;y--);
}
void writeCom(uchar com)
{
	lcdrs=0;
	P0=com;
	lcdelay(1);
	lcden=1;
	lcdelay(5);
	lcden=0;
}
void writeData(uchar value)
{
	lcdrs=1;
	P0=value;
	lcdelay(1);
	lcden=1;
	lcdelay(5);
	lcden=0;
}


 