#include <AT89X51.H>
#define DBPORT P0
unsigned char act[4]={0xfe,0xfd,0xfb,0xf7};
sbit RS=P2^7;
sbit RW=P2^6;
sbit E=P2^5;
unsigned char ascii;
unsigned char lock=0,led=0x0f;
unsigned int led_con=0;

void LCD_CMD(unsigned char cmd)
{
  char i;
  DBPORT=cmd;
  RS=0;
  RW=0;
  E=1;
  for(i=0;i<40;i++);
  RS=0;RW=0;E=0;
}

void LCD_DATA(unsigned char data1)
{
  char i;
  DBPORT=data1;
  RS=1;RW=0;E=1;
  for(i=0;i<40;i++);
  RS=1;RW=0;E=0;
}

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
  while((key==scan_key()))
  {
    ;
  }
  return key;
}

void clear_lcd()
{
  delay1ms(5);
  LCD_CMD(0x3f);
  LCD_CMD(0Xf);
  LCD_CMD(0X1);
  delay1ms(2);
  LCD_CMD(0X80);
}
void print_lcd(unsigned char UpOrDown,unsigned char space,char code word[])
{
  char *pp;
  if(UpOrDown==1)
  {
    LCD_CMD(0X80+space-1);
  }
  else
  {
    LCD_CMD(0Xc0+space-1);
  }
  delay1ms(2);
  pp=word;
  while(*pp!=0)
  {
    LCD_DATA(*pp++);
  }
}

void SCON_int (void)  interrupt 4  //串列中斷函數 
{
  if(RI==1)   //若是接收中斷，RI=1，執行下列動作
  {
    RI=0;       //接收完畢，令RI=0
    ascii=SBUF;    //將接收到的字元由P1輸出
	LCD_DATA(ascii);
  } 
  //否則是發射中斷，令TI=0，忽略之
}

void UART(int BPS)  //串列環境及鮑率設定
{
  SCON=0x50;   //設定串列傳輸為MODE3及致能接收
  TMOD=0x20;   //設定TIMER1為MODE2
  TH1=256-(28800/BPS);  //設計時器決定串列傳輸鮑率
  TR1=1;      //開始計時
}

int main()
{
  unsigned char key;
  UART(9600);
  EA=1;
  ES=1;
  clear_lcd();
  print_lcd(1,1,"Starfine^o^ &");
  print_lcd(2,5,"=.=\"Dororo!");
  delay1ms(4000);
  clear_lcd();

  while(1)
  {
    key=give_key();
	if(key!=-1)
	{
	  if((key>=0)&&(key<=9))
	  {
	    SBUF=key+0x30;
	  }
	  else
	  {
	    SBUF=key+0x41-0x0a;
	  }
	  while(TI==0)
      {
	    ;
      }
      TI=0;
	}
  }
}