#include "reg51.h"
unsigned short num_0to9999=0,a,aa,x,de=0;
unsigned short b=1;
unsigned short seg[4]={0},scan=0,scan_num=0;/*segノㄓ纗琿陪ボ竟计scan琌苯磞﹚ノscan_num琌﹚scan紹獹讽segΤ计獹*/
unsigned short seg_scan[4]={0x0e,0x0d,0x0b,0x07};/*匡拒琿陪ボ竟陪ボ*/
unsigned short seg_num[16]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};/*琿陪ボ竟陪ボよΑ*/

void delay(int del)           	//┑筐ㄧ计
{
  long d;
  for(d=0;d<del;d++)
  {
    ; 
  } 
}

void T0_int(void)interrupt 1    	/*い耞祘家Α1*/
{  
  TH0=(65536-50000)/256;       		/*砞﹚Ωい耞磅︽丁丁秈い耞祘*/
  TL0=(65536-50000)%256;        	/*ぃ耞暗琿陪ボ璸计笆*/
  de=de+b;                      	//–Ωい耞常盢de+b   (de+1)
  while(de==10)                 	//癹伴  讽DE=10秨﹍笆
  {
    de=0;                       	//DE=10 磷炒礚伴
    x=0;				//箇砞x篈0
    a=P3&(0xe0);                	//盢P3&(0xe0)糶a跑计  (ㄓ蔼3じノ) 
    if(a==0x20)                 	//璝p361
    {
      num_0to9999=0000;         	/*㏕﹚计0000*/
      x=1;                      	//砞﹚篈x=1,篈ぃ穦秈璸计场だ 
    }
    else if(a==0x40)             
    {
      num_0to9999=num_0to9999;  	/*㏕﹚计*/
      x=1;				//砞﹚篈x=1,篈ぃ穦秈璸计场だ 
    }
    else if(a==0x80)           		//璝P381
    {
      b=1;				//跑计b=1,ㄏde耕篊糤10   (璸计硉耕篊)
    }	
    else if(a==0x00)			//璝P380
    {
      b=2;				//跑计b=2,ㄏde耕е糤10   (璸计硉耕篊)
    }

    aa=P3&0x1f;                 	//盢P3&(0x1f)糶aa跑计  (ㄓ5じノ) 

    if(x==0)                    	//璝x篈0
    {
      switch(aa)		
      {
        case 0x02:              	/*讽aa=0x02,案计┕璸计*/
        {
          if(num_0to9999%2==1)          //讽璸计ず甧计,玥盢计+1ㄏΘ案计
          {
            num_0to9999=num_0to9999+1;
          }
          else if(num_0to9999%2==0)     //讽璸计ず甧案计,玥盢计+2
          {
            num_0to9999=num_0to9999+2;
          }
          if(num_0to9999>9998)          //讽璸计ず甧>9998,玥盢计耴0
          {
            num_0to9999=0;
          }
          break;                        //埃岿
        }
        case 0x04:           		/*案计┕璸计*/
        {
          if(num_0to9999<=0)		//讽璸计ず甧<=9998,玥盢计砞10000
          {
            num_0to9999=10000;
          }
          if(num_0to9999%2==1)		//讽璸计ず甧计,玥盢计-1ㄏΘ案计
          {
            num_0to9999=num_0to9999-1;
          }
          else if(num_0to9999%2==0)     //讽璸计ず甧案计,玥盢计-2
          {
            num_0to9999=num_0to9999-2;
          }
          break;
        }
        case 0x08	:            	/*计┕璸计*/
        {
          if(num_0to9999%2==0)          //讽璸计ず甧案计,玥盢计+1,ㄏ计计
          {
            num_0to9999=num_0to9999+1;
          }
          else if(num_0to9999%2==1)	//讽璸计ず甧计,玥盢计+2
          {
            num_0to9999=num_0to9999+2;
          }
          if(num_0to9999>9999)		//讽璸计ず甧>9999,玥盢计砞1
          {
            num_0to9999=1;
          }
          break;                        //逼埃ㄤ猵
        }
        case 0x10:            		/*计┕璸计*/
        {
          if(num_0to9999<=1)		//讽璸计ず甧<=1,玥盢计砞10001
          {
            num_0to9999=10001;
          }
          if(num_0to9999%2==0)		//讽璸计ず甧案计,玥盢计-1,ㄏ计计
          {
            num_0to9999=num_0to9999-1;
          }
          else if(num_0to9999%2==1)	//讽璸计ず甧计,玥盢计-2
          {
            num_0to9999=num_0to9999-2;
          }
          break;
        }
        case 0x00:             		/*┕璸计*/
        {
          num_0to9999++;                //计+1
          if(num_0to9999>9999)          //讽璸计ず甧>9999,玥盢计砞0
          {
            num_0to9999=0;
          }
          break;			//逼埃ㄤ猵
        }
        case 0x01:            		/*┕璸计*/
        {
          if(num_0to9999<=0)		//讽璸计ず甧<0,玥盢计砞10000
          {
            num_0to9999=10000;		
          }
          num_0to9999--;                //计+1
          break;			//逼埃ㄤ猵
        }
        default:             		/*玥蝴瞷*/
        {
          num_0to9999=num_0to9999; 
        }
      }
    }
  }
}
int main()
{
  IE=0x82;                		/*砞﹚璸い耞*/
  TMOD=0x01;              		/*砞﹚い耞家Α*/
  TH0=(65536-50000)/256;  		/*砞﹚い耞磅︽丁砞﹚ЧΘ眖仓搭搭0秈い耞祘*/
  TL0=(65536-50000)%256;  		/*砞﹚い耞磅︽丁そΑ65536⌒xt∽x(us)*/
  TR0=1;                  		/*币笆璸竟琵AT89S51璸竟币笆*/
  while(1)		  		//い耞家Α1
  {
    delay(64);  	  		// ┑筐

    seg[0]=num_0to9999%10;		//盢计seg[0]
    seg[1]=(num_0to9999/10)%10;		//盢计seg[1]
    seg[2]=(num_0to9999/100)%10;	//盢计seg[2]
    seg[3]=(num_0to9999/1000);		//盢计seg[3]
    P2=seg_scan[scan];			//p2苯磞絬
    P1=seg_num[seg[scan]];		//p1戈絬
    scan_num=3;                         
    scan++;                             //scan+1
    if(scan>scan_num)			//璝scan>scan_num,scan=0  ㄏscan0 ~ 3  ъseg_scan[scan]苯磞絬块

    { 
    scan=0;
    }
  }
}