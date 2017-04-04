#include <AT89X51.H>
int delay(int a)
{
  int i;
  for(i=a;i>0;i--)
  {
    ;
  }
}
int main()
{
  int a,b,x=0;
  while(1)
  {
    a=P3;
    delay(10000);
    if(a==0x00)
      {
        b=0xff;
        x=0;
      }
    else if(a==0x01)
      {
        if(x==0)
        {
          x=1;
          b=0x7f;
        }
        else if(x==1)
        {
          if(b==0xfe)
          {
            x=0;
          }
          else
          {
            b=b/2+0x80;
          }
        }
      }
	else if(a==0x02)
      {
        if(x==0)
        {
          x=1;
          b=0xfe;
        }
        else if(x==1)
        {
          if((b&0xff)==0x7f)
          {
            x=0;
          }
          else
          {
            b=b*2+1;
          }
        }
      }	
    else if(a==0x03)
      {
        if(x==1)
        {
          b=0x00;
          x=0;
        }
        else if(x==0)
        {
          b=0xff;
          x=1;
        }        
      }
    P1=b;
  }
}
