#include <sd.h>
#include <ctype.h>
/*
 ������¼�ṹ����
*/
struct PartRecord
{
 unsigned char Active;         //0x80��ʾ�˷�����Ч
 unsigned char StartHead;      //�����Ŀ�ʼͷ
 unsigned char StartCylSect[2];//��ʼ����������
 unsigned char PartType;       //��������
 unsigned char EndHead;        //�����Ľ���ͷ
 unsigned char EndCylSect[2];  //��������������
 unsigned char StartLBA[4];    //�����ĵ�һ������
 unsigned char Size[4];        //�����Ĵ�С 
};

/*
 ��������������0��������������
*/
struct PartSector
{
 unsigned char  PartCode[446]; //MBR����������
 struct PartRecord Part[4];      //4��������¼
 unsigned char BootSectSig0;
 unsigned char BootSectSig1;
};

struct FAT32_FAT_Item
{
 unsigned char Item[4];
};

struct FAT32_FAT
{
 struct FAT32_FAT_Item Items[128];
};

/*
 FAT32�ж�BPB�Ķ�������  һ��ռ��90���ֽ�
*/
struct FAT32_BPB
{
 unsigned char BS_jmpBoot[3];     //��תָ��            offset: 0
 unsigned char BS_OEMName[8];     //                    offset: 3
 unsigned char BPB_BytesPerSec[2];//ÿ�����ֽ���        offset:11
 unsigned char BPB_SecPerClus[1]; //ÿ��������          offset:13
 unsigned char BPB_RsvdSecCnt[2]; //����������Ŀ        offset:14
 unsigned char BPB_NumFATs[1];    //�˾���FAT����       offset:16
 unsigned char BPB_RootEntCnt[2]; //FAT32Ϊ0            offset:17
 unsigned char BPB_TotSec16[2];   //FAT32Ϊ0            offset:19
 unsigned char BPB_Media[1];      //�洢����            offset:21
 unsigned char BPB_FATSz16[2];    //FAT32Ϊ0            offset:22
 unsigned char BPB_SecPerTrk[2];  //�ŵ�������          offset:24
 unsigned char BPB_NumHeads[2];   //��ͷ��              offset:26
 unsigned char BPB_HiddSec[4];    //FAT��ǰ��������     offset:28
 unsigned char BPB_TotSec32[4];   //�þ���������        offset:32

 unsigned char BPB_FATSz32[4];    //һ��FAT��������     offset:36
 unsigned char BPB_ExtFlags[2];   //FAT32����           offset:40
 unsigned char BPB_FSVer[2];      //FAT32����           offset:42
 unsigned char BPB_RootClus[4];   //��Ŀ¼�غ�          offset:44
 unsigned char FSInfo[2];         //��������FSINFO������offset:48
 unsigned char BPB_BkBootSec[2];  //ͨ��Ϊ6             offset:50
 unsigned char BPB_Reserved[12];  //��չ��              offset:52
 unsigned char BS_DrvNum[1];      //                    offset:64
 unsigned char BS_Reserved1[1];   //                    offset:65
 unsigned char BS_BootSig[1];     //                    offset:66
 unsigned char BS_VolID[4];       //                    offset:67
 unsigned char BS_FilSysType[11]; //	                offset:71
 unsigned char BS_FilSysType1[8]; //"FAT32    "         offset:82
};

// Structure of a dos directory entry.
struct direntry 
{
 unsigned char deName[8];       // filename, blank filled
 unsigned char deExtension[3]; 	// extension, blank filled
 unsigned char deAttributes;   	// file attributes
 unsigned char deLowerCase;    	// NT VFAT lower case flags  (set to zero)
 unsigned char deCHundredth;   	// hundredth of seconds in CTime
 unsigned char deCTime[2];     	// create time
 unsigned char deCDate[2];     	// create date
 unsigned char deADate[2];     	// access date
 unsigned char deHighClust[2];     // high unsigned chars of cluster number
 unsigned char deMTime[2];     	// last update time
 unsigned char deMDate[2];     	// last update date
 unsigned char deLowCluster[2]; 	// starting cluster of file
 unsigned char deFileSize[4];      // size of file in unsigned chars
};

