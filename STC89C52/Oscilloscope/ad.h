#include <intrins.h>
unsigned char a,A1,A2,A2t,A3;
sbit adrd=P3^7;                          //IO�ڶ���
sbit adwr=P3^6;
sbit diola=P2^5;
sbit dula=P2^6;
sbit wela=P2^7;
sbit led_stop=P2^5;
unsigned char j,k,adval;
extern void delay(unsigned char i);

	void ad()		//	������
	{
		dula=0;
		dula=1;
		P0=0x00;
		dula=0;
		
		led_stop=0;
		wela=1;
	    P0=0;                       //ѡͨADCS
	    adwr=0;			//ADд�루���д��ʲô���У���Ҫ��Ϊ������ADת����
	    _nop_();
	    adwr=1;
	    P0=0xff;                    //�ر�ADCS
	    delay(10);
	    wela=0;                     //�ر���ADƬѡ�ź���������������Է�ֹ�ڲ��������ʱʹAD��Ƭѡ�����仯
	    delay(50);                 //��Ҫע�����ADC0804��д�Ͷ�֮���ʱ����Ҫ�㹻�������޷���������
	                                //�������ʾ���ַ������ԭ��Ҳ��Ϊ������д��֮���ʱ����                                                                                                                                                                                                                           
	  								//��ȥ��ʾ��λ��
		wela=1;                    //���´���ADƬѡ�ź��������������
	    P1=0xff;                    //��ȡP1��֮ǰ�ȸ���дȫ1
	    P0=0;                       //ѡͨADCS
	    adrd=0;                     //AD��ʹ��
	    adval=P1;			//AD���ݶ�ȡ����P1��
	    adrd=1;
	    P0=0xff;                     //�ر�ADCS
	    adwr=0;
//	    P1=~adval;  /ͬʱ��AD��ֵ�Ͱ˸����������ʾ


	}
