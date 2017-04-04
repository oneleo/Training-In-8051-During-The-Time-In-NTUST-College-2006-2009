#include "reg51.h"
//num_0to9999ノㄓ纗璸计计a琌磷璸计籔北が紇臫綛竛纗块蔼じ戈
//aa琌纗块きじ戈x琌癘魁ヘ玡琌北┪璸计篈de琌硉北
unsigned short num_0to9999=0,a,aa,x,de=0;
unsigned short b=1;
//segノㄓ纗琿陪ボ竟计scan琌苯磞﹚ノscan_num琌﹚scan紹獹讽segΤ计獹
unsigned short seg[4]={0},scan=0,scan_num=0;
//匡拒琿陪ボ竟陪ボ
unsigned short seg_scan[4]={0x0e,0x0d,0x0b,0x07};
//琿陪ボ竟陪ボよΑ
unsigned short seg_num[16]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};

//┑筐ㄧ计
void delay(int del)
{
  long d;
  for(d=0;d<del;d++)
  {
    ; 
  } 
}

//い耞祘家Α1
void T0_int(void)interrupt 1
{  
//砞﹚Ωい耞磅︽丁丁秈い耞祘ぃ耞暗琿陪ボ璸计笆
  TH0=(65536-50000)/256;
  TL0=(65536-50000)%256;
//–Ωい耞常盢de+b
  de=de+b;
//癹伴  讽de=10秨﹍笆
  while(de==10)
  {
//竚de
    de=0;
//箇砞x篈0
    x=0;
//盢P3&(0xe0)糶a跑计  (ㄓ蔼3じノ) 
    a=P3&(0xe0);
//璝p361
    if(a==0x20)
    {
//㏕﹚计0000
      num_0to9999=0000;
//砞﹚篈x=1,篈ぃ穦秈璸计场だ 
      x=1;
    }
    else if(a==0x40)             
    {
  	/*㏕﹚计*/
      num_0to9999=num_0to9999;
//砞﹚篈x=1,篈ぃ穦秈璸计场だ
      x=1;
    }
//璝P381
    else if(a==0x80)
    {
//跑计b=1,ㄏde耕篊糤10   (璸计硉耕篊)
      b=1;
    }	
//璝P380
    else if(a==0x00)
    {
//跑计b=2,ㄏde耕е糤10   (璸计硉耕篊)
      b=2;
    }
//盢P3&(0x1f)糶aa跑计  (ㄓ5じノ) 
    aa=P3&0x1f;
//璝x篈0
    if(x==0)
    {
      switch(aa)		
      {
//讽aa=0x02,案计┕璸计
        case 0x02:
        {
//讽璸计ず甧计,玥盢计+1ㄏΘ案计
          if(num_0to9999%2==1)
          {
            num_0to9999=num_0to9999+1;
          }
//讽璸计ず甧案计,玥盢计+2
          else if(num_0to9999%2==0)
          {
            num_0to9999=num_0to9999+2;
          }
//讽璸计ず甧>9998,玥盢计耴0
          if(num_0to9999>9998)
          {
            num_0to9999=0;
          }
//埃岿
          break;
        }
//案计┕璸计
        case 0x04:
        {
//讽璸计ず甧<=9998,玥盢计砞10000
          if(num_0to9999<=0)
          {
            num_0to9999=10000;
          }
//讽璸计ず甧计,玥盢计-1ㄏΘ案计
          if(num_0to9999%2==1)
          {
            num_0to9999=num_0to9999-1;
          }
//讽璸计ず甧案计,玥盢计-2
          else if(num_0to9999%2==0)
          {
            num_0to9999=num_0to9999-2;
          }
          break;
        }
//计┕璸计
        case 0x08:
        {
//讽璸计ず甧案计,玥盢计+1,ㄏ计计
          if(num_0to9999%2==0)
          {
            num_0to9999=num_0to9999+1;
          }
//讽璸计ず甧计,玥盢计+2
          else if(num_0to9999%2==1)
          {
            num_0to9999=num_0to9999+2;
          }
//讽璸计ず甧>9999,玥盢计砞1
          if(num_0to9999>9999)
          {
            num_0to9999=1;
          }
//逼埃ㄤ猵
          break;
        }
//计┕璸计
        case 0x10:
        {
//讽璸计ず甧<=1,玥盢计砞10001
          if(num_0to9999<=1)
          {
            num_0to9999=10001;
          }
//讽璸计ず甧案计,玥盢计-1,ㄏ计计
          if(num_0to9999%2==0)
          {
            num_0to9999=num_0to9999-1;
          }
//讽璸计ず甧计,玥盢计-2
          else if(num_0to9999%2==1)
          {
            num_0to9999=num_0to9999-2;
          }
          break;
        }
//┕璸计
        case 0x00:
        {
//计+1
          num_0to9999++;
//讽璸计ず甧>9999,玥盢计砞0
          if(num_0to9999>9999)
          {
            num_0to9999=0;
          }
//逼埃ㄤ猵
          break;
        }
//┕璸计
        case 0x01:
        {
//讽璸计ず甧<0,玥盢计砞10000
          if(num_0to9999<=0)
          {
            num_0to9999=10000;		
          }
//计+1
          num_0to9999--;
//逼埃ㄤ猵
          break;
        }
//玥蝴瞷
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
//砞﹚璸い耞
  IE=0x82;
//砞﹚い耞家Α
  TMOD=0x01;
//砞﹚い耞磅︽丁砞﹚ЧΘ眖仓搭搭0秈い耞祘
  TH0=(65536-50000)/256;
//砞﹚い耞磅︽丁そΑ65536⌒xt∽x(us)
  TL0=(65536-50000)%256;
//币笆璸竟琵AT89S51璸竟币笆
  TR0=1;
//い耞家Α1
  while(1)
  {
// ┑筐
    delay(64);
//盢计seg[0]
    seg[0]=num_0to9999%10;
//盢计seg[1]
    seg[1]=(num_0to9999/10)%10;
//盢计seg[2]
    seg[2]=(num_0to9999/100)%10;
//盢计seg[3]
    seg[3]=(num_0to9999/1000);
//p2苯磞絬
    P2=seg_scan[scan];
//p1戈絬
    P1=seg_num[seg[scan]];
    scan_num=3;                         
//scan+1
    scan++;
//璝scan>scan_num,scan=0  ㄏscan0 ~ 3  ъseg_scan[scan]苯磞絬块
    if(scan>scan_num)
    { 
    scan=0;
    }
  }
}