// Stuctures
struct FileInfoStruct
{
 unsigned char  FileName[12];
 unsigned long  FileStartCluster;			//< file starting cluster for last file accessed
 unsigned long  FileCurCluster;
 unsigned long  FileNextCluster;
 unsigned long  FileSize;					//< file size for last file accessed
 unsigned char  FileAttr;					//< file attr for last file accessed
 unsigned short FileCreateTime;			//< file creation time for last file accessed
 unsigned short FileCreateDate;			//< file creation date for last file accessed
 unsigned short FileMTime;
 unsigned short FileMDate;
 unsigned long  FileSector;				    //<file record place
 unsigned int   FileOffset;				    //<file record offset
};

/*
  FAT32��ʼ��ʱ��ʼ����װ�����½ṹ����
*/
struct FAT32_Init_Arg
{
 unsigned char BPB_Sector_No;   //BPB����������
 unsigned long Total_Size;            //���̵�������
 unsigned long FirstDirClust;       //��Ŀ¼�Ŀ�ʼ��
 unsigned long FirstDataSector;	 //�ļ����ݿ�ʼ������
 unsigned int  BytesPerSector;	 //ÿ���������ֽ���
 unsigned int  FATsectors;            //FAT����ռ������
 unsigned int  SectorsPerClust;	 //ÿ�ص�������
 unsigned long FirstFATSector;	 //��һ��FAT����������
 unsigned long FirstDirSector;	 //��һ��Ŀ¼��������
 unsigned long RootDirSectors;	 //��Ŀ¼��ռ������
 unsigned long RootDirCount;	 //��Ŀ¼�µ�Ŀ¼���ļ���
};

//#define FIND_BPB_UP_RANGE 2000  //BPB��һ����0��������0��FINE_BPB_UP_RANGE��������ɨ��


unsigned char xdata FAT32_Buffer[512]; //�������ݶ�д������
struct FAT32_Init_Arg Init_Arg;	       //��ʼ�������ṹ��ʵ��
struct FileInfoStruct FileInfo;        //�ļ���Ϣ�ṹ��ʵ��

unsigned char * FAT32_ReadSector(unsigned long LBA,unsigned char *buf) //FAT32�ж�ȡ�����ĺ���
{
 MMC_get_data_LBA(LBA,512,buf);
 return buf;
}

unsigned char FAT32_WriteSector(unsigned long LBA,unsigned char *buf)//FAT32��д�����ĺ���
{
 return MMC_write_sector(LBA,buf);
}

unsigned long lb2bb(unsigned char *dat,unsigned char len) //С��תΪ���
{
 unsigned long temp=0;
 unsigned long fact=1;
 unsigned char i=0;
 for(i=0;i<len;i++)
 {
  temp+=dat[i]*fact;
  fact*=256;
 }
 return temp;
}

unsigned long  FAT32_FindBPB()  //Ѱ��BPB���ڵ�������
{
 /*
 unsigned long Range=0;
 while(Range<FIND_BPB_UP_RANGE)
 { 
  FAT32_ReadSector(Range,FAT32_Buffer);
  if(FAT32_Buffer[0]==0xeb && FAT32_Buffer[2]==0x90)
   break;
  Range++;
 }
 return Range;*/
 FAT32_ReadSector(0,FAT32_Buffer);
 return lb2bb(((((struct PartSector *)(FAT32_Buffer))->Part[0]).StartLBA),4);
}

unsigned long FAT32_Get_Total_Size() //�洢��������������λΪM
{
 FAT32_ReadSector(Init_Arg.BPB_Sector_No,FAT32_Buffer);
 return ((float)(lb2bb((((struct FAT32_BPB *)(FAT32_Buffer))->BPB_TotSec32),4)))*0.0004883;
}

