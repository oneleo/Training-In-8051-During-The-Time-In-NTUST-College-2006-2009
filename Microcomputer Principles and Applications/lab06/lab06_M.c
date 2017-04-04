//------------------------------

//�[�JAT89X51.H�ɡC
#include <AT89X51.H>
//�w�qDBPORT��P0�C
#define DBPORT P0
//��L�P�C�q��ܾ����y���A�C
unsigned char act[4]={0xfe,0xfd,0xfb,0xf7};
//�w�qRS��LCD��RS���}�C
sbit RS=P2^7;
//�w�qRW��LCD��RW���}�C
sbit RW=P2^6;
//�w�qE��LCD���P�౵�}�C
sbit E=P2^5;
//�ŧi�x�s4����Ȫ��}�C�C
char mess[4]={0,0,0,0};
//�ŧiLOCK�ܼƻPLED����ܼơC
unsigned char lock=0,led=0x0f;
//�ŧiLED��ܮɶ������ܼơC
unsigned int led_con=0;

//------------------------------

//�ŧiLCD�R�O��J��ơC
void LCD_CMD(unsigned char cmd)
{
//�ŧi�����ܼ�i�C
  char i;
//��X�R�O��LCD�C
  DBPORT=cmd;
//���LCD���O�Ȧs���C
  RS=0;
//�]�w���g�J�Ҧ��C
  RW=0;
//LCD�P��C
  E=1;
//����@�|��C
  for(i=0;i<40;i++)
  {
    ;
  }
//���LCD���O�Ȧs���C
  RS=0;
//�]�w���g�J�Ҧ��C
  RW=0;
//LCD����C
  E=0;
}

//------------------------------

//LCD��ƿ�J��ơC
void LCD_DATA(unsigned char data1)
{
//�ŧi�����ܼ�i�C
  char i;
//��X��Ʀ�LCD�C
  DBPORT=data1;
//���LCD��ƼȦs���C
  RS=1;
//�]�w���g�J�Ҧ��C
  RW=0;
//LCD�P��C
  E=1;
//����@�|��C
  for(i=0;i<40;i++)
  {
    ;
  }
//���LCD���O�Ȧs���C
  RS=1;
//�]�w���g�J�Ҧ��C
  RW=0;
//LCD����C
  E=0;
}

//------------------------------

//�ŧi1mini�����ơC
void delay1ms(unsigned int count)
{
//�ŧi�ܼ�i�Mj�C
  unsigned int i,j;
//���^�驵��A�i�קK�j���ܼƷ���C
  for(i=0;i<50;i++)
  {
    for(j=0;j<count;j++)
    {
      ;
    }
  }
}

//------------------------------

//��L���y��ơC
char scan_key(void)
{
//�ŧi�C����j��r�B�汱��j��c�P���U��Ȧs��in�C
  char r,c,in;
  for(c=0;c<4;c++)
  {
//�������汽�y��ơC
    P3=act[c];
//���𪺥ت����F�ư��u�������C
    delay(20);
//Ū�X�C����ƨ��x�s��in�A�Y�L��ƫh���ű���ơC
    in=(P3>>4)|0xf0;
//�}�l�P�_�O�_��������ơA�çP�_���b�C�����Ӧ�m�C
    for(r=0;r<4;r++)
    {
      if(in==act[r])
      {
//�p�G���ŦX��ơA�h�^�ǧA�ҫ��U���ƭȡC
        return (3-r)*4+(3-c);
      }
    }
  }
//�p�G�S���A�h�^��-1�C
  return -1;
}

//------------------------------

//�ŧi���U��}�~�^�Ǽƭȱ����ơC
char give_key(void)
{
//���I�s���^��L���y��Ʊo�����ƭȡC
  char key=scan_key();
//�p�G�@��������Ӫ��ƭȩΨS�����U����A�h���J�L���j��A�����}����C
  while((key==scan_key()))
  {
//�D�حn�Dled�{�{����C
//��lock�Ҧ��ҰʡC
    if(lock==1)
    {
//�ܼ�led_con+1�C
      led_con++;
//���ܼ�led_con=25�ɡC
      if(led_con==25)
      {
//�ܼ�led_con�k0�C
        led_con=0;
//led�ϦV�]�{�{�Ρ^
        led=0xff-led;
//P1��X�ܼ�led���e�C
        P1=led;
      }
    }
//�D�حn�Dled�{�{������C
  }
//�����j��A��ܤw��}�A�h�^�ǼƭȡC
  return key;
}

//------------------------------

//�ŧiLCD�M����ơC
void clear_lcd()
{
//����5ms�C
  delay1ms(5);
//�I�sLCD_CMD()�R�O��ơA�R�O��0x3f�A��ƪ��סA�r���j�p����C
  LCD_CMD(0x3f);
//�I�sLCD_CMD()�R�O��ơA�R�O��0xf�A��ܾ��}�ҡA��ܴ�Шð{�{�C
  LCD_CMD(0Xf);
//�I�sLCD_CMD()�R�O��ơA�R�O��0x1�A�M��lcd��ܡC
  LCD_CMD(0X1);
//����2ms�C
  delay1ms(2);
//�I�s LCD_CMD()�R�O��ơA�R�O��0x80�A��Ш�̥��W��C
  LCD_CMD(0X80);
}

//------------------------------

