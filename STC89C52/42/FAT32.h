/*****************************************************************************

                         FAT32 �ļ�ϵͳ51�ü���
                      (��֧�ָ�Ŀ¼�ļ��������ȡ)

*****************************************************************************/

#include <reg51.h>

union
{
    unsigned long all;
    unsigned char byte[4];
} File_length, Next_cluster;

unsigned long FAT_base;
unsigned long Dat_base;

//unsigned char Cluster_size;
//unsigned char Shift_bits;
#define Cluster_size TH1
#define Shift_bits   TL1


/******************************** ��ȡ���̲��� ******************************/
void Get_disk_info(void)
{
    unsigned int  Reserved;     //������
    unsigned char Signature;    //����������ʶ��
    unsigned char i;

    Addr.all = 0;               //ָ��0������
    SD_send_read_cmd();         //���Ͷ�ȡָ��
    Wait_start_token();         //�ȴ�SD����Ӧ

    Signature = SD_spi_read();  //��ȡ���ֽ�
    SD_spi_read();              //����1���ֽ�
    Skip_nwords(222);           //����444�ֽ�

    if (Signature == 0xd7)      //�Ƿ�Ϊ��ָ��"0xeb"
    {                           //���ǣ��ж�ΪDBR����
        Skip_nwords(0x20);      //�ն�64�ֽ�
    }
    else                        //�����ж�ΪMBR��������
    {
        i = 4;                  //���Ϊ4������
        while(1)                //˳������4��������Ϣ
        {
            Read_nbytes(0x10);  //��ȡ16�ֽڷ�����Ϣ
            if (Buffer[8] | Buffer[9])         //�粻Ϊ�㣬��Ϣ��Ч
            {
              //Addr.byte[0] = Buffer[11];     //��ȡ������ʼ������
                Addr.byte[1] = Buffer[10];
                Addr.byte[2] = Buffer[9];
                Addr.byte[3] = Buffer[8];     
                Addr.all = Addr.all << 9;      //ת��Ϊ��ַ
                while(--i) Read_nbytes(0x10);  //�ն�ʣ���ֽ�
                break; 
            }
            else
            {
                if(--i == 0)  Error(6);        //δ�ҵ�������Ϣ������
            }
        }
    }

    Signature = SD_spi_read();                 //��ȡ������ֽ�
    if (SD_spi_read() != 0x55 || Signature != 0xaa) Error(6);//������ֽ�ӦΪ0x55aa
    SD_read_block_complete();                  //����һ�ο������

    //---------------------- ��ȡDBR�������д��� ---------------------------
    SD_send_read_cmd();                           //���Ͷ�ȡָ��
    Wait_start_token();                           //�ȴ�SD����Ӧ
    Read_nbytes(64);                              //��ȡ64�ֽ�

    //------------------ ���������ṹ�Ƿ����FAT32�淶 ---------------------
    if (Buffer[0] != 0xeb) Error(7);              //��תָ��0xeb
    if (Buffer[0x0c] != 0x02 ||
        Buffer[0x0b] != 0x00 ||                   //������СΪӦΪ512�ֽ�
        Buffer[0x2c] != 0x02 ||                   //��Ŀ¼�غ�ӦΪ2
       (Buffer[0x2d]|Buffer[0x2e]|Buffer[0x2f]) != 0
        || Buffer[0x10] != 0x02 ||                //FAT��ӦΪ����
        Buffer[0x0d] == 0x00) Error(8);           //ÿ������������Ϊ0

    //-------------------------- �������õ���Ϣ ----------------------------
    Cluster_size = Buffer[0x0d];
    Reserved = Buffer[0x0e] + (Buffer[0x0f]<<8);  //������������
    FAT_base = Addr.all + (Reserved<<9);          //FAT��ַ = DBR��ַ + ��������С
    for(Shift_bits = 9; (Buffer[0x0d] & 1) == 0; Shift_bits++) Buffer[0x0d] >>= 1;
                                                  //����غ�תΪ��ַʱ��Ҫ��λ����
  //File_length.byte[0] = Buffer[0x27];           //��ȡÿ��FATռ�õ�������(����ֽ�Ϊ0)
    File_length.byte[1] = Buffer[0x26];
    File_length.byte[2] = Buffer[0x25];
    File_length.byte[3] = Buffer[0x24];
    Dat_base = FAT_base + (File_length.all << 10);//��Ŀ¼��ַ = FAT��ַ + ����FAT��С
    Dat_base -= 2ul << Shift_bits;                //���ڴغŴ�2���𣬰���������ʼ��ַ��ǰ��2��

    Skip_nwords(223);                             //�Թ�446�ֽ�
    Signature = SD_spi_read();                    //���������ֽ�ӦΪ0x55aa
    if (SD_spi_read() != 0x55 || Signature != 0xaa) Error(7);
    SD_read_block_complete();                     //����һ�ο������
}



