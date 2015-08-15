/*********************************
*���ļ��Ƿ۳����ݲɼ��ӿڣ�������һЩ��۳��ɼ��йصĺ�����ͨ��
*���ñ��ļ����ܹ��Է۳�����������һЩ��������
*********************************/
#ifndef __DUST_H__
#define __DUST_H__

#include <stdio.h>
#include <string.h>
#include <math.h>

#define uchar unsigned char
#define uint unsigned int
//��������ر���
sfr  T2MOD=0xC9;
sbit pulse=P3^2;
//sbit buzzer=P2^3;

uint  counter=0;
float lowpulseoccupancy;
uint code sampletime_ms = 3000;
float ratio,concentration;
/*********************************
*��������ʱ1s����
*********************************/
void Dustdelay(int n)
{
	int x,y;
	for(x=n;x>0;x--)
		for(y=110;y>0;y--);
}
/*********************************
*�۳����������ݶ�ȡ��ʼ��
*********************************/
void DustInit()
{
	lowpulseoccupancy = 0;
	//��ʼ����ʱ��0
	TL0=0;
	TH0=0;
//	TR0=1;
	EX0=1;
	IT0=1;//��������
	//��ʱ��2���ݼĴ�����ʼ��
	RCAP2H=(65536-1000)/256;
	RCAP2L=(65536-1000)%256;
	T2CON=0;
	T2MOD=0;
	//����ʱ���жϺ�ȫ���ж�
	ET2=1;
	TR2=1;
	EA=1; 	
}
/*********************************
*�����ⲿ�жϺ���
*********************************/
void int1() interrupt 0 using 3
{
	TR0=1;
	while(pulse==0);
	P1=TL0;
	lowpulseoccupancy = lowpulseoccupancy+ToDem(TL0);
	TR0=0;
	TL0=0;
}
/*********************************
*���嶨ʱ��2�жϺ���
*********************************/
void timer2() interrupt 5
{
	TF2=0;
	counter++;	
}
/*********************************
*�������Ͷ�����תʮ���ƺ���
*********************************/
float DustToDem(uchar x)
{
	int i=0;
	float y=0;
	unsigned char m=0x01;
	if((x&m)==m)
		y=1;
	for(i=1;i<8;i++)
	{
		m=m<<1;
		if((x&m)==m)
			y=y+pow(2.0,i);
	//	printf("%f,%d\n",y,m);
	}
	return y;
}

#endif 
