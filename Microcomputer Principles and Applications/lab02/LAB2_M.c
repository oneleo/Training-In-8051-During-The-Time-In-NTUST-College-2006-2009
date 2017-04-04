#include <AT89X51.H>  
int delay(unsigned int a)  //�ŧi������
{
  unsigned int i;    //�ŧi�L����
  for(i=a;i>0;i--)   //�Q��FOR�^�鰵����  �ϥ��ܼ�i�����ɶ�    
  {
    ;
  }
}
void RotateRight(int count) //�\��1:�i�H����P1��X  ��w�]��00101011  �C500ms�k�ۤ@��bit  �C��count�k��8��bit 
{
  int i,b,x=0;              //�ŧii,b,�åB�]�wx��l��0
  for(i=count*8+1;i>0;i--)  //�NŪ��Ӫ�count�ഫ���k�۪�bit��=count*8  �ĥΰj�鱱���
  {
    delay(50000);             //�C��bit����500mS
    if(x==0)                  //�P�_�O�_�����Ax=0,�p�G�O�K��b(��X��p1)�אּ0x2b  (00101011)
    {
      x=1;
      b=0x2b;
    }
    else if(x==1)       //�P�_�O�_�����Ax=1           
    {
      if((b&0x01)==0)     //b��LSB�Y��0 �Nb�k��1��bit
      {
        b=b/2;            
      }
      else                //b��LSB�Y����0 �Nb�k�ۤ@��bit �åBMSB+1  (10000000=0x80)              
      {
        b=b/2+0x80;
      }
    }
    P1=b;//�Nb�����e�e��p1
  }
}
void Flash(int count)  //�\��2:  �i�Op1���LED�������椬�o�G,�åB�����Ƹ����G
{
  int i,a,x=0;   //�ŧii,a, �ñNx���A���w�]��0         
  for(i=count+1;i>0;i--) //�Hcount�����e  ����^�骺����
  {
    delay(50000);//����500ms
    if(x==0)  //�Yx���A��0, �Nx���A�אּ1 �åB�N0x55�g�J�ܼ�a(������o�G) 
    {
      x=1;  
      a=0x55;
    }
    else    //�Yx���A����0, �Nx���A�אּ0 �åB�N0xaa�g�J�ܼ�a(�������o�G) 
    {
      x=0;
      a=0xaa;
    }
    P1=a;   //�Na�g�Jp1
  }
}
void light1(void)   //�\��3: ��led�̷ӳW�h�o�G
{
  int i,a,b,x=0;  //�ŧi�ܼ�  �Nx���A�w�]��0
  b=P3;           //�Np3�g�J�ܼ�b
  while(b==0x04)  //��b��0x04�ɫK���H�U�^��
  {
  b=P3;      //�b�^���������Np3�A���g�i�ܼ�b,�~��T�O���|���J�L���j��(b�@�������b0x04)   
  
  delay(50000); //����
  
  if(x==0)//�Y�����A0  �Nx���A�]��1  a=10000001  
  {
    x=1;
    a=0x81;
  }
  else if(x==1)  //�Y�����A1  �Nx���A�]��2  a=01000010
  {
    x=2;
    a=0x42;
  }
  else if(x==2)  //�Y�����A2  �Nx���A�]��3  a=10000001
  {
    x=3;
    a=0x24;
  }
  else if(x==3)  //�Y�����A3  �Nx���A�]��4  a=00100100
  {
    x=4;
    a=0x18;
  }
  else if(x==4)  //�Y�����A4  �Nx���A�]��5  a=00011000
  {
    x=5;
    a=0x24;
  }
  else if(x==5)  //�Y�����A5  �Nx���A�]��0  a=00100100
  {
    x=0;
    a=0x42;
  }
  P1=~a;  //�Na�ϦV��X��p1
  }
}
void light2(void) //�\��: ��led�̷ӳW�h�o�G
{
  int a,b,x=0;  //�ŧi�ܼ�  �Nx���A�w�]��0
  b=P3;         //�Np3�g�J�ܼ�b
  while(b==0x08)  //��b��0x08�ɫK���H�U�^��
  {
    b=P3;       //�b�^���������Np3�A���g�i�ܼ�b,�~��T�O���|���J�L���j��(b�@�������b0x08)
    delay(50000); //����
    if(x==0)    //�H�U�j���p�\��3
    {
      x=1;
      a=0xc0;
    }
    else if(x==1)
    {
      x=2;
      a=0x50;
    }
    else if(x==2)
    {
      x=3;
      a=0x24;
    }
    else if(x==3)
    {
      x=4;
      a=0x11;
    }
    else if(x==4)
    {
      x=5;
      a=0x48;
    }
    else if(x==5)
    {
      x=6;
      a=0x14;
    }
    else if(x==6)
    {
      x=7;
      a=0x06;
    }
    else if(x==7)
    {
      x=0;
      a=0x01;
    }
    P1=~a;
  }
}
int main(void)  //�D�{��
{
unsigned char a,b; //�ŧi�L���Ʀr��
while(1)
{
    a=P3;   //�Np3�g�Ja

    b=(a/16)&0x0f;   //�N�k��4��bit��a�g�Jb

    a=a&0x0f;  //���C4�줸

    if(a==0x01)       //�Ya==0x01 �I�s�\��1
    {
      RotateRight(b); //�Nb�ȶǨ�count
    }
    else if(a==0x02)  //�Ya==0x02 �I�s�\��2
    {
      Flash(b);       //�Nb�ȶǨ�count
    }
    else if(a==0x04)  //�Ya==0x04 �I�s�\��3
    {
      light1();       
    }
    else if(a==0x08)  //�Ya==0x04 �I�s�\��4
    {
      light2();	      
    }
  }
}
