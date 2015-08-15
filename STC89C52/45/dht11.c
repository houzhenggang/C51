#include <reg52.h>
#define uchar unsigned char
sbit dht11=P2^0;
uchar data_byte,RH,RL,TH,TL;

void delay(int x)            //delay
{
    uchar a,b;
    for(a=x;a>0;a--)
      for(b=60;b>0;b--);
}
void delay_1(int x)
{
	uchar i;
	i=2*x;
	while(i--);
}

void start()//DHT11ʪ�ȿ�ʼ�ź�
{
    dht11=1;
    delay(1);
    dht11=0;
    delay(30);//>18ms
    dht11=1;
    delay_1(10);//20-40us
}

uchar receive_byte()
{
    uchar i,tmp,count;
    for(i=0;i<8;i++)             //ѭ��8��ֱ����ȡ�������ݣ�8bit��
    {
        count=2;
        while((!dht11)&&count++)//�ȴ�50us�͵�ƽ����
        tmp=0;                    // Ĭ�Ͻ�tmp��0
        delay_1(20);            //70us��Ӧ��1����26~28us��Ӧ��0��
        if(dht11==1)tmp=1;
        count=2;
        while((dht11)&&count++);   //�ȴ�ֱ���͵�ƽ
        if(count==1)break;
        data_byte = data_byte<<1;     //����һλ
        data_byte|=tmp;                 //���¶�ȡ����ֵ���ϲ�
    }
    return data_byte;           
}

void receive()
{
    uchar T_H,T_L,R_H,R_L,check,num_check;
    uchar count;
    start();//��ʼ�ź�
    dht11=1;
    if(!dht11)//��ȡDHT11��Ӧ�ź�
    {
    count=2;
    while((!dht11)&&count++);//DHT11�ߵ�ƽ80us�Ƿ����
    count=2;
    while((dht11)&&count++);
    R_H=receive_byte();
    R_L=receive_byte();
    T_H=receive_byte();
    T_L=receive_byte();
    check=receive_byte();  //����У���8λ
    dht11=0;//������ʱ50us
    delay_1(20);
    dht11=1;
    num_check=R_H+R_L+T_H+T_L;
    if(num_check==check)
    {
        RH=R_H;
		P1=RH;
        RL=R_L;
        TH=T_H;
        TL=T_L;
        check=num_check;
    }
    }
}
void main()
{
	receive();
	delay(5000);
}
