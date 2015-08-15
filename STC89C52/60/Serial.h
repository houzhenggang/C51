#ifndef __SERIAL_H__
#define __SERIAL_H__

#include<string.h>
#include<intrins.h>
#include <stdlib.h>
#define uchar unsigned char
#define uint unsigned int

uchar Serialflag=1,a,i=0,len;
long position;//,num,temp,m,count;
uchar xdata inbuff[32];
/***********************************************
*                       ���ڳ�ʼ��
************************************************/
void SerialInit()
{
	TMOD=0x21;//���ö�ʱ��1Ϊ������ʽ2
	TH1=0xfd;
	TL1=0xfd;//
	TR1=1;
	//
	REN=1;
	SM0=0;
	SM1=1;
	EA=1;
	ES=1;
	//
	position=200;
	Serialflag=1;
	memset(inbuff,0x00,32);
}
/***********************************************
*                       ���ڷ����ַ�
************************************************/
void SerialPutByte(uchar c)
{
	SBUF=c;
	while(!TI);
	TI=0;
}
/***********************************************
*                       ���ڷ����ַ���
************************************************/
void SerialPutString(uchar *puts)
{
	int j;
	int n;
	ES=0;
	n=strlen(puts);
	//short showdata=0;
	for(j=0;j<n;j++)
		SerialPutByte(puts[j]);
//	SerialPutByte('\n');
//	SerialPutByte('\r');
//	memset(puts,0x00,32);
	ES=1;
}
/***********************************************
*                       ���ڷ�����'\n'��β���ַ���
************************************************/
void SerialPutStr(uchar *puts)
{
	int j;
	int n;
	ES=0;
	n=strlen(puts);
	//short showdata=0;
	for(j=0;j<n;j++)
		SerialPutByte(puts[j]);
	SerialPutByte('\n');
//	SerialPutByte('\r');
//	memset(puts,0x00,32);
	ES=1;
}
/***********************************************
*                       �����жϺ���
************************************************/
void serialInt() interrupt 4
{
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
			inbuff[i]='\0';
			i=0;
			Serialflag=1;
		}
	}
	//P1=0x00;
	position=atoi(inbuff);
}
/***********************************************
*                       
************************************************/
long getIndex()
{
	return position;
}
/***********************************************
*                       ���ڼ���Ƿ�������
************************************************/
uchar SerailRevCheck()
{
	return Serialflag;
}
/***********************************************
*                       ���ڼ���Ƿ�������
************************************************/
void SerailSetCheck()
{
	Serialflag=0;
}
/***********************************************
*                       ���ڽ�������
************************************************/
void SerialRevData(uchar buff[32])
{
	Serialflag=0;
	len=strlen(inbuff);
	memcpy(buff,inbuff,strlen(inbuff));
	buff[strlen(inbuff)]='\0';
	memset(inbuff,0x00,32);

	//return buff;
}
/***********************************************
*                       ���ڻ�ȡ�������ݳ���
************************************************/
uchar SerialGetDataLen()
{
	return len;
}

#endif 
