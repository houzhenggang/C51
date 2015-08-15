#include <reg52.h>
#include <intrins.h>

void delay1();
int m,n;
unsigned char temp1,temp2;

void main()
{
	temp1=0xfe;
	temp2=0x7f;
	//���300ms����������ż������ѭ������
	for(m=0;m<3;m++)
	{
		P1=0xAA;
		delay1();
		P1=0x55;
		delay1();
	}
	//һ��������ѭ������
	for(m=0;m<3;m++)
	{
		for(n=0;n<8;n++)
		{
			P1=temp1;
			temp1= _crol_(temp1,1);
			delay1();
		}
		for(n=0;n<8;n++)
		{
			P1=temp2;
			temp2= _cror_(temp2,1);
			delay1();
		}
	}
	//�����ֱ���������м���������,�ٴ��м���������������
	for(m=0;m<3;m++)
	{
		for(n=0;n<4;n++)
		{
			P1=temp1&temp2;
			temp1= _crol_(temp1,1);
			temp2= _cror_(temp2,1);
			delay1();
		}
		for(n=0;n<4;n++)
		{
			P1=temp1&temp2;
			temp1= _crol_(temp1,1);
			temp2= _cror_(temp2,1);
			delay1();
		}
	}
	//8��ȫ����˸3��
	for(m=0;m<3;m++)
	{
		P1=0x00;
		delay1();
		P1=0xff;
		delay1();
	}
	//�رշ����
	P1=0xff;
	delay1();
}

void delay1()
{
	int i,j;
	for(i=250;i>0;i--)
		for(j=230;j>0;j--);
}