/***************************** �Ѳ��Ŀ¼�е��ļ� ****************************/
unsigned char Scan_file(unsigned char n, unsigned char File_type)
{
    unsigned char Cluster_count;
    unsigned char Sector_count;
    unsigned char i,j,k;
    unsigned char code Rev[16] =            //ASCII�ַ��ߵ�λ����ת����
    {
        0x0c,0x8c,0x4c,0xcc,0x2c,0xac,0x6c,0xec,0x1c,0x9c,  //'0' ~ '9'
        0x42,0xb2,0x0a,0xea,0x82,0x6a,                      //"BMP" "WAV"
    };

    for (k = 3; k != 0; k--)                //Ԥ���ļ�����Buffer���Ա���бȽ�
    {
        Buffer[k + 7] = Rev[k + File_type]; //������չ��������"BMP"��"WAV"�ĵ�����
        Buffer[k - 1] = Rev[n%10];          //���������ʮ��λ�ĵ�����
        n = n / 10;                         //ʮ��������һλ
    }  

    Cluster_count = 100;                    //���Ƹ�Ŀ¼������ȣ���ֹ���ݴ���������
    Next_cluster.all = 2;                   //��Ŀ¼�Ӵغ�2��ʼ
    do                                      //���������ν����ļ�����
    {
        Sector_count = 0;
        Addr.all = Dat_base + (Next_cluster.all << Shift_bits); //����ػ�ַ
        do                                  //�������ڸ�������
        {  
            SD_send_read_cmd();             //���Ͷ�ȡָ��
            Addr.all += 512;                //������һ������ַ
            Wait_start_token();             //�ȴ�SD����Ӧ
            for (j = 16; j != 0; j--)       //���������ڸ����ļ���¼
            {   
                for (i = 0; i != 12; i++)   //�Ƚ��ļ�����11���ַ�
                {                           //ֻ�Ƚ��ļ���ǰ��λ���ֺ���չ������������
                    if (SD_spi_read() != Buffer[i] && (i & 0x07) < 3) break;
                }
                 
                if (i == 12)                                  //ƥ��ɹ�
                {
                    Cluster_count = 0;                        //����ؼ�����������������
                    Read_nbytes(20);                          //��ȡ�ļ���¼��Ϣ
                    Next_cluster.byte[0] = Buffer[0x15-12];   //��ȡ�ļ�����ʼ�غ�
                    Next_cluster.byte[1] = Buffer[0x14-12];
                    Next_cluster.byte[2] = Buffer[0x1b-12];
                    Next_cluster.byte[3] = Buffer[0x1a-12];
                    Addr.all = Dat_base + (Next_cluster.all << Shift_bits);//������ļ�����ʼ��ַ

                    File_length.byte[0] = Buffer[0x1f-12];    //��ȡ�ļ��ĳ���
                    File_length.byte[1] = Buffer[0x1e-12];
                    File_length.byte[2] = Buffer[0x1d-12];
                    File_length.byte[3] = Buffer[0x1c-12];
                    File_length.all = File_length.all >> 9;   //�����ļ�ռ�õ���������

                    while(--j) Skip_nwords(16);               //����һ������ʣ���ֽ�
                    break;
                }
                else                                          //ƥ�䲻�ɹ������Թ�ʣ���ֽ�
                {
                    i = 31 - i;
                    do
                    {
                         SD_spi_read();
                    }
                    while(--i);
                }
            }
            SD_read_block_complete();                         //����һ�ο������

            if (Cluster_count == 0) return 1;                 //�����ɹ�������
        }
        while(++Sector_count != Cluster_size);                //��������������һ����

        //------------------------- ���ݴ���ȡ����һ�غ� ------------------------------
        Set_block_size(0);                                    //�趨SD�����СΪ4�ֽ�
        Addr.all = FAT_base + (Next_cluster.all << 2);        //���ݴ������������ַ
        SD_send_read_cmd();                                   //���Ͷ�ȡָ��
        Wait_start_token();                                   //�ȴ�SD����Ӧ
        Next_cluster.byte[3] = SD_read_byte();//��ȡ��һ�شغ�
        Next_cluster.byte[2] = SD_read_byte();
        Next_cluster.byte[1] = SD_read_byte();
        Next_cluster.byte[0] = SD_read_byte();
        SD_read_block_complete();             //����һ�ο������

        Set_block_size(1);                    //�����趨SD�����СΪ512�ֽ�
        if (( Next_cluster.byte[3]            //�ж��Ƿ�Ϊ����������־ 0x0fffffff
            & Next_cluster.byte[2]
            & Next_cluster.byte[1]) == 0xff
           && Next_cluster.byte[0]  == 0x0f)
        break;
    }
    while(--Cluster_count != 0);              //ѭ��������һ�أ�ֱ���ﵽ�����Ŀ
    return 0;
}
