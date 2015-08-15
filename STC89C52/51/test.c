#include <reg52.h>
/*
*���ļ��ǵ�Ƭ�����ݲɼ��ͷ��͵���ڲ��֣�������Ӳ������ģ�飬����
*DHT.H�ļ��ɼ��¶ȣ�����SERIAL.Hʵ�ִ���ͨ�ţ�����LCD1602.Hʵ��LCD����ʾ��
*���ļ��ڲ�ʵ����GPRSͨ��
*/

#include <stdio.h>
#include <math.h>
#include "LCD1602.h"
#include "Serial.h"
//#include "DHT.h"

#define uchar unsigned char
#define uint unsigned int

//sbit pulse=P3^2;
//����LCD��ʾ���ַ�����
uchar code str1[]="Current Temperat"; //��һ����ʾ����
uchar code str2[]="Current Humidity"; //�ڶ�����ʾ����
uchar code str3[]="Current Dust";      //
//������SIM300ģ��GPRSͨ�ŵ�ATָ��
uchar code s1[]="AT+CMGF=0\r\n";                          //�趨Textģʽ
uchar code s2[]="AT+CMGS=27\r\n";                         //ע��һ��Ҫ���� �� ;\r\n �� ���ͻ��С��س���
uchar code s3[]="0011000D91685102801325F70008A00C62116B63572853D177ED4FE1";    //��������
uchar s4=0x1a;                                       //Ctrl+Z 
uchar code s5[]="\r\n";   
//��������۳��ɼ��йصı���
unsigned long xdata  duration;
unsigned long xdata starttime;
unsigned long code sampletime_ms = 5000;
unsigned long xdata lowpulseoccupancy = 0;
float idata ratio = 0;
float idata concentration = 0;
uchar xdata value[40]="hello!";
char xdata sensor[10];
uchar count,len,i;
//������GPRSͨ���йصĺ���
//uchar THvalue[6];
void delay(int n);
void sendsms();
void DialVoiceCall();
void ConnectGPRS(float dust,float t,float h);
//������DHT11��صı���
sbit P2_2 = P2^2 ;
uchar DHTFLAG,DHTtemp;
uchar DHT_data_H,DHT_data_L,DHTRH_data_H,DHTRH_data_L,DHTcheckdata;
uchar DHT_data_H_temp,DHT_data_L_temp,DHTRH_data_H_temp,DHTRH_data_L_temp,DHTcheckdata_temp;
uchar DHTcomdata;
uchar xdata buff[20];
float temperature,humidity;
//������DHT11��صĺ���
void DHTDelay(uint z);
void DHTDelay_10us(void);
void DHTCOM(void);
void DHTCOM(void);
float ToDem(uchar x);
float binToDem(uchar x);
void RH(void);
//������ڲ���
void main()
{
	//uchar i;
	//EX1=1;
	//IT1=1;
	//��ʼ��LCD�ʹ���
	LCDInit();
	SerialInit();
	len=strlen(value);
	for(i=0;i<len;i++)
  		LCDWriteData(value[i]);
	//sendsms();
	//DialVoiceCall();
	//
	while(1)
	{
	//��ȡ���������ݣ������͵�GPRSģ��
		RH();
		SerialPutString(buff);
		 // SerialPutByte('\n');
		/*
		LCDWriteCom(0x01);//����
		LCDWriteCom(0x80);
		len=strlen(buff);
		for(i=0;i<len;i++)
	  		LCDWriteData(buff[i]);
			*/
	//	ConnectGPRS(0.3,0.4,0.8);
	}
}
/***********************************************
*                       �ӳ�1ms����
************************************************/
void delay(int n)
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
delay(2000);
SerialPutString(s2);
delay(3000);
SerialPutString(s3);
delay(2000);
SerialPutString(&s4);
delay(2000);
}
/***********************************************
*                       ��绰
************************************************/
void DialVoiceCall()
{
  SerialPutString("AT+COLP=1\r\n");//dial the number
  delay(2000);
  SerialPutString("ATD15200831527;\r\n");//dial the number
  delay(2000);
}
/***********************************************
*                       GPRS�ɼ�����
************************************************/
void ConnectGPRS(float dust,float t,float h)
{
 // char temp[40];
  /*
  SerialPutString("AT+CGCLASS=\"B\"\r\n");
  delay(2000);
  SerialPutString("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");
  delay(2000);
  SerialPutString("AT+CGATT=1\r\n");
  delay(2000);
  SerialPutString("AT+CIPCSGP=1,\"CMNET\"\r\n");
  delay(2000);
  SerialPutString("AT+CIPCSGP=0\r\n");
  delay(2000);
  SerialPutString("AT+CDNSCFG=\"211.91.216.129\"\r\n");
  delay(2000);
  */
  SerialPutString("AT+CLPORT=\"UDP\",\"3033\"\r\n");//set a port
  delay(2000);
  SerialPutString("AT+CIPSTART=\"UDP\",\"125.221.182.95\",\"8080\"\r\n");//set destination IP
  delay(2000);
  SerialPutString("AT+CIPSEND\r\n");//dial the number
  delay(2000);
  //mySerial.print("PM2.5 value:");
  sprintf(sensor,"%.2f",dust);//
  SerialPutString(sensor);
  sprintf(sensor,",%.2f",t);//
  SerialPutString(sensor);
  sprintf(sensor,",%.2f",h);//
  SerialPutString(sensor);
  SerialPutByte(s4);
  delay(2000);
  SerialPutString("AT+CIPCLOSE\r\n");//dial the number
  delay(2000);
  SerialPutString("AT+CIPSHUT\r\n");
  delay(2000);
}
/******************************
*����DHT�ӳ�1s����
********************************/
void DHTDelay(uint z)
{
uchar x,y;
for(x=z;x>0;x--)
   for(y=110;y>0;y--);
}
/******************************
*����DHT�ӳ�10us����
********************************/
void DHTDelay_10us(void)
   {
     uchar i;
     i--;
     i--;
     i--;
     i--;
     i--;
     i--;
    }
