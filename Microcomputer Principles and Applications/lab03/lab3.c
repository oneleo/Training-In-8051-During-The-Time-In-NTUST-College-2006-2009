#include "reg51.h"
unsigned short num_0to9999=0,a,aa,x,de=0;
unsigned short b=1;
unsigned short seg[4]={0},scan=0,scan_num=0;/*seg�Ψ��x�s�U�ӤC�q��ܾ����ƭȡAscan�O���y���w�ΡAscan_num�O���wscan���B�ӫG�A��seg���Ʀr�~�G*/
unsigned short seg_scan[4]={0x0e,0x0d,0x0b,0x07};/*��ܭ��ӤC�q��ܾ����*/
unsigned short seg_num[16]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};/*�C�q��ܾ�����ܤ覡*/

void delay(int del)           	//�ŧi������
{
  long d;
  for(d=0;d<del;d++)
  {
    ; 
  } 
}

void T0_int(void)interrupt 1    	/*���_�{�ǡA�Ҧ���1*/
{  
  TH0=(65536-50000)/256;       		/*�]�w�U�����_����ɶ��A�ɶ��@��A�A�׶i�J�����_�{��*/
  TL0=(65536-50000)%256;        	/*�p���~�i���_���C�q��ܭp�ưʧ@�C*/
  de=de+b;                      	//�C�����_���Nde+b   (de+1)
  while(de==10)                 	//�j��  ��DE=10�ɶ}�l�H�U�ʧ@
  {
    de=0;                       	//�ODE=10 �קK���J�L���^��
    x=0;				//�w�]x���A��0
    a=P3&(0xe0);                	//�NP3&(0xe0)�üg�Ja�ܼ�  (�ӧP�O��3�줸��) 
    if(a==0x20)                 	//�Yp36��1
    {
      num_0to9999=0000;         	/*�T�w�ƭ�0000*/
      x=1;                      	//�]�w���A��x=1,�b�����A�U���|�i�J�p�ƪ����� 
    }
    else if(a==0x40)             
    {
      num_0to9999=num_0to9999;  	/*�T�w�ƭ�*/
      x=1;				//�]�w���A��x=1,�b�����A�U���|�i�J�p�ƪ����� 
    }
    else if(a==0x80)           		//�YP38��1
    {
      b=1;				//�O�ܼ�b=1,��de���C�W�[��10   (�p�Ƴt�׸��C)
    }	
    else if(a==0x00)			//�YP38��0
    {
      b=2;				//�O�ܼ�b=2,��de���ּW�[��10   (�p�Ƴt�׸��C)
    }

    aa=P3&0x1f;                 	//�NP3&(0x1f)�üg�Jaa�ܼ�  (�ӧP�O�C5�줸��) 

    if(x==0)                    	//�Yx�����A0
    {
      switch(aa)		
      {
        case 0x02:              	/*��aa=0x02,���Ʃ��W�p��*/
        {
          if(num_0to9999%2==1)          //��p�Ƥ��e���_��,�h�N�ƭ�+1�Ϧ�������
          {
            num_0to9999=num_0to9999+1;
          }
          else if(num_0to9999%2==0)     //��p�Ƥ��e������,�h�N�ƭ�+2
          {
            num_0to9999=num_0to9999+2;
          }
          if(num_0to9999>9998)          //��p�Ƥ��e��>9998,�h�N�ƭ��k0
          {
            num_0to9999=0;
          }
          break;                        //����
        }
        case 0x04:           		/*���Ʃ��U�p��*/
        {
          if(num_0to9999<=0)		//��p�Ƥ��e��<=9998,�h�N�ƭȳ]��10000
          {
            num_0to9999=10000;
          }
          if(num_0to9999%2==1)		//��p�Ƥ��e���_��,�h�N�ƭ�-1�Ϧ�������
          {
            num_0to9999=num_0to9999-1;
          }
          else if(num_0to9999%2==0)     //��p�Ƥ��e������,�h�N�ƭ�-2
          {
            num_0to9999=num_0to9999-2;
          }
          break;
        }
        case 0x08	:            	/*�_�Ʃ��W�p��*/
        {
          if(num_0to9999%2==0)          //��p�Ƥ��e������,�h�N�ƭ�+1,�ϼƭȬ��_��
          {
            num_0to9999=num_0to9999+1;
          }
          else if(num_0to9999%2==1)	//��p�Ƥ��e���_��,�h�N�ƭ�+2
          {
            num_0to9999=num_0to9999+2;
          }
          if(num_0to9999>9999)		//��p�Ƥ��e��>9999,�h�N�ƭȳ]��1
          {
            num_0to9999=1;
          }
          break;                        //�ư���L���p
        }
        case 0x10:            		/*�_�Ʃ��U�p��*/
        {
          if(num_0to9999<=1)		//��p�Ƥ��e��<=1,�h�N�ƭȳ]��10001
          {
            num_0to9999=10001;
          }
          if(num_0to9999%2==0)		//��p�Ƥ��e������,�h�N�ƭ�-1,�ϼƭȬ��_��
          {
            num_0to9999=num_0to9999-1;
          }
          else if(num_0to9999%2==1)	//��p�Ƥ��e���_��,�h�N�ƭ�-2
          {
            num_0to9999=num_0to9999-2;
          }
          break;
        }
        case 0x00:             		/*���W�p��*/
        {
          num_0to9999++;                //�ƭ�+1
          if(num_0to9999>9999)          //��p�Ƥ��e��>9999,�h�N�ƭȳ]��0
          {
            num_0to9999=0;
          }
          break;			//�ư���L���p
        }
        case 0x01:            		/*���U�p��*/
        {
          if(num_0to9999<=0)		//��p�Ƥ��e��<0,�h�N�ƭȳ]��10000
          {
            num_0to9999=10000;		
          }
          num_0to9999--;                //�ƭ�+1
          break;			//�ư���L���p
        }
        default:             		/*�_�h�����{��*/
        {
          num_0to9999=num_0to9999; 
        }
      }
    }
  }
}
int main()
{
  IE=0x82;                		/*�]�w�p�ɤ��_*/
  TMOD=0x01;              		/*�]�w���_�Ҧ�*/
  TH0=(65536-50000)/256;  		/*�]�w���_����ɶ��A�]�w������A�Y�q���ɲִ�A���0�ɶi�J���_�{��*/
  TL0=(65536-50000)%256;  		/*�]�w���_����ɶ��A�����G�]65536��x�^�At��x(us)*/
  TR0=1;                  		/*�Ұʭp�ɾ��A��AT89S51�p�ɾ��Ұ�*/
  while(1)		  		//���_�Ҧ�1
  {
    delay(64);  	  		// ����

    seg[0]=num_0to9999%10;		//�N�Ӧ�Ʀs�Jseg[0]
    seg[1]=(num_0to9999/10)%10;		//�N�Ӧ�Ʀs�Jseg[1]
    seg[2]=(num_0to9999/100)%10;	//�N�Ӧ�Ʀs�Jseg[2]
    seg[3]=(num_0to9999/1000);		//�N�Ӧ�Ʀs�Jseg[3]
    P2=seg_scan[scan];			//p2�@�����y�u
    P1=seg_num[seg[scan]];		//p1�@����ƽu
    scan_num=3;                         
    scan++;                             //scan+1
    if(scan>scan_num)			//�Yscan>scan_num,�Oscan=0  ��scan��0 ~ 3  �i�H���seg_scan[scan]�@�����y�u����X

    { 
    scan=0;
    }
  }
}