#include <AT89X51.H>
#define DBPORT P0
unsigned char act[4]={0xfe,0xfd,0xfb,0xf7};
sbit RS=P2^7;
sbit RW=P2^6;
sbit E=P2^5;
char mess[4]={0,0,0,0};
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
    P3=act[c];
    delay1ms(1);
    in=(P3>>4)|0xf0;
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
//題目要求led閃爍控制
    if(lock==1)
	{
	  led_con++;
	  if(led_con==25)
	  {
		led_con=0;
		led=0xff-led;
		P1=led;
	  }
	}
// 題目要求led閃爍控制結束
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

int main()
{
  char sel_num=0,key=0;
  char save_pass[4]={0,0,0,0};
  clear_lcd();
  print_lcd(1,3,"Starfine &");
  print_lcd(2,8,"Dororo!");
  delay1ms(4000);
  clear_lcd();
  while(1)
  {
    if(sel_num<=3)
    {
      mess[sel_num]=give_key();
      if((mess[sel_num]!=-1)&&(mess[sel_num]<=9))
      {
        LCD_DATA(mess[sel_num]+0x30);
        sel_num++;
      }
    }
    while(sel_num>=4)
    {
      key=give_key();
      if((key==10)&&(lock==0))
      {
        sel_num=0;
        clear_lcd();
        break;
      }
      else if((key==11)&&(lock==0))
      {
        for(sel_num=0;sel_num<=3;sel_num++)
        {
          save_pass[sel_num]=mess[sel_num];
        }
        print_lcd(2,1,"Save!");
        delay1ms(2000);
        clear_lcd();
        break;
      }
      else if((key==12)&&(lock==0))
      {
        lock=1;
        sel_num=0;
        print_lcd(2,1,"Lock!");
        LCD_CMD(0X80);
        break;
      }
      else if((key==13)&&(lock==1))
      {
        for(sel_num=0;sel_num<=3;sel_num++)
        {
          if(save_pass[sel_num]!=mess[sel_num])
          {
            print_lcd(2,1,"Failure!");
            delay1ms(2000);
            sel_num=0;
            clear_lcd();
            print_lcd(2,1,"Lock!");
            LCD_CMD(0X80);
            break;
          }
        }
        if(sel_num!=0)
        {
          print_lcd(2,1,"Success!");
          delay1ms(2000);
          clear_lcd();
          lock=0;
          sel_num=0;
        }
      }
    }
  }
}
