#include <reg51.h> 
#include <intrins.h> 
#include <math.h>    
#include <stdio.h>   
#define uchar unsigned char
#define uint unsigned int
#define   Data_0_time    4
uchar disdat[6];
uchar code reg[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
typedef unsigned char  U8;       /* defined for unsigned 8-bits integer variable 	  �޷���8λ���ͱ���  */
typedef unsigned int   U16;      /* defined for unsigned 16-bits integer variable 	  �޷���16λ���ͱ��� */
U8  U8FLAG,k;
U8  U8count,U8temp;
U8  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
U8  U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
U8  U8comdata;
U8  outdata[5];  //���巢�͵��ֽ���	   
U8  indata[5];
U8  count, count_r=0;

U16 U16temp1,U16temp2;
sbit P1_1=P2^2;
//void Delay(void);
void Delay_10us(void);
void delay(void);
void display(void);
void display1(void);
void delay(void)		//��ʱ����
	{
	uchar i;
	for (i=0;i<201;i++)
		_nop_();
	}
void Delay(U16 j)
    {      U8 i;
	    for(;j>0;j--)
	  { 	
		for(i=0;i<27;i++);

	  }
    }
       void  Delay_10us(void)
      {
        U8 i;
        i--;
        i--;
        i--;
        i--;
        i--;
        i--;
       }
void display(void)           //��λ�������ʾ����disdat[]��Ҫ��ʾ�����ݣ���0��9�����֣���xsd������һλ��ʾС����
	{
	uchar i,disa,disb,disc;
	disb = 0x00;		//��ʾ��nλ
	for (i=0;i<6;i++)		//����ʾ6������
		{
		disa = disdat[i];	//��ʾ����
		if (disa < 10)
			{
			P2 = disb;      //���������
			disc = reg[disa];  //��ʾ����
			if (i==1||i == 4)
			disc = disc | 0x80;		//����С������ʾ
			P0 = disc;				//����ʾ
			delay ();				//��ʱ
			P0 = 0x00;              //
			disb++;					//��һ��Ҫ��ʾ��λ��
			}
		}
	P2 = 0xff;
	}
/*void display1(void)
{uchar i,disa,disb,disc;
 disb = 0x00;
 for(i=0;i<6;i++)
 {disa=disdat[i];
 if(disa<10)
 {P2=disb;
  disc=reg[disa];
  if(i==2)
  disc=0x40;
  if(i==4)
  disc=disc | 0x80;
  P0=disc;
  delay ();				//��ʱ
  P0 = 0x00;              //
  disb++;					//��һ��Ҫ��ʾ��λ��
			}
		}
P2 = 0xff;
	}*/

void init_uart()
{SCON=0X52;
 TMOD=0X20;
 TCON=0X69;
 TH1 =0XFD;
}
void  COM(void)
      {
     
	    U8 i;
		U8comdata=0;
       for(i=0;i<8;i++)	   
	    {
		
	   	    U8FLAG=2;
	   /*	//----------------------
	         P1_1=0 ;  //T
	         P1_1=1 ;  //T
	    //---------------------- 
	*/
	   	while((!P1_1)&&U8FLAG++);
			Delay_10us();
			Delay_10us();
			Delay_10us();
	  		U8temp=0;
	     if(P1_1)
		 {U8temp=1;
		    U8FLAG=2;
		 while((P1_1)&&U8FLAG++);
		 }	
	   /* //----------------------
	          P2_1=0 ;  //T
	          P2_1=1 ;  //T
	   //----------------------*/
	   	//��ʱ������forѭ��		  
	   	 //if(U8FLAG==1)break;
	   	//�ж�����λ��0����1	 
	   	   
		// ����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1 
	   	 else
		   U8temp=0;
		   U8comdata<<=1;
	   	   U8comdata|=U8temp;        //0
	     }//rof
	   //return(U8comdata);
	}
	
	//--------------------------------
	//-----ʪ�ȶ�ȡ�ӳ��� ------------
	//--------------------------------
	//----���±�����Ϊȫ�ֱ���--------
	//----�¶ȸ�8λ== U8T_data_H------
	//----�¶ȵ�8λ== U8T_data_L------
	//----ʪ�ȸ�8λ== U8RH_data_H-----
	//----ʪ�ȵ�8λ== U8RH_data_L-----
	//----У�� 8λ == U8checkdata-----
	//----��������ӳ�������----------
	//---- Delay();, Delay_10us();,COM(); 
	//--------------------------------

	void RH(void)
	{
	  //��������18ms 
       P1_1=0;
	   Delay(180);
	   P1_1=1;
	 //������������������ ������ʱ40us
	   Delay_10us();
	   Delay_10us();
	   Delay_10us();
	   Delay_10us();
	 //������Ϊ���� �жϴӻ���Ӧ�ź� 
	   P1_1=1;
	 //�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������	  
	   if(!P1_1)		 //T !	  
	   {
	   U8FLAG=2;
	 //�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����	 
	   while((!P1_1)&&U8FLAG++);
	   U8FLAG=2;
	 //�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
	   while((P1_1)&&U8FLAG++);
	 //���ݽ���״̬		 
	   COM();
	   U8RH_data_H_temp=U8comdata;
	   COM();
	   U8RH_data_L_temp=U8comdata;
	   COM();
	   U8T_data_H_temp=U8comdata;
	   COM();
	   U8T_data_L_temp=U8comdata;
	   COM();
	   U8checkdata_temp=U8comdata;
	   P1_1=1;
	   P1=U8comdata;
	 //����У�� 
	 
	   U8temp=(U8T_data_H_temp+U8T_data_L_temp+U8RH_data_H_temp+U8RH_data_L_temp);
	   if(U8temp==U8checkdata_temp)
	   {
	   	  U8RH_data_H=U8RH_data_H_temp;
	   	  U8RH_data_L=U8RH_data_L_temp;
		  U8T_data_H=U8T_data_H_temp;
	   	  U8T_data_L=U8T_data_L_temp;
	   	  U8checkdata=U8checkdata_temp;
	   }//fi
	   }//fi

	}
void main()
{
    unsigned char c;
	unsigned int a,i;
    init_uart();
	Delay(1);         //��ʱ100US��12M����)
	while(1)
	{  

	   //------------------------
	   //������ʪ�ȶ�ȡ�ӳ��� 
	   RH();
	   c=U8RH_data_H;
	   disdat[0]=c/10;
       disdat[1]=c%10;
	   disdat[2]=U8RH_data_L;
	   a=U8T_data_H;
       disdat[3]=a/10;
       disdat[4]=a%10;
       disdat[5]=U8T_data_L;
	   i=255;
	   while(i--)
       display(); //��ʾ��ʪ��
	/*   //������ʾ���� 
	   //--------------------------

	   str[0]=U8RH_data_H;
	   str[1]=U8RH_data_L;
	   str[2]=U8T_data_H;
	   str[3]=U8T_data_L;
	   str[4]=U8checkdata;
	   SendData(str) ;  //���͵�����  
	   //��ȡģ���������ڲ���С�� 2S */
	   //Delay(10);
	}//elihw
	
}// main

