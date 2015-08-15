#include<reg52.h>           
#define uchar unsigned char 
#define uint unsigned int 

sbit sound=P2^3; 
uint counter=0,sound_pace,temp; 
uchar k=0,flag=0; 
uchar sound_signalhigh,sound_signallow;//��ʱ���ĸ�8λ���8λ 
//������ļ����� 
//
#define NOTE_END 0x0000
#define NOTE_SONGEND 0x0001
//
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
//
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
  NOTE_D6,NOTE_END

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
  1+1+1+1,1

  };
  //
void init() 
{//��ʱ����ʼ�� 
 TMOD=0x11; 
 TH0=0xd8; 
 TL0=0xef; 
 EA=1; 
 ET0=1;
 ET1=1;
}  

void timer0() interrupt 1 using 3
{//timer0�жϷ�������ж�һ����ʱ10ms 
 counter++;//���Ĵ�������  
 TH0=0xd8; //���嵥λ���ĵ���ʱ��С  
 TL0=0xef; 
} 

void timer1() interrupt 3
{//timer1�жϷ������
 flag=1;
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
 while(!flag);
 	flag=0;
	TR1=0; 
} 
/*
void pwm_delay(uint x)
{
	uint time,i;
	time=0xffff-x+1;
	for(i=time;i>0;i--);
}
*/
void main() 
{  
 init(); 
 while(1) 
   { 
      k=0; 
   while(tune[k]!=NOTE_END)//��Ϊ�����Ľ�β����Ϊ�ļ����� ������ʾ��  0  
   { 
	/*�ӱ���ȡ�� �������ߣ�Ҳ������������ǵ������������Ǹ�
��,�����������������ǾͿ���ȷ��������Ӧ�Ķ�ʱ����ֵ�ڱ��е�λ���ˡ� */
    if(tune[k]==NOTE_SONGEND)
	{
		k++;
	}
    sound_signalhigh=0xff&(tune[k]>>8); 
    sound_signallow=0xff&tune[k]; 
    //�ӱ���ȡ�� ���Ĵ�С  
    sound_pace=40*duration[k];//600Ϊһ�ĵ�ʱ�� ,�ڴ��޸�һ�ĵ�ʱ���� 
	k++;
    TR0=1; 
   //��������δ�ﵽʱ�򣬼���ѭ��������������������  
   while(counter!=sound_pace) 
      {  
       sound=~sound; 
       sound_delay(sound_signalhigh,sound_signallow); 
	  // pwm_delay(temp);
      } 
     counter=0;//���ļ�������0��������һ������  
     TR0=0; 
    } 
   delay(100); //�����ݳ���Ϻ�,��ʱһ��ʱ��     }  
} 
}


 