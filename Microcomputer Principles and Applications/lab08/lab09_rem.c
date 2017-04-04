//------------------------------

//�[�JAT89X51.H�ɡC
#include<reg51.h>
//��@�C�q��ܤp���I����C
sbit led_dot=P1^7;
//�|�ӤC�q��ܾ��P��L���y�覡�C
unsigned short act[4]={0xfe,0xfd,0xfb,0xf7};
//�C�q��ܾ���ܤ覡�C
unsigned short seg_num[17]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0xff};
//int_delay���_������G�]�w�����p�Ʈɶ��C
unsigned int int_delay=0;
//seg�Ψ��x�s�U�ӤC�q��ܾ����ƭȡC
unsigned short seg[4]={0};
//scan_num�O���wscan���B�ӫG�A��seg���Ʀr�~�G�C
unsigned short scan_num=0;
//scan�O���y���w�ΡC
unsigned short scan=0;

//------------------------------

//�ŧi�����ơC
void delay(int delay)
{
//�ŧi�����x�s�ܼ�d�C
  int d;
  for(d=0;d<(delay*100);d++)
  {
    ;
  }
}

//------------------------------

//���y�C�q��ܾ���ơC
void scan_7seg(void)
{
//�]�wAT89S51��P0.0�BP0.1�BP0.2�BP0.3����n�����ӤC�q��ܾ��o���A
//�]���|�ӤC�q��ܾ���abcdefgh���}�P���@�ݡA
//�ҥH�n�Υt�|������u�M�w��ӧO�C�q��ܾ����ݪ��T���L�ӮɡA
//�n���ӤC�q��ܾ��o���C
  P2=act[scan];
//�]�wAT89S51��P1.0�BP1.1�BP1.2�BP1.3�BP1.4�BP1.5�BP1.6�BP1.7���O���J�C�q��ܾ���abcdefgh�A
//�]���|�ӤC�q��ܾ�����ƽu���ۦP�A
//�ҥH�ȤC���N�i�����A
//�A�y�ѧֳt���y�A
//�ײ��L�k��O�����p�U�A
//�F����ت��C
  P1=seg_num[seg[scan]];
//���ĤT�ӤC�q�p����ܡC
  if(scan==2)
  {
//���j�ɻP���C
    led_dot=0;
  }
//�Y���w�Ӧ�Ƥ����s�A
  if(seg[0]!=0)
  {
//�h����ܲĤ@�ӤC�q��ܾ��C
    scan_num=0;
  }
//�Y���w�Q��Ƥ����s�A
  if(seg[1]!=0)
  {
//�h��ܲĤ@��ĤG�ӤC�q��ܾ��C
    scan_num=1;
  }
//�Y�ɰw�Ӧ�Ƥ����s�A
  if(seg[2]!=0)
  {
//�h��ܲĤ@��ĤT�ӤC�q��ܾ��C
    scan_num=2;
  }
//�Y�ɰw�Q��Ƥ����s�A
  if(seg[3]!=0)
  {
//�h��ܲĤ@��ĥ|�ӤC�q��ܾ��C
    scan_num=3;
  }
//�Y�ɰw�Ӧ�Ƥ����s�B�Q��Ƭ��s�A���w�Q��Ƭ��s�A
  if((seg[3]==0)&&(seg[2]!=0)&&(seg[1]==0))
  {
//�h����ܲĤ@�ӻP�ĤT�ӤC�q��ܾ��C
    scan_num=2;
//���y�֥[�A�t�X�U�������y�֥[�A�i���L��q���|��C�q��ܾ��C
    scan++;
  }
//���y�֥[�C
  scan++;
//�Y�S�F��H�W�n�D�N�k�s�C
  if(scan>scan_num)
  {
//���y�M���C
    scan=0;
  } 
}

//------------------------------

//�ŧi���_��ơC
void T0_int(void)interrupt 1
{
//�]�w�U�����_����ɶ��A�ɶ��@��A�A�׶i�J�����_�{�ǡC
  TH0=(256-200)/256;
//�p���~�i���_���C�q�֥[�ʧ@�C
  TL0=(256-200)%256;
//�C�q��ܾ����y�{�ǡC
  scan_7seg();
//����p�ơC
  int_delay++;
//�C���p��5000*���_2000u��C
  while(int_delay==5000)
  {
//���]����p�ơC
    int_delay=0;
//���w�Ӧ�Ʋ֥[1�C
    seg[0]++;
//�p�G���w�Ӧ�Ƭ�9+1�������A���ƭ��k0�A���Q��Ʋ֥[1�A
    if(seg[0]>=10)
    {
//�p�G���w�Ӧ�Ƭ�9+1�������C
      seg[0]=0;
//���w�Q��Ʋ֥[1�C
      seg[1]++;
//�p�G���w�Q��Ƭ�5+1�������A���ƭ��k0�A���ɰw�֥[1�C
      if(seg[1]>=6)
      {
//�p�G���w�Q��Ƭ�5+1�������C
        seg[1]=0;
//�ɰw�Ӧ�Ʋ֥[1�C
        seg[2]++;
//�p�G�ɰw�Ӧ�Ƭ�9+1�������A���ƭ��k0�A���Q��Ʋ֥[1�C
        if(seg[2]>=10)
        {
//�p�G�ɰw�Ӧ�Ƭ�9+1�������C
          seg[2]=0;
//�ɰw�Q��Ʋ֥[1�C
          seg[3]++;
//�p�G�ɰw�Q��Ƭ�5+1�������A���ƭ��k0�C
          if(seg[3]>=6)
          {
//�p�G�ɰw�Q��Ƭ�5+1�������C
            seg[3]=0;
          }
        }
      }
    }
  }
}

//------------------------------

//�D�{���C
int main(void)
{
//�]�w�p�ɤ��_�C
  IE=0x82;
//�]�w���_�Ҧ��C
  TMOD=0x02;
//�]�w���_����ɶ��A�]�w������A�Y�q���ɲִ�A���0�ɶi�J���_�{�ǡC
  TH0=(256-200)/256;
//�]�w���_����ɶ��A�����G�]256��x�^�At��x(us)�C
  TL0=(256-200)%256;
//�Ұʭp�ɾ��A��AT89S51�p�ɾ��ҰʡC
  TR0=1;
//�ɰw�A���w���j�p���I�C
  P1=0;
//�D�حn�D�ɶ��n�q19��49��}�l�p�ɡC
  seg[3]=1;
  seg[2]=9;
  seg[1]=4;
  seg[0]=9;
//���F���_����A�εL�a�j��]��C
  while(1)
  {
//�D�حn�D�A�Y�ɶ��F��20��10��ɡA�ɶ����s�^��19��49���l�C
    if(((seg[1]*10+seg[0])>=11)&&((seg[3]*10+seg[2])>=20))
    {
//�ɶ���l�ơC
      seg[3]=1;
      seg[2]=9;
      seg[1]=4;
      seg[0]=9;
    }
  }
}
