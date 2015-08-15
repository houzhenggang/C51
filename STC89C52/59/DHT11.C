/*
*���ļ��ǵ�Ƭ�����ݲɼ��ͷ��͵���ڲ��֣�������Ӳ������ģ�飬����DUST.H�ļ�
*ʵ��PPD42NS�Է۳��Ĳɼ�������SERIAL.Hʵ�ִ���ͨ�ţ�����LCD.Hʵ��1602LCD����ʾ������
*GPRS.H�ļ�ʵ����SIM300ģ��GPRS���ݷ��ͺ�ͨ��
*/

#include <reg52.h>
#include <intrins.h> 
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Serial.h"
#include "LCD.h"
#include "GPRS.h"

#define uchar unsigned char
#define uint unsigned int

sbit P2_2 = P2^2 ;
//�������¶Ȳɼ��йصı���
uchar DHTFLAG,DHTtemp;
//uchar shidu_shi,shidu_ge,wendu_shi,wendu_ge;
uchar idata DHT_data_H,DHT_data_L,DHRH_data_H,DHRH_data_L,ucharcheckdata;
uchar idata DHT_data_H_temp,DHT_data_L_temp,DHRH_data_H_temp,DHRH_data_L_temp,ucharcheckdata_temp;
uchar DHTcomdata;
uchar xdata buff[10];
//����LCD��ʾ���ַ�����
uchar code info1[]="Temp:";
uchar code info2[]="Humid:";
uchar code info3[]="Dust:";
float idata temperature,humidity;
//����۳����ݲɼ�����ر���
sfr  T2MOD=0xC9;
sbit pulse=P3^2;
uint  counter=0;
float volatile lowpulseoccupancy;
uint code sampletime_ms = 6000;
float ratio,concentration;
/*********************************
*�����ӳ�1s����
*********************************/
void Delay(uint z)
{
uchar x,y;
for(x=z;x>0;x--)
   for(y=110;y>0;y--);
}
/*********************************
*�����ӳ�10us����
*********************************/
void Delay_10us(void)
   {
     uchar i;
     i--;
     i--;
     i--;
     i--;
     i--;
     i--;
    }
