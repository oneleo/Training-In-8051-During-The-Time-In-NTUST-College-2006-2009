//------------------------------

//加入AT89X51.H檔。
#include <AT89X51.H>
//定義DBPORT為P0。
#define DBPORT P0
//鍵盤與七段顯示器掃描狀態。
unsigned char act[4]={0xfe,0xfd,0xfb,0xf7};
//定義RS為LCD之RS接腳。
sbit RS=P2^7;
//定義RW為LCD之RW接腳。
sbit RW=P2^6;
//定義E為LCD之致能接腳。
sbit E=P2^5;
//宣告儲存4位鍵值的陣列。
char mess[4]={0,0,0,0};
//宣告LOCK變數與LED顯示變數。
unsigned char lock=0,led=0x0f;
//宣告LED顯示時間控制變數。
unsigned int led_con=0;

//------------------------------

//宣告LCD命令輸入函數。
void LCD_CMD(unsigned char cmd)
{
//宣告延遲變數i。
  char i;
//輸出命令至LCD。
  DBPORT=cmd;
//選擇LCD指令暫存器。
  RS=0;
//設定為寫入模式。
  RW=0;
//LCD致能。
  E=1;
//延遲一會兒。
  for(i=0;i<40;i++)
  {
    ;
  }
//選擇LCD指令暫存器。
  RS=0;
//設定為寫入模式。
  RW=0;
//LCD除能。
  E=0;
}

//------------------------------

//LCD資料輸入函數。
void LCD_DATA(unsigned char data1)
{
//宣告延遲變數i。
  char i;
//輸出資料至LCD。
  DBPORT=data1;
//選擇LCD資料暫存器。
  RS=1;
//設定為寫入模式。
  RW=0;
//LCD致能。
  E=1;
//延遲一會兒。
  for(i=0;i<40;i++)
  {
    ;
  }
//選擇LCD指令暫存器。
  RS=1;
//設定為寫入模式。
  RW=0;
//LCD除能。
  E=0;
}

//------------------------------

//宣告1mini秒延遲函數。
void delay1ms(unsigned int count)
{
//宣告變數i和j。
  unsigned int i,j;
//雙回圈延遲，可避免迴圈變數溢位。
  for(i=0;i<50;i++)
  {
    for(j=0;j<count;j++)
    {
      ;
    }
  }
}

//------------------------------

//鍵盤掃描函數。
char scan_key(void)
{
//宣告列控制迴圈r、行控制迴圈c與按下鍵暫存器in。
  char r,c,in;
  for(c=0;c<4;c++)
  {
//先給予行掃描資料。
    P3=act[c];
//延遲的目的為了排除彈跳效應。
    delay(20);
//讀出列的資料並儲存至in，若無資料則為空接資料。
    in=(P3>>4)|0xf0;
//開始判斷是否有接收資料，並判斷落在列的哪個位置。
    for(r=0;r<4;r++)
    {
      if(in==act[r])
      {
//如果找到符合資料，則回傳你所按下的數值。
        return (3-r)*4+(3-c);
      }
    }
  }
//如果沒有，則回傳-1。
  return -1;
}

//------------------------------

//宣告按下放開才回傳數值控制函數。
char give_key(void)
{
//先呼叫取回鍵盤掃描函數得到按鍵數值。
  char key=scan_key();
//如果一直為為原來的數值或沒有按下按鍵，則陷入無限迴圈，直到放開為止。
  while((key==scan_key()))
  {
//題目要求led閃爍控制。
//當lock模式啟動。
    if(lock==1)
    {
//變數led_con+1。
      led_con++;
//當變數led_con=25時。
      if(led_con==25)
      {
//變數led_con歸0。
        led_con=0;
//led反向（閃爍用）
        led=0xff-led;
//P1輸出變數led內容。
        P1=led;
      }
    }
//題目要求led閃爍控制結束。
  }
//脫離迴圈，表示已放開，則回傳數值。
  return key;
}

//------------------------------

//宣告LCD清除函數。
void clear_lcd()
{
//延遲5ms。
  delay1ms(5);
//呼叫LCD_CMD()命令函數，命令為0x3f，資料長度，字元大小控制。
  LCD_CMD(0x3f);
//呼叫LCD_CMD()命令函數，命令為0xf，顯示器開啟，顯示游標並閃爍。
  LCD_CMD(0Xf);
//呼叫LCD_CMD()命令函數，命令為0x1，清除lcd顯示。
  LCD_CMD(0X1);
//延遲2ms。
  delay1ms(2);
//呼叫 LCD_CMD()命令函數，命令為0x80，游標到最左上方。
  LCD_CMD(0X80);
}

//------------------------------