//�ŧi��X��LCD��r��ܨ�ơC
void print_lcd(unsigned char UpOrDown,unsigned char space,char code word[])
{
//�ŧi�����ܼ�pp�ΨӰO���r���}�C��m�C
  char *pp;
//�n�D�q�Ĥ@��}�l�L�X�C
  if(UpOrDown==1)
  {
//�qLCD�Ĥ@��}�l�L�X��ơC
    LCD_CMD(0X80+space-1);
  }
//�n�D�q�ĤG��}�l�L�X�C
  else
  {
//�qLCD�ĤG��}�l�L�X��ơC
    LCD_CMD(0Xc0+space-1);
  }
//����2ms�C
  delay1ms(2);
//�����Ы���r���}�C�Ĥ@���m�C
  pp=word;
//�p�Gpp���Щҫ�����m�����Ŧr��A�h��LCD�L�X��ơC
  while(*pp!=0)
  {
//��LCD�L�X��ơA�ñNpp���в֥[�C
    LCD_DATA(*pp++);
  }
}

//------------------------------

//�D�{���C
int main()
{
//�ŧi�ܼ�sel_num=0,key=0�C
  char sel_num=0,key=0;
//�ŧi�x�s4��K�X�ΰ}�C�C
  char save_pass[4]={0,0,0,0};
//�M��LCD�C
  clear_lcd();
//�L�X�@�̦W�١C
//�Ĥ@��ĤT��}�l�L�X"Starfine &"�C
  print_lcd(1,3,"Starfine &");
//�ĤG��ĤK��}�l�L�X"Dororo!"�C
  print_lcd(2,8,"Dororo!");
//���4��C
  delay1ms(4000);
//�M��LCD�C
  clear_lcd();
//���F���_����A�εL�a�j��]��C
  while(1)
  {
//�Y�ܼ�sel-num<=3�A����٨S��J���|�ӼƦr�C
    if(sel_num<=3)
    {
//�}�Cmess[sel_num]=���give_key()�^�ǭȡ]���y��L�^�C
      mess[sel_num]=give_key();
//�Ymess[sel_num]!=-1�Bmess[sel_num]<=�]��L�Ȥ���-1�B�p�󵥩�9�^�C
      if((mess[sel_num]!=-1)&&(mess[sel_num]<=9))
      {
//��X�}�Cmess[sel_num]�����e�]�[0x30�O�N�Ʀr��ASCII�X�^�C
        LCD_DATA(mess[sel_num]+0x30);
//sel_num+1�C
        sel_num++;
      }
    }
//�Y�ܼ�sel-num>=4�C
    while(sel_num>=4)
    {
//�ܼ�KEY=���give_key()�^�ǭȡ]���y��L�^�C
      key=give_key();
//�YKEY=10�]��J����A�^�BLOCK���A=0�C
      if((key==10)&&(lock==0))
      {
//�Nsel_num�M����0�C
        sel_num=0;
//�M��LCD�C
        clear_lcd();
//���Xwhile(sel_num>=4)�^��C
        break;
      }
//�Ϊ̭YKEY=11�]��J����B�^�BLOCK���A=0�C
      else if((key==11)&&(lock==0))
      {
//���s�]�w�j��N�ثe������x�s�C
        for(sel_num=0;sel_num<=3;sel_num++)
        {
//�N�}�Cmess[]�s�J�}�Csave_pass[]�C
          save_pass[sel_num]=mess[sel_num];
        }
//�ĤG��Ĥ@��}�l�L�X"Save!"�C
        print_lcd(2,1,"Save!");
//���2��C
        delay1ms(2000);
//�M��LCD�C
        clear_lcd();
//���X�^��while(sel_num>=4)�C
        break;
      }
//�Ϊ̭YKEY=12�]��J����c�^�BLOCK���A=0�C
      else if((key==12)&&(lock==0))
      {
//locd���A�]��1�C
        lock=1;
//�M��sel_num��0�C
        sel_num=0;
//�Ĥ@��ĤT��}�l�L�X"Starfine &"�C
        print_lcd(2,1,"Lock!");
//��в��ܥ��W�C
        LCD_CMD(0X80);
//���X�^��while(sel_num>=4)�C
        break;
      }
//�Ϊ̭YKEY=13(��J����d)�BLOCK���A=1�C
      else if((key==13)&&(lock==1))
      {
//for�j��sel_num0��3�C
        for(sel_num=0;sel_num<=3;sel_num++)
        {
//�Y�}�Csave_pass[]������}�Cmess[]�C
          if(save_pass[sel_num]!=mess[sel_num])
          {
//�ĤG��Ĥ@��}�l�L�X"Failure!"�C
            print_lcd(2,1,"Failure!");
//���2��C
            delay1ms(2000);
//�M��sel_num��0�C
            sel_num=0;
//�M��LCD�C
            clear_lcd();
//�Ĥ@��ĤT��}�l�L�X"Lock!"�C
            print_lcd(2,1,"Lock!");
//��в��쥪�W�C
            LCD_CMD(0X80);
//���X�^��while(sel_num>=4)�C
            break;
          }
        }
//�Y�ܼ�sel_num������0�C
        if(sel_num!=0)
        {
//�Ĥ@��ĤT��}�l�L�X"Success!"�C
          print_lcd(2,1,"Success!");
//���2��C
          delay1ms(2000);
//�M��LCD�C
          clear_lcd();
//lock���A��0�C
          lock=0;
//�M��sel_num��0�C
          sel_num=0;
        }
      }
    }
  }
}
