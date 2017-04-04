#include <reg51.h>
#define s 65536
#define t 12000000/12/2
#define Do s-t/523
#define Re s-t/587
#define Mi s-t/659
#define Fa s-t/698
#define So s-t/784
#define La s-t/880
#define Si s-t/998
#define DO s-t/1046
sbit p3_7=P3^7;
unsigned int temp;
const unsigned int tonetable[8]={Do,Re,Mi,Fa,So,La,Si,DO};
int x50ms=100,d;
/*void T1_int(void) interrupt 3
{
  x50ms--;
  if(x50ms<=0)
{
  x50ms=100;
  TH0=(s-50000)/256;
  TL0=(s-50000)%256;
  TR0=0;   
}
}*/
void delay(int del)
{
  int d,c;
  for(d=0;d<del;d++)
  {
    for(c=0;c<2 0;c++); 
  } 
}
main()
{
  char i=0;
  char toneno;
  unsigned char keystatus;
  TH0=(s-50000)/256;
  TL0=(s-50000)%256;
  IE=0x8A;
  TMOD=0x01;
  TR0=0;
  p3_7=1;
  while(1)
   {
    P1=0xff;
    keystatus=~P1;
    if(keystatus!=0)
    {
       toneno=0;
       for(i=0;i<8;i++)
        {
          if(keystatus==0x01)
          {
            i=8;
            temp=tonetable[toneno];
            TR0=1;       
  		   delay(400);
		    TR0=0;
          }     
          else
          {
           toneno++;
           keystatus>>=1;
          }
        }
    }
    else
      {
       
	  }
    }
}
void T0_int(void)interrupt 1
{
    TH0=temp/256;
    TL0=temp%256;
    p3_7=~p3_7;
}