//宣告輸出至LCD文字顯示函數。
void print_lcd(unsigned char UpOrDown,unsigned char space,char code word[])
{
//宣告指標變數pp用來記錄字元陣列位置。
  char *pp;
//要求從第一行開始印出。
  if(UpOrDown==1)
  {
//從LCD第一行開始印出資料。
    LCD_CMD(0X80+space-1);
  }
//要求從第二行開始印出。
  else
  {
//從LCD第二行開始印出資料。
    LCD_CMD(0Xc0+space-1);
  }
//延遲2ms。
  delay1ms(2);
//讓指標指到字元陣列第一格位置。
  pp=word;
//如果pp指標所指的位置不為空字串，則讓LCD印出資料。
  while(*pp!=0)
  {
//讓LCD印出資料，並將pp指標累加。
    LCD_DATA(*pp++);
  }
}

//------------------------------

//主程式。
int main()
{
//宣告變數sel_num=0,key=0。
  char sel_num=0,key=0;
//宣告儲存4位密碼用陣列。
  char save_pass[4]={0,0,0,0};
//清除LCD。
  clear_lcd();
//印出作者名稱。
//第一行第三格開始印出"Starfine &"。
  print_lcd(1,3,"Starfine &");
//第二行第八格開始印出"Dororo!"。
  print_lcd(2,8,"Dororo!");
//顯示4秒。
  delay1ms(4000);
//清除LCD。
  clear_lcd();
//為了不斷執行，用無窮迴圈包住。
  while(1)
  {
//若變數sel-num<=3，表示還沒輸入滿四個數字。
    if(sel_num<=3)
    {
//陣列mess[sel_num]=函數give_key()回傳值（掃描鍵盤）。
      mess[sel_num]=give_key();
//若mess[sel_num]!=-1且mess[sel_num]<=（鍵盤值不為-1且小於等於9）。
      if((mess[sel_num]!=-1)&&(mess[sel_num]<=9))
      {
//輸出陣列mess[sel_num]的內容（加0x30是將數字轉ASCII碼）。
        LCD_DATA(mess[sel_num]+0x30);
//sel_num+1。
        sel_num++;
      }
    }
//若變數sel-num>=4。
    while(sel_num>=4)
    {
//變數KEY=函數give_key()回傳值（掃描鍵盤）。
      key=give_key();
//若KEY=10（輸入按鍵A）且LOCK狀態=0。
      if((key==10)&&(lock==0))
      {
//將sel_num清除為0。
        sel_num=0;
//清除LCD。
        clear_lcd();
//跳出while(sel_num>=4)回圈。
        break;
      }
//或者若KEY=11（輸入按鍵B）且LOCK狀態=0。
      else if((key==11)&&(lock==0))
      {
//重新設定迴圈將目前的資料儲存。
        for(sel_num=0;sel_num<=3;sel_num++)
        {
//將陣列mess[]存入陣列save_pass[]。
          save_pass[sel_num]=mess[sel_num];
        }
//第二行第一格開始印出"Save!"。
        print_lcd(2,1,"Save!");
//顯示2秒。
        delay1ms(2000);
//清除LCD。
        clear_lcd();
//跳出回圈while(sel_num>=4)。
        break;
      }
//或者若KEY=12（輸入按鍵c）且LOCK狀態=0。
      else if((key==12)&&(lock==0))
      {
//locd狀態設為1。
        lock=1;
//清除sel_num為0。
        sel_num=0;
//第一行第三格開始印出"Starfine &"。
        print_lcd(2,1,"Lock!");
//游標移至左上。
        LCD_CMD(0X80);
//跳出回圈while(sel_num>=4)。
        break;
      }
//或者若KEY=13(輸入按鍵d)且LOCK狀態=1。
      else if((key==13)&&(lock==1))
      {
//for迴圈sel_num0到3。
        for(sel_num=0;sel_num<=3;sel_num++)
        {
//若陣列save_pass[]不等於陣列mess[]。
          if(save_pass[sel_num]!=mess[sel_num])
          {
//第二行第一格開始印出"Failure!"。
            print_lcd(2,1,"Failure!");
//顯示2秒。
            delay1ms(2000);
//清除sel_num為0。
            sel_num=0;
//清除LCD。
            clear_lcd();
//第一行第三格開始印出"Lock!"。
            print_lcd(2,1,"Lock!");
//游標移到左上。
            LCD_CMD(0X80);
//跳出回圈while(sel_num>=4)。
            break;
          }
        }
//若變數sel_num不等於0。
        if(sel_num!=0)
        {
//第一行第三格開始印出"Success!"。
          print_lcd(2,1,"Success!");
//顯示2秒。
          delay1ms(2000);
//清除LCD。
          clear_lcd();
//lock狀態為0。
          lock=0;
//清除sel_num為0。
          sel_num=0;
        }
      }
    }
  }
}
