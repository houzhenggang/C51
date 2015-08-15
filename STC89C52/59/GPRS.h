/*********************************
*���ļ���GPRSͨ�Žӿڣ�������һЩ��GPRSͨ���йصĺ�����ͨ��
*���ñ��ļ����ܹ���GPRSͨ�Ž���һЩ��������
*********************************/
#ifndef __GPRS_H__
#define __GPRS_H__

#include <stdio.h>
#include <math.h>
//#include "Serial.h"
//����GPRSͨ�ŵ�ATָ��
uchar code s1[]="AT+CMGF=0\r\n";                          //�趨Textģʽ
uchar code s2[]="AT+CMGS=27\r\n";                         //ע��һ��Ҫ���� �� ;\r\n �� ���ͻ��С��س���
uchar code s3[]="0011000D91685102801325F70008A00C62116B63572853D177ED4FE1";    //��������
uchar code s4=0x1a;                                       //Ctrl+Z 
uchar code s5[]="\r\n";
uchar xdata sensor[30];
/***********************************************
*                       �ӳ�1ms����
************************************************/
void GPRSdelay(int n)
{
	int x,y;
	for(x=n;x>0;x--)
		for(y=110;y>0;y--);
}
/***********************************************
*                       ���Ͷ���
************************************************/
void sendsms()
{
	SerialPutString(s1);
	GPRSdelay(2000);
	SerialPutString(s2);
	GPRSdelay(3000);
	SerialPutString(s3);
	GPRSdelay(2000);
	SerialPutString(&s4);
	GPRSdelay(2000);
}
/***********************************************
*                       ��绰
************************************************/
void DialVoiceCall()
{
  SerialPutString("AT+COLP=1\r\n");//dial the number
  GPRSdelay(2000);
  SerialPutString("ATD15200831527;\r\n");//dial the number
  GPRSdelay(2000);
}
/***********************************************
*                       GPRS�ɼ�����
************************************************/
void ConnectGPRS(float dust,float t,float h)
{
  //��ʼ��GPRS
  /*
  SerialPutString("AT+CGCLASS=\"B\"\r\n");
  GPRSdelay(2000);
  SerialPutString("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");
  GPRSdelay(2000);
  SerialPutString("AT+CGATT=1\r\n");
  GPRSdelay(2000);
  SerialPutString("AT+CIPCSGP=1,\"CMNET\"\r\n");
  GPRSdelay(2000);
  SerialPutString("AT+CIPCSGP=0\r\n");
  GPRSdelay(2000);
  SerialPutString("AT+CDNSCFG=\"211.91.216.129\"\r\n");
  GPRSdelay(2000);
  */
  //����UDPͨ��
  memset(sensor,0x00,30);
  SerialPutString("AT+CLPORT=\"UDP\",\"3033\"\r\n");//set a port
  GPRSdelay(2000);
  SerialPutString("AT+CIPSTART=\"UDP\",\"125.221.182.95\",\"8080\"\r\n");//set destination IP
  GPRSdelay(2000);
  SerialPutString("AT+CIPSEND\r\n");//dial the number
  GPRSdelay(2000);
  //��������
  //mySerial.print("PM2.5 value:");
  sprintf(sensor,"%.2f,%.2f,%.2f",dust,t,h);//
  SerialPutString(sensor);
  //memset(sensor,0x00,10);
  //sprintf(sensor,",%.2f",t);//
  //GPRSdelay(200);
  //SerialPutByte(',');
  //SerialPutString(t);
  //memset(sensor,0x00,10);
  //sprintf(sensor,",%.2f",h);//
  //GPRSdelay(200);
  //SerialPutByte(',');
 // SerialPutString(h);
 //�������ݷ��ͣ����Ͽ�����
  SerialPutByte(s4);
  SerialPutByte(s4);
  GPRSdelay(2000);
  SerialPutString("AT+CIPCLOSE\r\n");//dial the number
  GPRSdelay(2000);
  SerialPutString("AT+CIPSHUT\r\n");
  GPRSdelay(2000);
}

#endif 
