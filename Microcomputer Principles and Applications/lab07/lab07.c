#include <AT89X51.H>

unsigned short seg_num[17]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0xff};  /*七段顯示器顯示方式*/

void delay1ms(unsigned int count)
{
  unsigned int i,j;
  for(i=0;i<50;i++)
    for(j=0;j<count;j++);
}

void T0_int(void) interrupt 1
{
  int i;
  for(i=0;i<=4;i++)
  {
    P2=0xfc;
    P1=seg_num[i];
    delay1ms(1000);
  }
}

void T1_int(void) interrupt 3
{
  int i;
  for(i=5;i<=9;i++)
  {
    P2=0xfc;
    P1=seg_num[i];
    delay1ms(1000);
  }
}
int main()
{
  int i;
  IE=0x85;
  IT0=1;
  IT1=1;
  TMOD=0x11;
  IP=0x04;
  while(1)
  {
    for(i=0;i<=9;i++)
    {
      P2=0xfc;
      P1=seg_num[i];
      delay1ms(1000);
    }
  }
}
