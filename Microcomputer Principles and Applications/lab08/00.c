#include<reg51.h>               /*STARFINE DESIGN*/
sbit led_dot=P1^7;
unsigned short act[4]={0xfe,0xfd,0xfb,0xf7};  /*四個七段顯示器與鍵盤掃描方式*/
unsigned short seg_num[17]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0xff};  /*七段顯示器顯示方式*/
unsigned short key,key15open=0,switch_sel=3,int_delay=0,title_segreg,title_count=0;  /*key鍵入數值，key15open啟動時鐘設定，switch_sel設定模式選四段，int_delay中斷內延遲：設定時鐘計數時間，title_segreg閃爍時七段暫存，title_count閃爍時間控制*/
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
  P0=act[scan];                  /*設定AT89S51的P0.0、P0.1、P0.2、P0.3控制要讓哪個七段顯示器發光，因為四個七段顯示器的abcdefgh接腳同為一端，所以要用另四條控制線決定當個別七段顯示器所屬的訊號過來時，要讓該七段顯示器發光。*/
  P1=seg_num[seg[scan]];         /*設定AT89S51的P1.0、P1.1、P1.2、P1.3、P1.4、P1.5、P1.6、P1.7分別接入七段顯示器的abcdefgh，因為四個七段顯示器的資料線均相同，所以僅七條就可完成，再籍由快速掃描，肉眼無法辨別的情況下，達成其目的。*/
  if(scan==2)                    /*讓第三個七段小數顯示，分隔時與分*/
  {
    led_dot=0;
  }
  if(key15open==1)               /*若設定模式啟動，四個七段全掃描*/
  {
    scan_num=3;
  }
  else if(key15open!=1)          /*若設定模式未啟動，時針或分針左邊為零則不顯示*/
  {
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
  }
    scan++;                       /*掃描累加*/
    if(scan>scan_num)             /*若沒達到以上要求就歸零*/
    { 
      scan=0;
    } 
}
void T0_int(void)interrupt 1      /*中斷程序，模式為1*/
{
  TH0=(65536-2000)/256;           /*設定下次中斷執行時間，時間一到，再度進入此中斷程序*/
  TL0=(65536-2000)%256;           /*如此才可不斷做七段累加動作。*/
  scan_7seg();                    /*七段顯示器掃描程序*/
  if(key15open==1)                /*設定模式啟動*/
  {
    if(seg[switch_sel]!=16)       /*16為七段全部不顯示，為達到選擇閃爍，必須在重設時先把數值儲存*/
    {
      title_segreg=seg[switch_sel];  /*儲存到title_segreg*/
    }
    title_count++;                /*閃爍計數*/
    if(title_count<250)           /*暗250單位時間*/
    {
      seg[switch_sel]=16;
    }
    else if((title_count>=250)&&(title_count<500))  /*回存數值並亮250單位時間*/
    {
      seg[switch_sel]=title_segreg;
    }
    else if(title_count>=500)      /*輪回閃爍動作，直到設定模式關閉*/
    {
      title_count=0;
    }
  }
  else if(key15open!=1)            /*如果設定模式關閉，繼續時鐘計數*/
  {
    int_delay++;                   /*延遲計數*/
    while(int_delay==500)          /*每次計數500*中斷2000u秒*/
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
}
unsigned short scan_key(void)      /*鍵盤掃描函數*/
{
  unsigned short i,j,find=0,ini,inj,in;  /*i、j水平垂直掃描迴圈，find是否有按鍵，ini、inj記錄當下水平垂直鍵值，in水平掃描記錄*/
  for(i=0;i<4;i++)                 /*垂直掃描迴圈*/
  {
    P2=act[i];                     /*送入垂直掃描信號*/
    delay(3);                      /*延遲3單位*/
    in=P2;
    in=in>>4;                      /*記錄水平信號（高四位元為水平信號，低四位元為垂直信號*/
    in=in|0xf0;                    /*水平信號清空*/
    for(j=0;j<4;j++)               /*接收水平信號迴圈*/
    {
      if(act[j]==in)               /*如果垂直與水平掃描信號相同（表示己按鍵）*/
      {
        find=1;                    /*找到了*/
        ini=i;                     /*記錄當下水平垂直鍵值*/
        inj=j;
      }
    }
  }
  if(find==0)                      /*沒找到傳回0*/
  {
    return 0;
  }
    key=ini*4+inj;                 /*有找到根據ij轉換key值，並傳回1*/
    return 1;
}
void give_key(void)                /*給予key值函數，設定按鍵放開後才離開此函數*/
{
  unsigned short getnum_witesc;    /*按下判斷*/
  getnum_witesc=scan_key();        /*判斷是否有按下*/
  while(getnum_witesc==1)        /*進入無窮回圈直到放開才離開*/
  {
    getnum_witesc=scan_key();
  }
}
void key_15open(void)              /*設定模式啟動時的動作*/
{
  if(key==11)                      /*如果按下“B”鍵，設定中的七段顯示器往左移*/
  {
    seg[switch_sel]=title_segreg;
    switch_sel++;
    if(switch_sel>3)               /*如果達到左邊界，則移往到右邊界*/
    {
      switch_sel=0;
    }
    key=16;
  }
  if(key==10)                      /*如果按下“A”鍵，設定中的七段顯示器往右移*/
  {
    seg[switch_sel]=title_segreg;
    if(switch_sel<=0)              /*如果達到右邊界，則移往到左邊界*/
    {
      switch_sel=3;
    }
    else
    {
      switch_sel--;      
    }
    key=16;
  }
    switch(switch_sel)             /*設定中的七段顯示器動作*/
    {
      case 3:                      /*第四個七段顯示器*/
      {
        if(key<6)                  /*時針十位數僅能輸入0到5*/
        {
          seg[3]=key;              /*設定進*/
          switch_sel=2;            /*設定中的七段顯示器往右移*/
          key=16;                  /*鍵值清空*/
        }
        break;
      }
      case 2:                      /*第三個七段顯示器*/
      {
        if(key<=9)                 /*時針個位數僅能輸入0到9*/
        {
          seg[2]=key;              /*設定進*/
          switch_sel=1;            /*設定中的七段顯示器往右移*/
          key=16;                  /*鍵值清空*/
        }
        break;
      }
      case 1:                      /*第二個七段顯示器*/
      {
        if(key<6)                  /*分針十位數僅能輸入0到5*/
        {
          seg[1]=key;              /*設定進*/
          switch_sel=0;            /*設定中的七段顯示器往右移*/
          key=16;                  /*鍵值清空*/
        }
        break;
      }
      case 0:                      /*第一個七段顯示器*/
      {
        if(key<=9)                 /*分針十位數僅能輸入0到9*/
        {
          seg[0]=key;              /*設定進*/
          switch_sel=3;            /*設定中的七段顯示器移向左邊界*/
          key=16;                  /*鍵值清空*/
        }
        break;
      }
      default:                     /*其他*/
      {
        switch_sel=3;              /*指定第四個七段顯示器*/
        key=16;                    /*鍵值清空*/
      }
    }
    if(key==14)                    /*如果按下“E”鍵，設定模式關閉*/
    {
      key15open=0;
      seg[switch_sel]=title_segreg;/*把儲存的數值讀回*/
      switch_sel=3;                /*指定第四個七段顯示器，以便下次使用*/
      key=16;                      /*鍵值清空*/
    }
}
int main(void)
{
  IE=0x82;                         /*設定計時中斷*/
  TMOD=0x01;                       /*設定中斷模式*/
  TH0=(65536-2000)/256;            /*設定中斷執行時間，設定完成後，即從此時累減，減至0時進入中斷程序*/
  TL0=(65536-2000)%256;            /*設定中斷執行時間，公式：（65536－x），t＝x(us)*/
  TR0=1;                           /*啟動計時器，讓AT89S51計時器啟動*/
  P1=0;
  while(1)                         /*進入鍵盤掃描程序*/
  {
    give_key();                    /*進入鍵盤掃描函數取鍵盤值*/
    if(key==15)                    /*判別是否按下“F”鍵*/
    {
      key15open=1;                 /*啟動設定模式*/
    }
    if(key15open==1)               /*若設定模式啟動，則開始所屬動作*/
    {
      key_15open();
    }
  }
}