/******************************
*����DHT�����
********************************/
void DHTCOM(void)
{     
    uchar i;         
    for(i=0;i<8;i++)    
    {

     DHTFLAG=2; 
     while((!P2_2)&&DHTFLAG++);
    DHTDelay_10us();
     DHTDelay_10us();
     DHTDelay_10us();
   DHTtemp=0;
     if(P2_2)DHTtemp=1;
     DHTFLAG=2;
    while((P2_2)&&DHTFLAG++);   
     if(DHTFLAG==1)break;    
     DHTcomdata<<=1;
     DHTcomdata|=DHTtemp; 
     }    
}
/******************************
*�������Ͷ�����תʮ���ƺ���
********************************/
float ToDem(uchar x)
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
	}
	return y;
}

/******************************
*���帡���Ͷ�����תʮ���ƺ���
********************************/
float binToDem(uchar x)
{
	float y=0;
	uchar i,m;
	m=0x80;	
	for(i=0;i<4;i++)
	{
		if((x&(m>>i))>0)
			y=y+1/pow(2.0,i+1);
	}
	return y;
}
/******************************
*�����¶ȶ�ȡ����
********************************/
void RH(void)
{
      P2_2=0;
    DHTDelay(34);
    P2_2=1;
    DHTDelay_10us();
    DHTDelay_10us();
    DHTDelay_10us();
    DHTDelay_10us();
    P2_2=1;   
     if(!P2_2) 
     {
      DHTFLAG=2; 
      while((!P2_2)&&DHTFLAG++);
      DHTFLAG=2;
      while((P2_2)&&DHTFLAG++); 
      DHTCOM();
      DHTRH_data_H_temp=DHTcomdata;
	  //P1=DHTRH_data_H_temp;//
      DHTCOM();
      DHTRH_data_L_temp=DHTcomdata;
      DHTCOM();
      DHT_data_H_temp=DHTcomdata;
	  //
	  P1=DHT_data_H_temp;
      DHTCOM();
      DHT_data_L_temp=DHTcomdata;
	  //P1=DHT_data_L_temp;
      DHTCOM();
      DHTcheckdata_temp=DHTcomdata;
      P2_2=1; 
      DHTtemp=(DHT_data_H_temp+DHT_data_L_temp+DHTRH_data_H_temp+DHTRH_data_L_temp);
       if(DHTtemp==DHTcheckdata_temp)
       {
          DHTRH_data_H=DHTRH_data_H_temp;
          DHTRH_data_L=DHTRH_data_L_temp;
          DHT_data_H=DHT_data_H_temp;
          DHT_data_L=DHT_data_L_temp;
          DHTcheckdata=DHTcheckdata_temp;
		  humidity=ToDem(DHTRH_data_H_temp)+binToDem(DHTRH_data_L_temp);
		  temperature=ToDem(DHT_data_H_temp)+binToDem(DHT_data_L_temp);
		 // temperature=(int)DHTRH_data_H_temp;
		 // humidity=(int)DHT_data_H_temp;
		  sprintf(buff,"%2.2f,%2.2f;",temperature,humidity);//
       }    
    }
}



