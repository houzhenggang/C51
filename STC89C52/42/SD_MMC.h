/*****************************************************************************

                               SD����������
                            (�ü��棬ֻ����д��

*****************************************************************************/

#include <reg51.h>

/************************ �������� *************************/
sbit MISO = P3^0;  //SD���������
sbit CLK  = P3^1;  //SD��ʱ������
sbit MOSI = P3^2;  //SD����������
sbit CS   = P3^3;  //SD��Ƭѡʹ��

/*********************** ȫ�ֱ��� **************************/

unsigned char Buffer[76] _at_ 0x14; //ͨ�����ݻ�����,
                                    //��λ0x14��Ϊ����ϻ�����
union
{
    unsigned long all;
    unsigned char byte[4];
}Addr;                              //SD�����������ַ

/************************ �ⲿ���� *************************/
extern void Delay(unsigned int i);

/********************** д��һ���ֽ� ***********************/
void SD_write_byte(unsigned char dat)
{   
                        //������ѭ���ṹ��Ϊ����ߴ����ٶ�
    ACC = dat << 1;     //ֱ�ӷ���ACC��CY�ӿ촦���ٶ�
    MOSI =  CY; CLK = 0;
    ACC += ACC; CLK = 1;
    MOSI =  CY; CLK = 0;
    ACC += ACC; CLK = 1;
    MOSI =  CY; CLK = 0;
    ACC += ACC; CLK = 1;
    MOSI =  CY; CLK = 0;
    ACC += ACC; CLK = 1;
    MOSI =  CY; CLK = 0;                                  
    ACC += ACC; CLK = 1;
    MOSI =  CY; CLK = 0;
    ACC += ACC; CLK = 1;
    MOSI =  CY; CLK = 0;
    ACC += ACC; CLK = 1;
    MOSI =  CY; CLK = 0;
/*ACC += ACC*/; CLK = 1;
}

/********************* ����д��һ���ֽ� *********************/
SD_write_byte_low_speed(unsigned char x)
{
    unsigned char i;
    for(i = 8; i != 0; i--)
    {
        MOSI = x & 0x80;
        x <<= 1;
        CLK = 0;
        Delay(1);
        CLK = 1;
        Delay(1);
    }
}

/******************** ����Ӳ������һ�ֽ� ********************/
unsigned char SD_spi_read(void)
{   
    RI = 0; //����51���ڵ�ͬ����λ���ܣ��Դﵽ��ߵ��ٶ� 2MHz CLK
    while(RI == 0);  //�ȴ���ȡ���
    return SBUF;     //����ֵ LSB MSB ����
}

/********************** ���ٶ���һ�ֽ� ***********************/
unsigned char SD_read_byte_low_speed(void)
{
    unsigned char i, temp;
    for(i = 8; i != 0; i--)
    {
        CLK = 0;
        temp <<= 1;
        if(MISO) temp++;
        Delay(1);
        CLK = 1;
        Delay(1);
    }
    return temp;
}

/************************ ����һ�ֽ� *************************/
unsigned char SD_read_byte(void)
{
    unsigned char i;
    for(i = 8; i != 0; i--)
    {   
        ACC <<= 1;          //ֱ�ӷ���ACC�ӿ�ִ���ٶ�
        CLK = 0;
        if (MISO) ACC++;
        CLK = 1;
    }
    return ACC;
}

/********************* SD�����ʳ����� **********************/
void Error(unsigned char Err_code)
{
    unsigned char i = 0;
    CS = 1;
    TR0 = 0;                //ֹͣ����
    do                      //�ն�512�ֽ�
    {
        SD_spi_read();
        SD_spi_read();
    }
    while(--i);
    LCD3310_clear_screen(); //����
    LCD3310_set_XY(37,2);
    LCD3310_print(0x0e);    //��ʾ'E'
    LCD3310_print(Err_code);//��ʾ�������
    PCON |= 0x02;           //MCU����
    while(1);
}
                             
