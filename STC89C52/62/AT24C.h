/*
*���ļ���AT24C02�洢��IICͨ��Э��ӿڣ�������һЩ��IICͨ���йصĺ�����ͨ��
*���ñ��ļ����ܹ���AT24C02�洢ģ�����һЩ��������
*/
#ifndef __AT24C_H__
#define __AT24C_H__

#define uint unsigned int
#define uchar unsigned char
//����IIC��ʱ�Ӻ������߱���
sbit sda=P2^0;
sbit scl=P2^1;

/*********************************
*����������ʱ����
*********************************/
void storeDelay()
{ ;; }
/*********************************
*��ʼ��IICͨ��Э��
*********************************/
void initStore()
{
	sda=1;
	storeDelay();
	scl=1;
	storeDelay();
}
/*********************************
*����Э�鿪ʼ�źź���
*********************************/
void storeStart()  //��ʼ�ź�
{	
	sda=1;
	storeDelay();
	scl=1;
	storeDelay();
	sda=0;
	storeDelay();
}
/*********************************
*����Э��ֹͣ����
*********************************/
void storeStop()   //ֹͣ
{
	sda=0;
	storeDelay();
	scl=1;
	storeDelay();
	sda=1;
	storeDelay();
}
/*********************************
*����Э��Ӧ����
*********************************/
void storeRespons()  //Ӧ��
{
	uchar i;
	scl=1;
	storeDelay();
	//ͨ����ʱ�ķ�ʽ������Ӧ��
	while((sda==1)&&(i<250))i++;
	scl=0;
	storeDelay();
}
/*********************************
*�����ֽ�д�뺯��
*********************************/
void storeWriteByte(uchar date)
{
	uchar i,temp;
	temp=date;

	//ʵ��λ��д��
	for(i=0;i<8;i++)
	{
		temp=temp<<1;
		scl=0;
	    storeDelay();
		sda=CY;
		storeDelay();
		scl=1;
		storeDelay();
	//	scl=0;
     //   storeDelay();
	}
	scl=0;
	storeDelay();
	sda=1;
	storeDelay();
}
/*********************************
*�����ֽڶ�ȡ����
*********************************/
uchar storeReadByte()
{
	uchar i,k;
	scl=0;
	storeDelay();
	sda=1;
	storeDelay();
	//ʵ��λ�Ķ�ȡ
	for(i=0;i<8;i++)
	{
		scl=1;
		storeDelay();	
		k=(k<<1)|sda;
		scl=0;
		storeDelay();	
	}
	return k;
}
/*********************************
*����д��ַ����
*********************************/
void storeWriteAdd(uchar address,uchar date)
{
	storeStart();
	storeWriteByte(0xa0);
	storeRespons();
	storeWriteByte(address);
	storeRespons();
	storeWriteByte(date);
	storeRespons();
	storeStop();
}
/*********************************
*�������ַ����
*********************************/
uchar storeReadAdd(uchar address)
{
	uchar date;
	storeStart();
	storeWriteByte(0xa0);
	storeRespons();
	storeWriteByte(address);
	storeRespons();
	storeStart();
	storeWriteByte(0xa1);
	storeRespons();
	date=storeReadByte();
	storeStop();
	return date;
}

#endif
