#include<reg52.h>           
#define uchar unsigned char 
#define uint unsigned int 

sbit sound=P2^3; 
uint counter=0; 

uchar code FREQH[3][8]=                    //������ʱ����ֵ,����8�����ĸ߰�λ 
{//ע��ÿ����ʼ��Ϊ0xff����Ϊ������û�� 0 ����� ������������˸����֣����治����
//������ 
     // ������ 
    //  1       2      3      4      5      6      7 
  {0xff,0xf9,   0xfa,  0xfa,  0xfb,  0xfb,  0xfc,  0xfc}, 
       //������ 
      //1       2      3      4      5      6      7 
  {0xff,0xfc,   0xfd,  0xfd,  0xfd,  0xfd,  0xfe,  0xfe}, 
       //����: 
      //1       2      3      4      5      6      7 
  {0xff,0xfe,   0xfe,  0xfe,  0xfe,  0xfe,  0xff,  0x00}//0x00 
};  

uchar code FREQL[3][8]=                    //������ʱ����ֵ,����8�����ĵͰ�λ 
{ 
     // ������ 
    //  1       2      3      4      5      6      7 
  {0xff,0x5b,   0x15,  0xb9,  0x04,  0x90,  0x0c,  0x79}, 
       //������ 
      // 1      2      3      4      5      6      7 
  {0xff,0xac,   0x09,  0x5c,  0x82,  0xc8,  0x05,  0x3c}, 
       //����: 
      //1       2      3      4      5      6      7 
  {0xff,0x55,   0x84,  0xad,  0xc0,  0xe3,  0x02,  0x00}//0x00 
}; 

uchar code MUSIC[]= 
{//���տ��� 
//��������,ÿ������ʾһ�����������ģ���һ����ʾ����1234567���ڶ�����ʾ���ߣ�0��
//ʾ������1��ʾ������2��ʾ��������������ʾ���ĳ��ȣ�1��ʾ���ģ�2��ʾһ�ģ��Դ�
//���ơ� 
 6,1,3,  5,1,1,  3,1,2,   5,1,2,   1,2,2,  6,1,1,  5,1,1,  6,1,4,   3,1,1,   5,1,1,  6,1,1,  
 5,1,2,  3,1,2,  
 1,1,1,  6,0,1,  5,1,1,   3,1,1,   2,1,4,  2,1,3,  3,1,1,  5,1,2,   5,1,1,   6,1,1,   
 3,1,2,  2,1,2,  1,1,4,   5,1,4,   3,1,1,  2,1,1,  1,1,1,  6,0,1,   1,1,1,   5,0,8,  0 
}; 

void init() 
{//��ʱ����ʼ�� 
 TMOD=0x11; 
 TH0=0xd8; 
 TL0=0xef; 
 EA=1; 
 ET0=1; 
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

void main() 
{ 
 uint i,j,k=0; 
 uchar sound_signalhigh,sound_signallow;//��ʱ���ĸ�8λ���8λ 
 uchar sound_pace;//������ļ�����  
 init(); 
 while(1) 
   { 
      k=0; 
   while(MUSIC[k]!=0)//��Ϊ�����Ľ�β����Ϊ�ļ����� ������ʾ��  0  
   { 
    j=MUSIC[k++]; //�ӱ���ȡ�� ������С  
    i=MUSIC[k++]; /*�ӱ���ȡ�� �������ߣ�Ҳ������������ǵ������������Ǹ�
��,�����������������ǾͿ���ȷ��������Ӧ�Ķ�ʱ����ֵ�ڱ��е�λ���ˡ� */
    sound_signalhigh=FREQH[i][j]; 
    sound_signallow=FREQL[i][j]; 
    //�ӱ���ȡ�� ���Ĵ�С  
    sound_pace=60*MUSIC[k++];//600Ϊһ�ĵ�ʱ�� ,�ڴ��޸�һ�ĵ�ʱ���� 
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
   delay(100); //�����ݳ���Ϻ�,��ʱһ��ʱ��     }  
} 
}


 