void FAT32_Init(struct FAT32_Init_Arg *arg)
{
 struct FAT32_BPB *bpb=(struct FAT32_BPB *)(FAT32_Buffer);             //�����ݻ�����ָ��תΪstruct FAT32_BPB ��ָ��
 arg->BPB_Sector_No   =FAT32_FindBPB();                                             //FAT32_FindBPB()���Է���BPB���ڵ�������
 arg->Total_Size      =FAT32_Get_Total_Size();                                        //FAT32_Get_Total_Size()���Է��ش��̵�����������λ����
 arg->FATsectors      =lb2bb((bpb->BPB_FATSz32)    ,4);                       //װ��FAT��ռ�õ���������FATsectors��
 arg->FirstDirClust   =lb2bb((bpb->BPB_RootClus)   ,4);                        //װ���Ŀ¼�غŵ�FirstDirClust��
 arg->BytesPerSector  =lb2bb((bpb->BPB_BytesPerSec),2);                   //װ��ÿ�����ֽ�����BytesPerSector��
 arg->SectorsPerClust =lb2bb((bpb->BPB_SecPerClus) ,1);                   //װ��ÿ����������SectorsPerClust ��
 arg->FirstFATSector  =lb2bb((bpb->BPB_RsvdSecCnt) ,2)+arg->BPB_Sector_No;//װ���һ��FAT�������ŵ�FirstFATSector ��
 arg->RootDirCount    =lb2bb((bpb->BPB_RootEntCnt) ,2);                   //װ���Ŀ¼������RootDirCount��
 arg->RootDirSectors  =(arg->RootDirCount)*32>>9;                           //װ���Ŀ¼ռ�õ���������RootDirSectors��
 arg->FirstDirSector  =(arg->FirstFATSector)+(bpb->BPB_NumFATs[0])*(arg->FATsectors); //װ���һ��Ŀ¼������FirstDirSector��
 arg->FirstDataSector =(arg->FirstDirSector)+(arg->RootDirSectors); //װ���һ������������FirstDataSector��
}

void FAT32_EnterRootDir()
{
 unsigned long iRootDirSector;
 unsigned long iDir;
 struct direntry *pDir;
 for(iRootDirSector=(Init_Arg.FirstDirSector);iRootDirSector<(Init_Arg.FirstDirSector)+(Init_Arg.SectorsPerClust);iRootDirSector++)
 {
  FAT32_ReadSector(iRootDirSector,FAT32_Buffer);
  for(iDir=0;iDir<Init_Arg.BytesPerSector;iDir+=sizeof(struct direntry))
  {
   pDir=((struct direntry *)(FAT32_Buffer+iDir));
   if((pDir->deName)[0]!=0x00 /*��ЧĿ¼��*/ && (pDir->deName)[0]!=0xe5 /*��ЧĿ¼��*/ && (pDir->deName)[0]!=0x0f /*��Ч����*/)
   {
    SerialPutString(pDir->deName);
   }
  }
 }
}

void FAT32_CopyName(unsigned char *Dname,unsigned char *filename)
{
 unsigned char i=0;
 for(;i<11;i++)
 {
  Dname[i]=filename[i];
 }
 Dname[i]=0;
}

unsigned long FAT32_EnterDir(char *path)
{
 unsigned long iDirSector;
 unsigned long iCurSector=Init_Arg.FirstDirSector;
 unsigned long iDir;
 struct direntry *pDir;
// unsigned char DirName[12];
 unsigned char depth=0,i=0;
 while(path[i]!=0)
 {
  if(path[i]=='\\')
  { 
   depth++;
  }
  i++;
 }
 if(depth==1)
 {
  return iCurSector;    //����Ǹ�Ŀ¼��ֱ�ӷ��ص�ǰ������
 }
 for(iDirSector=iCurSector;iDirSector<(Init_Arg.FirstDirSector)+(Init_Arg.SectorsPerClust);iDirSector++)
 {
  FAT32_ReadSector(iDirSector,FAT32_Buffer);
  for(iDir=0;iDir<Init_Arg.BytesPerSector;iDir+=sizeof(struct direntry))
  {
   pDir=((struct direntry *)(FAT32_Buffer+iDir));
   if((pDir->deName)[0]!=0x00 /*��ЧĿ¼��*/ && (pDir->deName)[0]!=0xe5 /*��ЧĿ¼��*/ && (pDir->deName)[0]!=0x0f /*��Ч����*/)
   {
    SerialPutString(pDir->deName);
   }
  }
 }
}

unsigned char FAT32_CompareName(unsigned char *sname,unsigned char *dname)
{
 unsigned char i,j=8;
 unsigned char name_temp[12];
 for(i=0;i<11;i++) name_temp[i]=0x20;
 name_temp[11]=0;
 i=0;
 while(sname[i]!='.')
 {
  name_temp[i]=sname[i];
  i++;
 }
 i++;
 while(sname[i]!=0)
 {
  name_temp[j++]=sname[i];
  i++;
 }
 //Printf(name_temp,0);
 for(i=0;i<11;i++)
 {
  if(name_temp[i]!=dname[i]) return 0;
 }
  //Printf(name_temp,0);
 return 1;
}

