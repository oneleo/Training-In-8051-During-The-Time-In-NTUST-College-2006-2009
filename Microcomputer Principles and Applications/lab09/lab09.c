#include <AT89X51.H>
unsigned char act[4]={0xfe,0xfd,0xfb,0xf7};
#define s 65536
#define t 12000000/12/2
#define Do s-t/523
#define Re s-t/587
#define Mi s-t/659
#define Fa s-t/698
#define So s-t/784
#define La s-t/880
#define Si s-t/998
#define Hi_Do s-t/1046
unsigned int temp,int_delay=0;
sbit laba=P3^7;
unsigned int tonetable[8]={Do,Re,Mi,Fa,So,La,Si,Hi_Do};
int akey=0;
void delay1ms(unsigned int count)
{
  unsigned int i,j;
  for(i=0;i<50;i++)
    for(j=0;j<count;j++);
}

char scan_key(void)
{
  char r,c,in;
  for(c=0;c<4;c++)
  {
    P1=act[c];
    delay1ms(1);
    in=(P1>>4)|0xf0;
    for(r=0;r<4;r++)
    {
      if(in==act[r])
      {
        return (3-r)*4+(3-c);
      }
    }
  }
  return -1;
}

char give_key(void)
{
  char key=scan_key();
  return key;
}

void T0_int(void)interrupt 1
{
  TH0=temp/256;
  TL0=temp%256;
  laba=~laba;
}

void T1_int(void)interrupt 3
{
  TH1=(65536-50000)/256;
  TL1=(65536-50000)%256;
  int_delay++;
  if(int_delay>=10)
  {
    int_delay=0;
    TR0=0;
    TR1=0;
  }
}

int main(void)
{
  IE=0x8a;
  TMOD=0x11;
  TR0=0;
  TR1=0;
  TH1=(65536-50000)/256;
  TL1=(65536-50000)%256;
  laba=1;
  while(1)
  {
    akey=give_key();
    if(akey!=-1)
    {
      TR0=1;
      temp=tonetable[akey];
      TR1=1;
    }
  }
}
