#include <AT89X51.H>
int delay(unsigned int a)
{
  unsigned int i;
  for(i=a;i>0;i--)
  {
    ;
  }
}
void RotateRight(int count)
{
  int i,b,x=0;
  for(i=count*8+1;i>0;i--)
  {
    delay(50000);
    if(x==0)
    {
      x=1;
      b=0x2b;
    }
    else if(x==1)
    {
      if((b&0x01)==0)
      {
        b=b/2;
      }
      else
      {
        b=b/2+0x80;
      }
    }
    P1=b;
  }
}
void Flash(int count)
{
  int i,a,x=0;
  for(i=count+1;i>0;i--)
  {
    delay(50000);
    if(x==0)
    {
      x=1;
      a=0x55;
    }
    else
    {
      x=0;
      a=0xaa;
    }
    P1=a;
  }
}
void light1(void)
{
  int i,a,b,x=0;
  b=P3;
  while(b==0x04)
  {
  b=P3;
  delay(50000);
  if(x==0)
  {
    x=1;
    a=0x81;
  }
  else if(x==1)
  {
    x=2;
    a=0x42;
  }
  else if(x==2)
  {
    x=3;
    a=0x24;
  }
  else if(x==3)
  {
    x=4;
    a=0x18;
  }
  else if(x==4)
  {
    x=5;
    a=0x24;
  }
  else if(x==5)
  {
    x=0;
    a=0x42;
  }
  P1=~a;
  }
}
void light2(void)
{
  int a,b,x=0;
  b=P3;
  while(b==0x08)
  {
    b=P3;
    delay(50000);
    if(x==0)
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
int main(void)
{
unsigned char a,b;
while(1)
{
    a=P3;
    b=(a/16)&0x0f;
    a=a&0x0f;
    if(a==0x01)
    {
      RotateRight(b);
    }
    else if(a==0x02)
    {
      Flash(b);
    }
    else if(a==0x04)
    {
      light1();
    }
    else if(a==0x08)
    {
      light2();
    }
  }
}
