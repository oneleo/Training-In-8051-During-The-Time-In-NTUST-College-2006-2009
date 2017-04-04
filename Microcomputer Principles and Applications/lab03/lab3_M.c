#include "reg51.h"
//num_0to9999�Ψ��x�s�p�ƼƭȡAa�O���קK�p�ƻP����ۤ��v�T�B�n���x�s��J���T�줸�����
//aa�O�x�s��J�C���줸����ơAx�O�O���ثe�O����έp�ƪ��A�Ade�O�t�ױ���C
unsigned short num_0to9999=0,a,aa,x,de=0;
unsigned short b=1;
//seg�Ψ��x�s�U�ӤC�q��ܾ����ƭȡAscan�O���y���w�ΡAscan_num�O���wscan���B�ӫG�A��seg���Ʀr�~�G
unsigned short seg[4]={0},scan=0,scan_num=0;
//��ܭ��ӤC�q��ܾ����
unsigned short seg_scan[4]={0x0e,0x0d,0x0b,0x07};
//�C�q��ܾ�����ܤ覡
unsigned short seg_num[16]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};

//�ŧi������
void delay(int del)
{
  long d;
  for(d=0;d<del;d++)
  {
    ; 
  } 
}

//���_�{�ǡA�Ҧ���1
void T0_int(void)interrupt 1
{  
//�]�w�U�����_����ɶ��A�ɶ��@��A�A�׶i�J�����_�{�ǡA�p���~�i���_���C�q��ܭp�ưʧ@�C
  TH0=(65536-50000)/256;
  TL0=(65536-50000)%256;
//�C�����_���Nde+b
  de=de+b;
//�j��  ��de=10�ɶ}�l�H�U�ʧ@�C
  while(de==10)
  {
//���mde�C
    de=0;
//�w�]x���A��0�C
    x=0;
//�NP3&(0xe0)�üg�Ja�ܼ�  (�ӧP�O��3�줸��) �C
    a=P3&(0xe0);
//�Yp36��1�C
    if(a==0x20)
    {
//�T�w�ƭ�0000
      num_0to9999=0000;
//�]�w���A��x=1,�b�����A�U���|�i�J�p�ƪ������C 
      x=1;
    }
    else if(a==0x40)             
    {
  	/*�T�w�ƭ�*/
      num_0to9999=num_0to9999;
//�]�w���A��x=1,�b�����A�U���|�i�J�p�ƪ������C
      x=1;
    }
//�YP38��1
    else if(a==0x80)
    {
//�O�ܼ�b=1,��de���C�W�[��10   (�p�Ƴt�׸��C)
      b=1;
    }	
//�YP38��0
    else if(a==0x00)
    {
//�O�ܼ�b=2,��de���ּW�[��10   (�p�Ƴt�׸��C)
      b=2;
    }
//�NP3&(0x1f)�üg�Jaa�ܼ�  (�ӧP�O�C5�줸��) 
    aa=P3&0x1f;
//�Yx�����A0
    if(x==0)
    {
      switch(aa)		
      {
//��aa=0x02,���Ʃ��W�p��
        case 0x02:
        {
//��p�Ƥ��e���_��,�h�N�ƭ�+1�Ϧ�������
          if(num_0to9999%2==1)
          {
            num_0to9999=num_0to9999+1;
          }
//��p�Ƥ��e������,�h�N�ƭ�+2
          else if(num_0to9999%2==0)
          {
            num_0to9999=num_0to9999+2;
          }
//��p�Ƥ��e��>9998,�h�N�ƭ��k0
          if(num_0to9999>9998)
          {
            num_0to9999=0;
          }
//����
          break;
        }
//���Ʃ��U�p��
        case 0x04:
        {
//��p�Ƥ��e��<=9998,�h�N�ƭȳ]��10000
          if(num_0to9999<=0)
          {
            num_0to9999=10000;
          }
//��p�Ƥ��e���_��,�h�N�ƭ�-1�Ϧ�������
          if(num_0to9999%2==1)
          {
            num_0to9999=num_0to9999-1;
          }
//��p�Ƥ��e������,�h�N�ƭ�-2
          else if(num_0to9999%2==0)
          {
            num_0to9999=num_0to9999-2;
          }
          break;
        }
//�_�Ʃ��W�p��
        case 0x08:
        {
//��p�Ƥ��e������,�h�N�ƭ�+1,�ϼƭȬ��_��
          if(num_0to9999%2==0)
          {
            num_0to9999=num_0to9999+1;
          }
//��p�Ƥ��e���_��,�h�N�ƭ�+2
          else if(num_0to9999%2==1)
          {
            num_0to9999=num_0to9999+2;
          }
//��p�Ƥ��e��>9999,�h�N�ƭȳ]��1
          if(num_0to9999>9999)
          {
            num_0to9999=1;
          }
//�ư���L���p
          break;
        }
//�_�Ʃ��U�p��
        case 0x10:
        {
//��p�Ƥ��e��<=1,�h�N�ƭȳ]��10001
          if(num_0to9999<=1)
          {
            num_0to9999=10001;
          }
//��p�Ƥ��e������,�h�N�ƭ�-1,�ϼƭȬ��_��
          if(num_0to9999%2==0)
          {
            num_0to9999=num_0to9999-1;
          }
//��p�Ƥ��e���_��,�h�N�ƭ�-2
          else if(num_0to9999%2==1)
          {
            num_0to9999=num_0to9999-2;
          }
          break;
        }
//���W�p��
        case 0x00:
        {
//�ƭ�+1
          num_0to9999++;
//��p�Ƥ��e��>9999,�h�N�ƭȳ]��0
          if(num_0to9999>9999)
          {
            num_0to9999=0;
          }
//�ư���L���p
          break;
        }
//���U�p��
        case 0x01:
        {
//��p�Ƥ��e��<0,�h�N�ƭȳ]��10000
          if(num_0to9999<=0)
          {
            num_0to9999=10000;		
          }
//�ƭ�+1
          num_0to9999--;
//�ư���L���p
          break;
        }
//�_�h�����{��
        default:
        {
          num_0to9999=num_0to9999; 
        }
      }
    }
  }
}

int main()
{
//�]�w�p�ɤ��_
  IE=0x82;
//�]�w���_�Ҧ�
  TMOD=0x01;
//�]�w���_����ɶ��A�]�w������A�Y�q���ɲִ�A���0�ɶi�J���_�{��
  TH0=(65536-50000)/256;
//�]�w���_����ɶ��A�����G�]65536��x�^�At��x(us)
  TL0=(65536-50000)%256;
//�Ұʭp�ɾ��A��AT89S51�p�ɾ��Ұ�
  TR0=1;
//���_�Ҧ�1
  while(1)
  {
// ����
    delay(64);
//�N�Ӧ�Ʀs�Jseg[0]
    seg[0]=num_0to9999%10;
//�N�Ӧ�Ʀs�Jseg[1]
    seg[1]=(num_0to9999/10)%10;
//�N�Ӧ�Ʀs�Jseg[2]
    seg[2]=(num_0to9999/100)%10;
//�N�Ӧ�Ʀs�Jseg[3]
    seg[3]=(num_0to9999/1000);
//p2�@�����y�u
    P2=seg_scan[scan];
//p1�@����ƽu
    P1=seg_num[seg[scan]];
    scan_num=3;                         
//scan+1
    scan++;
//�Yscan>scan_num,�Oscan=0  ��scan��0 ~ 3  �i�H���seg_scan[scan]�@�����y�u����X
    if(scan>scan_num)
    { 
    scan=0;
    }
  }
}