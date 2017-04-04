//------------------------------

//加入AT89X51.H檔。
#include <AT89X51.H>
//定義DBPORT為P0。
#define DBPORT P0
//定義RS為LCD之RS接腳。
sbit RS=P2^7;
//定義RW為LCD之RW接腳。
sbit RW=P2^6;
//定義E為LCD之致能接腳。
sbit E=P2^5;
//鍵盤掃描狀態變數。
unsigned char act[4]={0xfe,0xfd,0xfb,0xf7};
//宣告8051與PC間傳輸SBUF之變數。
unsigned char ascii;

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

//宣告0.0001秒延遲函數。
void delay1ms(unsigned int count)
{
//宣告延遲儲存變數i和j。
  unsigned int i,j;
//為避免迴圈變數溢位（＜65535），以雙層迴圈設定延遲。
  for(i=0;i<50;i++)
  {
//第二層迴圈。
    for(j=0;j<count;j++)
    {
//空轉。
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
    ;
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

//宣告串列中斷函數。
void SCON_int (void)  interrupt 4
{
//若是接收中斷，RI=1，執行下列動作。
  if(RI==1)
  {
//接收完畢，令RI=0。
    RI=0;
//將接收到的字元由變數ascii接收。
    ascii=SBUF;
//顯示至LCD上。
    LCD_DATA(ascii);
  }
}

//------------------------------

//宣告串列環境及鮑率設定。
void UART(int BPS)
{
//設定串列傳輸為MODE3及致能接收
  SCON=0x50;
//設定TIMER1為MODE2
  TMOD=0x20;
//設計時器決定串列傳輸鮑率
  TH1=256-(28800/BPS);
//開始計時
  TR1=1;
}

//------------------------------

/主程式。
int main()
{
//宣告鍵盤暫存變數key。
  unsigned char key;
//設定串列環境及鮑率。
  UART(9600);
//致能串列中斷。
  EA=1;
//致能串列中斷。
  ES=1;
//清除LCD。
  clear_lcd();
//印出作者名稱。
//第一行第一格開始印出"Starfine^o^ &"。
  print_lcd(1,1,"Starfine^o^ &");
//第二行第五格開始印出"=.=\"Dororo!"。
  print_lcd(2,5,"=.=\"Dororo!");
//顯示4秒。
  delay1ms(4000);
//清除LCD。
  clear_lcd();
//為了不斷執行，用無窮迴圈包住。
  while(1)
  {
//從鍵盤取回一鍵值。
    key=give_key();
//如果取回的鍵值為-1表示沒有按鍵，則進入顯示至PC程序。
    if(key!=-1)
    {
//當按下0～9值時，則讓PC螢幕顯示。
      if((key>=0)&&(key<=9))
      {
//輸入至SBUF暫存器，直接讓PC之終端機顯示。
//加30H是為了直接由ASCII碼傳輸。
        SBUF=key+0x30;
      }
      else
      {
//否則就為剩下的A～F的值。
//加0x41再淢去0x0a是為了直接由ASCII碼傳輸。
        SBUF=key+0x41-0x0a;
      }
//等待傳輸完畢。
      while(TI==0)
      {
        ;
      }
//傳輸完畢要讓TI暫存器清為0。
      TI=0;
    }
  }
}