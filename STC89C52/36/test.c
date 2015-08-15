#include<reg52.h>
#include <string.h>           
#define uchar unsigned char 
#define uint unsigned int 

sbit sound=P2^3; 
uchar counter=0; 
//
#define NOTE_END 0x0000
#define NOTE_SONGEND 0x0001
//���ϲ����Ƕ����ǰ�ÿ��������Ƶ��ֵ��Ӧ��������ʵ���ô���ô�࣬���Ƕ������ˣ�������������дE���ĸ��ָ裬��������NOTE_E+���ֱ�ʾ������NOTE_EH+���ֱ�ʾ�����е������������NOTE_EL+���ֱ�ʾ�����е��������������������д�����ȽϺ�ʶ��
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
//���ϲ����Ƕ����ǰ�ÿ��������Ƶ��ֵ��Ӧ��������ʵ���ô���ô�࣬���Ƕ������ˣ�������������дD���ĸ��ָ裬��������NOTE_D+���ֱ�ʾ������NOTE_DH+���ֱ�ʾ�����е������������NOTE_DL+���ֱ�ʾ�����е��������������������д�����ȽϺ�ʶ��
uchar code tune[] = 
{
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
  NOTE_D6,NOTE_END
};
//
float code duration[]=
{
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
  1+1+1+1
};
//
void init() 
{
	//��ʱ����ʼ�� 
	 TMOD=0x11; 
	 TH0=0xd8; 
	 TL0=0xef; 
	 EA=1; 
	 ET0=1; 
	 //
} 
void timer0(void) interrupt 1 using 3  
{//timer0�жϷ�������ж�һ����ʱ10ms 
 counter++;//���Ĵ�������  
 TH0=0xd8; //���嵥λ���ĵ���ʱ��С  
 TL0=0xef; 
} 

void delay(uchar n) 
{ //��ʱn����
 uchar i; 
 while(n--) 
  for(i=0;i<125;i++);  
} 

void sound_delay(uchar sound_signalhigh,uchar sound_signallow)//����һ��Ƶ�ʵ����� 
{  
 TH1=sound_signalhigh; 
 TL1=sound_signallow; 
 TR1=1; 
 while(TF1==0); 
 TR1=0; 
 TF1=0;   
} 
// 
void main() 
{ 
 uint k=0; 
 uchar sound_signalhigh,sound_signallow;//��ʱ���ĸ�8λ���8λ 
 uchar sound_pace;//������ļ�����  
 init(); 
 while(1) 
   { 
       k=0; 
	   while(tune[k]!=0)//��Ϊ�����Ľ�β����Ϊ�ļ����� ������ʾ��  0  
	   {
		sound_signallow=0xff&tune[k];
		P1=sound_signallow;
	    sound_signalhigh=0xff&(tune[k]>>8);      
	    //�ӱ���ȡ�� ���Ĵ�С  
	    sound_pace=40*duration[k];//400Ϊһ�ĵ�ʱ�� ,�ڴ��޸�һ�ĵ�ʱ���� 
		k++;
	    TR0=1; 
	   //��������δ�ﵽʱ�򣬼���ѭ��������������������  
	   while(counter!=sound_pace) 
	      {  
	       sound=~sound; 
	       sound_delay(sound_signalhigh,sound_signallow); 
	      } 
	     counter=0;//���ļ�������0��������һ������  
	     TR0=0; 
	    } 
	   delay(100); //�����ݳ���Ϻ�,��ʱһ��ʱ��  
	} 
}



 