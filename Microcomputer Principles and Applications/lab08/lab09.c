#include<reg51.h>               /*STARFINE DESIGN*/
sbit led_dot=P1^7;
unsigned short act[4]={0xfe,0xfd,0xfb,0xf7};  /*�|�ӤC�q��ܾ��P��L���y�覡*/
unsigned short seg_num[17]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0xff};  /*�C�q��ܾ���ܤ覡*/
unsigned short key,key15open=0,switch_sel=3,title_segreg,title_count=0;  /*key��J�ƭȡAkey15open�Ұʮ����]�w�Aswitch_sel�]�w�Ҧ���|�q�Aint_delay���_������G�]�w�����p�Ʈɶ��Atitle_segreg�{�{�ɤC�q�Ȧs�Atitle_count�{�{�ɶ�����*/
unsigned int int_delay=0;
unsigned short seg[4]={0},scan_num=0,scan=0;  /*seg�Ψ��x�s�U�ӤC�q��ܾ����ƭȡAscan�O���y���w�ΡAscan_num�O���wscan���B�ӫG�A��seg���Ʀr�~�G*/
void delay(int delay)           /*������*/
{
  int d;
  for(d=0;d<(delay*100);d++)
  {
    ;
  }
}
void scan_7seg(void)             /*���y�C�q��ܾ����*/
{
  P2=act[scan];                  /*�]�wAT89S51��P0.0�BP0.1�BP0.2�BP0.3����n�����ӤC�q��ܾ��o���A�]���|�ӤC�q��ܾ���abcdefgh���}�P���@�ݡA�ҥH�n�Υt�|������u�M�w��ӧO�C�q��ܾ����ݪ��T���L�ӮɡA�n���ӤC�q��ܾ��o���C*/
  P1=seg_num[seg[scan]];         /*�]�wAT89S51��P1.0�BP1.1�BP1.2�BP1.3�BP1.4�BP1.5�BP1.6�BP1.7���O���J�C�q��ܾ���abcdefgh�A�]���|�ӤC�q��ܾ�����ƽu���ۦP�A�ҥH�ȤC���N�i�����A�A�y�ѧֳt���y�A�ײ��L�k��O�����p�U�A�F����ت��C*/
  if(scan==2)                    /*���ĤT�ӤC�q�p����ܡA���j�ɻP��*/
  {
    led_dot=0;
  }
  if(seg[0]!=0)                /*�Y���w�Ӧ�Ƥ�����A�h����ܲĤ@�ӤC�q��ܾ�*/
  {
    scan_num=0;
  }
  if(seg[1]!=0)                /*�Y���w�Q��Ƥ����s�A�h��ܲĤ@��ĤG�ӤC�q��ܾ�*/
  { 
    scan_num=1;
  }
  if(seg[2]!=0)                /*�Y�ɰw�Ӧ�Ƥ����s�A�h��ܲĤ@��ĤT�ӤC�q��ܾ�*/
  {
    scan_num=2;
  }
  if(seg[3]!=0)                /*�Y�ɰw�Q��Ƥ����s�A�h��ܲĤ@��ĥ|�ӤC�q��ܾ�*/
  {
    scan_num=3;
  }
  if((seg[3]==0)&&(seg[2]!=0)&&(seg[1]==0))  /*�Y�ɰw�Ӧ�Ƥ����s�B�Q��Ƭ��s�A���w�Q��Ƭ��s�A�h����ܲĤ@�ӻP�ĤT�ӤC�q��ܾ�*/
  {
    scan_num=2;
    scan++;
  }
  scan++;                       /*���y�֥[*/
  if(scan>scan_num)             /*�Y�S�F��H�W�n�D�N�k�s*/
  { 
    scan=0;
  } 
}
void T0_int(void)interrupt 1      /*���_�{�ǡA�Ҧ���1*/
{
//  TH0=(256-200)/256;           /*�]�w�U�����_����ɶ��A�ɶ��@��A�A�׶i�J�����_�{��*/
//  TL0=(256-200)%256;           /*�p���~�i���_���C�q�֥[�ʧ@�C*/
  scan_7seg();                    /*�C�q��ܾ����y�{��*/
  int_delay++;                   /*����p��*/
  while(int_delay==5000)          /*�C���p��5000*���_2000u��*/
  {
    int_delay=0;                 /*���]����p��*/
    seg[0]++;                    /*���w�Ӧ�Ʋ֥[1�C*/
    if(seg[0]>=10)               /*�p�G���w�Ӧ�Ƭ�9+1�������A���ƭ��k0�A���Q��Ʋ֥[1�A*/
    {
      seg[0]=0;
      seg[1]++;                  /*���w�Q��Ʋ֥[1*/
      if(seg[1]>=6)              /*�p�G���w�Q��Ƭ�5+1�������A���ƭ��k0�A���ɰw�֥[1�C*/
      {
        seg[1]=0;
        seg[2]++;                /*�ɰw�Ӧ�Ʋ֥[1*/
        if(seg[2]>=10)           /*�p�G�ɰw�Ӧ�Ƭ�9+1�������A���ƭ��k0�A���Q��Ʋ֥[1�C*/
        {
          seg[2]=0;
          seg[3]++;
          if(seg[3]>=6)          /*�p�G�ɰw�Q��Ƭ�5+1�������A���ƭ��k0�C*/
          { 
            seg[3]=0;
          }
        }
      }
    }
  }
}
int main(void)
{
  IE=0x82;                         /*�]�w�p�ɤ��_*/
  TMOD=0x02;                       /*�]�w���_�Ҧ�*/
  TH0=(256-200)/256;            /*�]�w���_����ɶ��A�]�w������A�Y�q���ɲִ�A���0�ɶi�J���_�{��*/
  TL0=(256-200)%256;            /*�]�w���_����ɶ��A�����G�]65536��x�^�At��x(us)*/
  TR0=1;                           /*�Ұʭp�ɾ��A��AT89S51�p�ɾ��Ұ�*/
  P1=0;
  seg[3]=1;
  seg[2]=9;
  seg[1]=4;
  seg[0]=9;
  while(1)
  {    if(((seg[1]*10+seg[0])>=11)&&((seg[3]*10+seg[2])>=20))
    {
      seg[3]=1;
      seg[2]=9;
      seg[1]=4;
      seg[0]=9;
    }
  }
}