/*********************************
*�����¶Ȳɼ���ָ���
*********************************/
void DHTCOM(void)
{     
    uchar i;         
    for(i=0;i<8;i++)    
    {

     DHTFLAG=2; 
     while((!P2_2)&&DHTFLAG++);
    Delay_10us();
     Delay_10us();
     Delay_10us();
   DHTtemp=0;
     if(P2_2)DHTtemp=1;
     DHTFLAG=2;
    while((P2_2)&&DHTFLAG++);   
     if(DHTFLAG==1)break;    
     DHTcomdata<<=1;
     DHTcomdata|=DHTtemp; 
     }    
}
/*********************************
*�������Ͷ�����תʮ���ƺ���
*********************************/
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
	//	printf("%f,%d\n",y,m);
	}
	return y;
}
/*********************************
*���帡���Ͷ�����תʮ���ƺ���
*********************************/
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
/*********************************
*����DS18B20�¶ȶ�ȡ����
*********************************/
void RH(void)
{
	//�����¶ȴ����������ݵ�ʱ��ͼд��Ӧ��Э��
    P2_2=0;
    Delay(34);
    P2_2=1;
    Delay_10us();
    Delay_10us();
    Delay_10us();
    Delay_10us();
    P2_2=1;   
     if(!P2_2) 
     {
      DHTFLAG=2; 
      while((!P2_2)&&DHTFLAG++);
      DHTFLAG=2;
      while((P2_2)&&DHTFLAG++); 
      DHTCOM();
      DHRH_data_H_temp=DHTcomdata;
	  //P1=DHRH_data_H_temp;//
      DHTCOM();
      DHRH_data_L_temp=DHTcomdata;
      DHTCOM();
      DHT_data_H_temp=DHTcomdata;
	  //
	  //P1=DHT_data_H_temp;
      DHTCOM();
      DHT_data_L_temp=DHTcomdata;
	 // P1=DHT_data_L_temp;
      DHTCOM();
      ucharcheckdata_temp=DHTcomdata;
      P2_2=1; 
      DHTtemp=(DHT_data_H_temp+DHT_data_L_temp+DHRH_data_H_temp+DHRH_data_L_temp);
	  //���У�����ȷ���򸳸���Ӧ�ı���
       if(DHTtemp==ucharcheckdata_temp)
       {
          DHRH_data_H=DHRH_data_H_temp;
          DHRH_data_L=DHRH_data_L_temp;
          DHT_data_H=DHT_data_H_temp;
          DHT_data_L=DHT_data_L_temp;
          ucharcheckdata=ucharcheckdata_temp;
		  humidity=ToDem(DHRH_data_H_temp)+binToDem(DHRH_data_L_temp);
		  temperature=ToDem(DHT_data_H_temp)+binToDem(DHT_data_L_temp);
		 // temperature=(int)DHRH_data_H_temp;
		 // humidity=(int)DHT_data_H_temp;
		  sprintf(buff,"%2.2f,%2.2f;",temperature,humidity);
       }    
    }
//	return buff;
}
/*********************************
*�۳���������ȡ��ʼ��
*********************************/
void DustInit()
{
	lowpulseoccupancy = 0;
	//��ʱ��0���ݼĴ�����ʼ��
	TL0=0;
	TH0=0;
//	TR0=1;
	EX0=1;
	IT0=1;//��������
	//��ʱ��2���ݼĴ�����ʼֵ������
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
//	P1=TL0;
	lowpulseoccupancy = lowpulseoccupancy+ToDem(TL0);
	TR0=0;
	TL0=0;
}
/*********************************
*���嶨ʱ��2���жϺ���
*********************************/
void timer2() interrupt 5
{
	TF2=0;
	counter++;	
}
/***********************************************
*                       GPRS�ɼ�����
ע����ȡ��չram�����ݲ���ͨ���������ķ�ʽ������ֱ�����ã��������ֽ��ղ���������
************************************************/
void SendGPRS()
{
	//��ʼ��SIM300ģ��
	uchar idata sensor[10];
  //char temp=',';
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
	//��˳����UDPͨ�ŵ�ATָ��
  memset(sensor,0x00,10);
  SerialPutString("AT+CLPORT=\"UDP\",\"3033\"\r\n");//set a port
  GPRSdelay(2000);
  SerialPutString("AT+CIPSTART=\"UDP\",\"125.221.182.95\",\"8080\"\r\n");//set destination IP
  GPRSdelay(2000);
  SerialPutString("AT+CIPSEND\r\n");//dial the number
  GPRSdelay(2000);
  //��������
  //mySerial.print("PM2.5 value:");
  sprintf(sensor,"%.2f",concentration);//
  SerialPutString(sensor);
  memset(sensor,0x00,10);
  sprintf(sensor,",%.2f",temperature);//
  //GPRSdelay(200);
  //SerialPutByte(',');
  SerialPutString(sensor);
  memset(sensor,0x00,10);
  sprintf(sensor,",%.2f",humidity);//
  //GPRSdelay(200);
  //SerialPutByte(',');
  SerialPutString(sensor);
  //�������ݽ������Ͽ�����
  SerialPutByte(s4);
  SerialPutByte(s4);
  GPRSdelay(2000);
  SerialPutString("AT+CIPCLOSE\r\n");//dial the number
  GPRSdelay(2000);
  SerialPutString("AT+CIPSHUT\r\n");
  GPRSdelay(2000);
}
/*********************************
*���������
*********************************/
void main()
{
	//�������ݲɼ����м����
	uchar len,i;
	uchar xdata Tvalue[10],Hvalue[10],Dvalue[10];
	//��ʼ������
	SerialInit();
	//��ʼ��LCD
	LCDInit();
	//��ʼ���۳�������
	DustInit();
	sendsms();
	while(1)
	{ 
	    //�жϲ������ֵ�Ƿ�ﵽ
		if(counter>sampletime_ms)
		{
		//��ȡ�¶�����
			RH();
			TR2=0;
			EX0=0;
			//SerialPutStr(buff);
			memset(Tvalue,0x00,10);
			memset(Hvalue,0x00,10);
			memset(Dvalue,0x00,10);
			//��������תΪ�ַ���
			sprintf(Tvalue,"%2.2f",temperature);
			sprintf(Hvalue,"%2.2f%%",humidity);
			//��ȡ�۳�����
	    	counter=0;
		//	P1=0XAF;
			ratio = lowpulseoccupancy*100/sampletime_ms;  // Integer percentage 0=>100
	        concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
			sprintf(Dvalue,"%.2f",concentration);
			//������ͨ��GPRSģ�鷢�ͳ�ȥ
			//ConnectGPRS(Tvalue,Hvalue,Dvalue);
			SendGPRS();
		    //SerialPutStr(buff);
			lowpulseoccupancy = 0;
		    //TR2=1;

			//LCD��������������ʾ
			LCDWriteCom(0x01);//����
			LCDWriteCom(0x80);
			len=strlen(info1);
			//д���ݵ�LCD1602
			for(i=0;i<len;i++)
				LCDWriteData(info1[i]);
			len=strlen(Tvalue);
			for(i=0;i<len;i++)
				LCDWriteData(Tvalue[i]);
			LCDWriteData((uchar)223);
			LCDWriteData('C');
			LCDWriteCom(0x80+0x43);
			len=strlen(info2);
			for(i=0;i<len;i++)
				LCDWriteData(info2[i]);
			len=strlen(Hvalue);
			for(i=0;i<len;i++)
				LCDWriteData(Hvalue[i]);
		   //
		    //Delay(3000);
			GPRSdelay(2000);
			LCDWriteCom(0x01);//����
			LCDWriteCom(0x80);
			len=strlen(info3);
			//д���ݵ�LCD1602
			for(i=0;i<len;i++)
				LCDWriteData(info3[i]);
			len=strlen(Dvalue);
			for(i=0;i<len;i++)
				LCDWriteData(Dvalue[i]);
		   //
		    TR2=1;
		   	EX0=1;
		}	
	}

}