unsigned long FAT32_GetNextCluster(unsigned long LastCluster)
{
 unsigned long temp;
 struct FAT32_FAT *pFAT;
 struct FAT32_FAT_Item *pFAT_Item;
 temp=((LastCluster/128)+Init_Arg.FirstFATSector);
 FAT32_ReadSector(temp,FAT32_Buffer);
 pFAT=(struct FAT32_FAT *)FAT32_Buffer;
 pFAT_Item=&((pFAT->Items)[LastCluster%128]);
 return lb2bb(pFAT_Item,4);
}

struct FileInfoStruct * FAT32_OpenFile(char *filepath)
{
 unsigned char depth=0;
 unsigned char i,index;
 unsigned long iFileSec,iCurFileSec,iFile;
 struct direntry *pFile;
 unsigned char len=strlen(filepath);
 for(i=0;i<len;i++)
 {
  if(filepath[i]=='\\')
  { 
   depth++;
   index=i+1;
  }
 }
 iCurFileSec=FAT32_EnterDir(filepath)/*Init_Arg.FirstDirSector*/;
 Printf("iCurFileSec",iCurFileSec);
 for(iFileSec=iCurFileSec;iFileSec<iCurFileSec+(Init_Arg.SectorsPerClust);iFileSec++)
 {
  FAT32_ReadSector(iFileSec,FAT32_Buffer);
  for(iFile=0;iFile<Init_Arg.BytesPerSector;iFile+=sizeof(struct direntry))
  {
   pFile=((struct direntry *)(FAT32_Buffer+iFile));
   if(FAT32_CompareName(filepath+index,pFile->deName))
   {
    //PutHex(pFile->deFileSize[0]);
	//PutHex(pFile->deFileSize[1]);
	//PutHex(pFile->deFileSize[2]);
	//PutHex(pFile->deFileSize[3]);
    FileInfo.FileSize=lb2bb(pFile->deFileSize,4);
	strcpy(FileInfo.FileName,filepath+index);
	FileInfo.FileStartCluster=lb2bb(pFile->deLowCluster,2)+lb2bb(pFile->deHighClust,2)*65536;
	FileInfo.FileCurCluster=FileInfo.FileStartCluster;
	FileInfo.FileNextCluster=FAT32_GetNextCluster(FileInfo.FileCurCluster);
	FileInfo.FileOffset=0;
	Printf("FileStartCluster",FileInfo.FileStartCluster);
	return &FileInfo;
   }
  }
 }
}



void FAT32_ReadFile(struct FileInfoStruct *pstru)
{
 unsigned long Sub=pstru->FileSize-pstru->FileOffset;
 unsigned long iSectorInCluster=0;
 unsigned long i=0;
 while(pstru->FileNextCluster!=0x0fffffff)  //���FAT�еĴ���Ϊ0x0fffffff��˵���޺�̴�
 {
  for(iSectorInCluster=0;iSectorInCluster<Init_Arg.SectorsPerClust;iSectorInCluster++)   //������������
  {
   FAT32_ReadSector((((pstru->FileCurCluster)-2)*(Init_Arg.SectorsPerClust))+Init_Arg.FirstDataSector+(iSectorInCluster),FAT32_Buffer);
   pstru->FileOffset+=Init_Arg.BytesPerSector;
   Sub=pstru->FileSize-pstru->FileOffset;
   for(i=0;i<Init_Arg.BytesPerSector;i++)
   {
    send(FAT32_Buffer[i]);   //�����ݷ��͵��ն�����ʾ
   }
  }
  pstru->FileCurCluster=pstru->FileNextCluster;  
  pstru->FileNextCluster=FAT32_GetNextCluster(pstru->FileCurCluster);   //������FAT�����Ĵ���
 }
 iSectorInCluster=0;
 while(Sub>=Init_Arg.BytesPerSector)   //������һ�أ���������������
 {
  FAT32_ReadSector((((pstru->FileCurCluster)-2)*(Init_Arg.SectorsPerClust))+Init_Arg.FirstDataSector+(iSectorInCluster++),FAT32_Buffer);
  pstru->FileOffset+=Init_Arg.BytesPerSector;
  Sub=pstru->FileSize-pstru->FileOffset;
  for(i=0;i<Init_Arg.BytesPerSector;i++)
  {
   send(FAT32_Buffer[i]);
  }
 }
 FAT32_ReadSector((((pstru->FileCurCluster)-2)*(Init_Arg.SectorsPerClust))+Init_Arg.FirstDataSector+(iSectorInCluster),FAT32_Buffer); //��ȡ���һ������
 for(i=0;i<Sub;i++)    //SubΪ���ʣ����ֽ���
 {
  send(FAT32_Buffer[i]);
 }	
}











