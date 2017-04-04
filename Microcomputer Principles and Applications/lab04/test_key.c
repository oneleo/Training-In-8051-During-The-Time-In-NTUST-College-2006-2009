#include <REGX51.H>
char act[4]={0xfe,0xfd,0xfb,0xf7};
char seg[16]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
char seg_num[4]={0},seg_scan_con=0;
void delay(int d)
{
  for(d=d*9216;d>0;d--)
  {
    ;
  }
}

char scan_key(void)
{
  char r,c,in;
  for(c=0;c<4;c++)
  {
    P2=act[c];
    delay(20);
    in=(P2>>4)|0xf0;
    for(r=0;r<4;r++)
    {
      if(in==act[r])
      {
        return r*4+c;
      }
    }
  }
  return -1;
}

char give_key(void)
{
  char key=scan_key();
  while((key==scan_key()))
  {
    ;
  }
  return key;
}

int main(void)
{
  char key;
  while(1)
  {
    key=give_key();
    if(key!=-1)
    {
      seg_num[3]=seg_num[2];
      seg_num[2]=seg_num[1];
      seg_num[1]=seg_num[0];
      seg_num[0]=key;
    }
      P1=act[seg_scan_con];
      P0=seg[seg_num[seg_scan_con]];
      seg_scan_con++;
      if(seg_scan_con>3)
      {
        seg_scan_con=0;
      }
  }
}
