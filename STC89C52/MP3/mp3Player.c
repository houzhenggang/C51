#include <string.h>
#include <stdio.h>
#include <math.h>
#include <Serial.h>
#include <fat32.h>

void main()
{
 //delay(10000);
 SerialInit();  //���ڳ�ʼ��������������ն˷�������
 while(1)
 {
 SerialPutString("yahoo!!!"); //����һ�������ַ���
 }
/*
 MMC_Init(); //SD����ʼ��
 delay(10000);
 MMC_get_volume_info();	  //���SD�������Ϣ��������ն�
 FAT32_Init(&Init_Arg);	  //FAT32�ļ�ϵͳ��ʼ����װ�����
 Printf("BPB_Sector_No"  ,Init_Arg.BPB_Sector_No);
 Printf("Total_Size"     ,Init_Arg.Total_Size   );
 Printf("FirstDirClust"  ,Init_Arg.FirstDirClust); 
 Printf("FirstDataSector",Init_Arg.FirstDataSector); 
 Printf("BytesPerSector" ,Init_Arg.BytesPerSector); 
 Printf("FATsectors"     ,Init_Arg.FATsectors); 
 Printf("SectorsPerClust",Init_Arg.SectorsPerClust);
 Printf("FirstFATSector" ,Init_Arg.FirstFATSector); 
 Printf("FirstDirSector" ,Init_Arg.FirstDirSector);   //���ϼ�����������������ֵ���ն�
 Printf("FAT32_OpenFile" ,(FAT32_OpenFile("\\TEST.TXT"))->FileSize); //�򿪸�Ŀ¼�µ�TEST.TXT�ļ���������ļ���С 
 FAT32_ReadFile(&FileInfo);  //��ȡ�ļ����ݣ�������ն�
*/
// while(1);
}
