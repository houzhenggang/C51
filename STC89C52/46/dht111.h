#ifndef __DHT11_H__
#define __HDT11_H__

//�趨��־(static unsigned char status)�ĺ�ֵ
#define OK 1
#define ERROR 0 

extern void Delay_1ms(unsigned int ms); 
//�����ķ���ֵ��ʾ��ȡ�����Ƿ�ɹ� OK ��ʾ�ɹ� ERROR ��ʾʧ��
extern unsigned char DHT11_ReadTempAndHumi(void); 

#endif 
