#include<reg52.h>   
#define DELAY_TIME 80 //SD���ĸ�λ���ʼ��ʱSPI����ʱ����������ʵ�������޸���ֵ����������SD����λ���ʼ��ʧ��
#define TRY_TIME 800    //��SD��д������֮�󣬶�ȡSD���Ļ�Ӧ����������TRY_TIME�Σ������TRY_TIME���ж�������Ӧ��������ʱ��������д��ʧ��
sbit SD_SCL=P1^3; //SD��ͬ��ʱ��  ����
sbit SD_SI =P1^1; //SD��ͬ������  ����
sbit SD_CS =P1^2; //SD��Ƭѡ   ����
sbit SD_SO =P1^0; //SD��ͬ������  ���
 
unsigned long laddr;
unsigned char SD_Reset();
unsigned char SD_Init();
unsigned char SD_Read_Sector(unsigned long addr);
unsigned char xdata  pbuf[512]; //���ݻ�����
unsigned char  pbu[10];
unsigned char is_init; //�ڳ�ʼ����ʱ�����ô˱���Ϊ1,                    
unsigned char bdata _dat;
sbit _dat7=_dat^7;
sbit _dat6=_dat^6;
sbit _dat5=_dat^5;
sbit _dat4=_dat^4;
sbit _dat3=_dat^3;
sbit _dat2=_dat^2;
sbit _dat1=_dat^1;
sbit _dat0=_dat^0;
void delay(unsigned int time)
{
 while(time--);
}
 
 
 
void SD_spi_write(unsigned char x)
{
 _dat=x;
 SD_SI=_dat7;
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SI=_dat6;
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SI=_dat5;
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SI=_dat4;
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SI=_dat3;
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SI=_dat2;
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SI=_dat1;
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SI=_dat0;
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME); 
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
}
 
 
unsigned char SD_spi_read() //SPI��һ���ֽ�
{ 
 SD_SO=1;
 
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 _dat7=SD_SO;
 
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 _dat6=SD_SO;
 
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 _dat5=SD_SO;
 
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 _dat4=SD_SO;
 
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 _dat3=SD_SO;
 
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 _dat2=SD_SO;
 
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 _dat1=SD_SO;
 
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 _dat0=SD_SO;
 
 return (_dat);
}
 
 
unsigned char SD_Write_Cmd(unsigned char *pcmd) //��SD��д���pcmd�������ֽ����е��׵�ַ
{
 unsigned char temp,tim;
 tim=0;
 SD_CS=1;
 SD_spi_write(0xff); //��߼����ԣ����û�������ЩSD�����ܲ�֧��  
 SD_CS=0;
 SD_spi_write(pcmd[0]);
 SD_spi_write(pcmd[1]);
 SD_spi_write(pcmd[2]);
 SD_spi_write(pcmd[3]);
 SD_spi_write(pcmd[4]);
 SD_spi_write(pcmd[5]); 
 do
 { 
  temp = SD_spi_read();//һֱ����ֱ�������Ĳ���0xff��ʱ
  tim++;
  
 }while((temp==0xff)&&(tim<TRY_TIME));
 
 return(temp);
}
 
 
 
 
 
unsigned char SD_Init() //��ʼ����ʹ��CMD1������1��
{ 
 unsigned char temp;
 unsigned char pcmd[] = {0x41,0x00,0x00,0x00,0x00,0xff}; //����1���ֽ�����
 
 SD_CS=0; //��Ƭѡ 
 temp=SD_Write_Cmd(pcmd);                 
 is_init=0; //��ʼ����ϣ���is_init����Ϊ0,Ϊ������Ժ�����ݴ����ٶ�   
 SD_CS=1;  //�ر�Ƭѡ
 SD_spi_write(0xff); //����SD���Ĳ���ʱ�������ﲹ8��ʱ��
 return(temp); //����0,˵����ʼ�������ɹ�
}
 
unsigned char SD_Read_Sector(unsigned long addr)//��SD����ָ�������ж���512���ֽڣ�ʹ��CMD17��17�����
{unsigned char temp;
 unsigned char pcmd[]={0x51,0x00,0x00,0x00,0x00,0xff}; //CMD17���ֽ�����  
 addr<<=9; //addr=addr*512    �����ַ��������ַ��תΪ�ֽڵ�ַ
 pcmd[1]=((addr&0xff000000)>>24);//���ֽڵ�ַд�뵽CMD17�ֽ�������
 pcmd[2]=((addr&0x00FF0000)>>16);
 pcmd[3]=((addr&0x0000FF00)>>8);
 SD_CS=0;//��Ƭѡ
 //time=0;  
 temp=SD_Write_Cmd(pcmd); //д��CMD17
 return (temp);
}
unsigned char SD_Reset()//SD����λ������SPIģʽ��ʹ��CMD0������0��
{
 unsigned char temp,i;
 unsigned char pcmd[] = {0x40,0x00,0x00,0x00,0x00,0x95}; //����0���ֽ�����
 is_init=1; //��is_init��Ϊ1
 SD_CS=1;  //�ر�Ƭѡ
 for(i=0;i<0x0f;i++) //��λʱ������Ҫ��������74��ʱ���źţ����Ǳ���ģ�����
 {
  SD_spi_write(0xff);//120��ʱ��
 }
 
 SD_CS=0; //��Ƭѡ      
 temp=SD_Write_Cmd(pcmd);//д��CMD0   
 SD_CS=1; //�ر�Ƭѡ
 //����SD���Ĳ���ʱ�������ﲹ8��ʱ��
 return (temp);//����0,˵����λ�����ɹ�
 
}
void main()
{unsigned char t,s;
 unsigned int j;
 laddr=1892;
 //P2=0x00;
 t=0x00;
 while(t!=0x01)
 {t=SD_Reset();}
 SD_spi_write(0xff);
 //P2=0x0f;
 while(t!=0x00)
 {t=SD_Init();}    
 //SD_Write_Sector(ADDR,pbuf);//�����ݻ������е�512���ֽڵ�����д��SD���ĵ�ADDR������
 
 t=0xff;
 while(t!=0x00)
 {t=SD_Read_Sector(laddr);}//��SD���ĵ�ADDR�����ж�ȡ512���ֽڵ����ݵ����ݻ�����
 //P2=0xca;
 while (t!=0xfe){t=SD_spi_read(); //һֱ����������0xfeʱ��˵���������512�ֽڵ�������                            
            
                    }
 
  //P2=0x0f;
 for(j=0;j<512;j++)       //������д�뵽���ݻ�������
 {   
  pbuf[j]=SD_spi_read();
 }   
 for(j=0;j<10;j++)  //������д�뵽���ݻ�������
 {   
  pbu[j]=pbuf[j];
 }                                                                                                        
 SD_spi_read();
 SD_spi_read();//��ȡ�����ֽڵ�CRCУ���룬���ù�������
 SD_CS=1;  //SD���ر�Ƭѡ
 SD_spi_write(0xff);//����SD���Ĳ���ʱ�������ﲹ8��ʱ��
 for(j=0;j<10;j++)  //������д�뵽���ݻ�������
 {   
   P2=pbu[j];
   for(s=0;s<200;s++){delay(1000);}
 
 }          
 
 //P2=0x0c;
 while(1);
}