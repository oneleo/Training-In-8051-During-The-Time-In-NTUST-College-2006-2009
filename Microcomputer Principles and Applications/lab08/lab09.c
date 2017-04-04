#include<reg51.h>               /*STARFINE DESIGN*/
sbit led_dot=P1^7;
unsigned short act[4]={0xfe,0xfd,0xfb,0xf7};  /*四個七段顯示器與鍵盤掃描方式*/
unsigned short seg_num[17]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0xff};  /*七段顯示器顯示方式*/
unsigned short key,key15open=0,switch_sel=3,title_segreg,title_count=0;  /*key鍵入數值，key15open啟動時鐘設定，switch_sel設定模式選四段，int_delay中斷內延遲：設定時鐘計數時間，title_segreg閃爍時七段暫存，title_count閃爍時間控制*/
unsigned int int_delay=0;
unsigned short seg[4]={0},scan_num=0,scan=0;  /*seg用來儲存各個七段顯示器的數值，scan是掃描指定用，scan_num是指定scan哪畿個亮，當seg有數字才亮*/
void delay(int delay)           /*延遲函數*/
{
  int d;
  for(d=0;d<(delay*100);d++)
  {
    ;
  }
}
void scan_7seg(void)             /*掃描七段顯示器函數*/
{
  P2=act[scan];                  /*設定AT89S51的P0.0、P0.1、P0.2、P0.3控制要讓哪個七段顯示器發光，因為四個七段顯示器的abcdefgh接腳同為一端，所以要用另四條控制線決定當個別七段顯示器所屬的訊號過來時，要讓該七段顯示器發光。*/
  P1=seg_num[seg[scan]];         /*設定AT89S51的P1.0、P1.1、P1.2、P1.3、P1.4、P1.5、P1.6、P1.7分別接入七段顯示器的abcdefgh，因為四個七段顯示器的資料線均相同，所以僅七條就可完成，再籍由快速掃描，肉眼無法辨別的情況下，達成其目的。*/
  if(scan==2)                    /*讓第三個七段小數顯示，分隔時與分*/
  {
    led_dot=0;
  }
  if(seg[0]!=0)                /*若分針個位數不為聆，則僅顯示第一個七段顯示器*/
  {
    scan_num=0;
  }
  if(seg[1]!=0)                /*若分針十位數不為零，則顯示第一到第二個七段顯示器*/
  { 
    scan_num=1;
  }
  if(seg[2]!=0)                /*若時針個位數不為零，則顯示第一到第三個七段顯示器*/
  {
    scan_num=2;
  }
  if(seg[3]!=0)                /*若時針十位數不為零，則顯示第一到第四個七段顯示器*/
  {
    scan_num=3;
  }
  if((seg[3]==0)&&(seg[2]!=0)&&(seg[1]==0))  /*若時針個位數不為零、十位數為零，分針十位數為零，則謹顯示第一個與第三個七段顯示器*/
  {
    scan_num=2;
    scan++;
  }
  scan++;                       /*掃描累加*/
  if(scan>scan_num)             /*若沒達到以上要求就歸零*/
  { 
    scan=0;
  } 
}
void T0_int(void)interrupt 1      /*中斷程序，模式為1*/
{
//  TH0=(256-200)/256;           /*設定下次中斷執行時間，時間一到，再度進入此中斷程序*/
//  TL0=(256-200)%256;           /*如此才可不斷做七段累加動作。*/
  scan_7seg();                    /*七段顯示器掃描程序*/
  int_delay++;                   /*延遲計數*/
  while(int_delay==5000)          /*每次計數5000*中斷2000u秒*/
  {
    int_delay=0;                 /*重設延遲計數*/
    seg[0]++;                    /*分針個位數累加1。*/
    if(seg[0]>=10)               /*如果分針個位數為9+1的瞬間，讓數值歸0，讓十位數累加1，*/
    {
      seg[0]=0;
      seg[1]++;                  /*分針十位數累加1*/
      if(seg[1]>=6)              /*如果分針十位數為5+1的瞬間，讓數值歸0，讓時針累加1。*/
      {
        seg[1]=0;
        seg[2]++;                /*時針個位數累加1*/
        if(seg[2]>=10)           /*如果時針個位數為9+1的瞬間，讓數值歸0，讓十位數累加1。*/
        {
          seg[2]=0;
          seg[3]++;
          if(seg[3]>=6)          /*如果時針十位數為5+1的瞬間，讓數值歸0。*/
          { 
            seg[3]=0;
          }
        }
      }
    }
  }
}
int main(void)
{
  IE=0x82;                         /*設定計時中斷*/
  TMOD=0x02;                       /*設定中斷模式*/
  TH0=(256-200)/256;            /*設定中斷執行時間，設定完成後，即從此時累減，減至0時進入中斷程序*/
  TL0=(256-200)%256;            /*設定中斷執行時間，公式：（65536－x），t＝x(us)*/
  TR0=1;                           /*啟動計時器，讓AT89S51計時器啟動*/
  P1=0;
  seg[3]=1;
  seg[2]=9;
  seg[1]=4;
  seg[0]=9;
  while(1)
  {    if(((seg[1]*10+seg[0])>=11)&&((seg[3]*10+seg[2])>=20))
    {
      seg[3]=1;
      seg[2]=9;
      seg[1]=4;
      seg[0]=9;
    }
  }
}