/************************ �ȴ�SD��Ӧ *************************/
void Wait_start_token(void)
{
    unsigned int i;
    RI = 0;
    i  = 20000;
    while(--i)
    {             
        if (SBUF == 0x7f) return;//�ȴ���ʼ����0xfe
        RI = 0;
    }
    Error(0);                   //��ʱ����
}

/*********************** ��������Ҫ������ ********************/
void Skip_nwords(unsigned char n)
{
    do
    {
        SD_spi_read();
        SD_spi_read();
    }           
    while(--n);
}

/******************* SD����n���ֽ����� ***********************/
void Read_nbytes(unsigned char n)
{
    unsigned char i = 0;
    do
    {
        Buffer[i] = SD_read_byte();
    }
    while(++i != n);
}

/********************** SD�� ��λ������ **********************/
void SD_init(void)
{
    //unsigned char i, timeout, retry, temp, cmd, crc;
    #define cmd     Buffer[0]
    #define i       Buffer[1]
    #define timeout Buffer[2]
    #define retry   Buffer[3]
    #define temp    Buffer[4]
    #define crc     Buffer[5]

    CS = 1;
    for(i = 15; i != 0; i--) //��λǰ,�ȷ���74������ʱ������
    {
        SD_write_byte_low_speed(0xff);
    }

    cmd = 0x40;              //CMD0
    crc = 0x95;              
    i = 2;                   
    do                       //�ֱ��� CMD0 �� CMD1
    {
        retry = 100;
        do
        {
            CS = 1;
            SD_write_byte_low_speed(0xff);
            CS = 0;
            SD_write_byte_low_speed(cmd);
            SD_write_byte_low_speed(0);
            SD_write_byte_low_speed(0);
            SD_write_byte_low_speed(0);
            SD_write_byte_low_speed(0);
            SD_write_byte_low_speed(crc);

            timeout = 100;
            do
            {
                temp = SD_read_byte_low_speed();
            }                       
            while(temp == 0xff && --timeout);//�ȴ�SD����Ӧ

            if(--retry == 0) Error(i);       //����ʧ�ܱ���
        }
        while(++temp != i);                  //SD��Ӧ�ֽ�ӦΪ0x01,0x00
        SD_write_byte_low_speed(0xff);       //����8��ʱ������
        CS = 1;             
        cmd = 0x41;                          //CMD17
        crc = 0xff;                          //αCRC
    }
    while(--i);

    #undef cmd
    #undef i   
    #undef timeout
    #undef retry
    #undef temp
    #undef crc
}

/************************ ���ÿ��С ************************/
void Set_block_size(unsigned char size)
{
    unsigned char timeout;
    CS = 0;
    SD_write_byte(0x50);     //����CMD16
    //MOSI = 0;
    SD_spi_read();           //����0x00
    SD_spi_read();           //����0x00
    if (size == 1)
    {
        SD_write_byte(2);    //����0x02
        //MOSI = 0;
        SD_spi_read();       //����0x00
    }
    else
    {
       SD_spi_read();        //����0x00
       SD_write_byte(4);     //����0x04
    }
    MOSI = 1;
    SD_spi_read();           //����0xff

    timeout = 100;
    do
    {
        if(--timeout == 0) Error(4);//��ʱ����
    }
    while(SD_spi_read() != 0);
    MOSI = 1;
    SD_spi_read();           //����8��ʱ������
    CS = 1;
}

/*********************** ���Ϳ������ ************************/
void SD_send_read_cmd(void)
{
    unsigned char timeout;
    CS = 0;
    SD_write_byte(0x51);     //����CMD17
    SD_write_byte(Addr.byte[0]);
    SD_write_byte(Addr.byte[1]);
    SD_write_byte(Addr.byte[2]);
    SD_write_byte(Addr.byte[3]);
    MOSI = 1;
    SD_spi_read();           //����0xff
    timeout = 100;
    do
    {
        if(--timeout == 0) Error(3);
    }
    while(SD_spi_read() != 0);
}

/********************* ����������� **************************/
void SD_read_block_complete(void)
{
    SD_spi_read();          //�������ֽ�У����
    SD_spi_read();          //
    MOSI = 1;
    SD_spi_read();          //����8��ʱ������
    CS = 1;                 //����һ�ο������
}
