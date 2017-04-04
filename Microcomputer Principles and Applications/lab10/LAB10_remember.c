//------------------------------

//�[�JAT89X51.H�ɡC
#include <AT89X51.H>
//�w�qDBPORT��P0�C
#define DBPORT P0
//�w�qRS��LCD��RS���}�C
sbit RS=P2^7;
//�w�qRW��LCD��RW���}�C
sbit RW=P2^6;
//�w�qE��LCD���P�౵�}�C
sbit E=P2^5;
//��L���y���A�ܼơC
unsigned char act[4]={0xfe,0xfd,0xfb,0xf7};
//�ŧi8051�PPC���ǿ�SBUF���ܼơC
unsigned char ascii;

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

//�ŧi0.0001�����ơC
void delay1ms(unsigned int count)
{
//�ŧi�����x�s�ܼ�i�Mj�C
  unsigned int i,j;
//���קK�j���ܼƷ���]��65535�^�A�H���h�j��]�w����C
  for(i=0;i<50;i++)
  {
//�ĤG�h�j��C
    for(j=0;j<count;j++)
    {
//����C
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
    ;
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

//�ŧi��C���_��ơC
void SCON_int (void)  interrupt 4
{
//�Y�O�������_�ARI=1�A����U�C�ʧ@�C
  if(RI==1)
  {
//���������A�ORI=0�C
    RI=0;
//�N�����쪺�r�����ܼ�ascii�����C
    ascii=SBUF;
//��ܦ�LCD�W�C
    LCD_DATA(ascii);
  }
}

//------------------------------

//�ŧi��C���Ҥ��j�v�]�w�C
void UART(int BPS)
{
//�]�w��C�ǿ鬰MODE3�έP�౵��
  SCON=0x50;
//�]�wTIMER1��MODE2
  TMOD=0x20;
//�]�p�ɾ��M�w��C�ǿ��j�v
  TH1=256-(28800/BPS);
//�}�l�p��
  TR1=1;
}

//------------------------------

/�D�{���C
int main()
{
//�ŧi��L�Ȧs�ܼ�key�C
  unsigned char key;
//�]�w��C���Ҥ��j�v�C
  UART(9600);
//�P���C���_�C
  EA=1;
//�P���C���_�C
  ES=1;
//�M��LCD�C
  clear_lcd();
//�L�X�@�̦W�١C
//�Ĥ@��Ĥ@��}�l�L�X"Starfine^o^ &"�C
  print_lcd(1,1,"Starfine^o^ &");
//�ĤG��Ĥ���}�l�L�X"=.=\"Dororo!"�C
  print_lcd(2,5,"=.=\"Dororo!");
//���4��C
  delay1ms(4000);
//�M��LCD�C
  clear_lcd();
//���F���_����A�εL�a�j��]��C
  while(1)
  {
//�q��L���^�@��ȡC
    key=give_key();
//�p�G���^����Ȭ�-1��ܨS������A�h�i�J��ܦ�PC�{�ǡC
    if(key!=-1)
    {
//����U0��9�ȮɡA�h��PC�ù���ܡC
      if((key>=0)&&(key<=9))
      {
//��J��SBUF�Ȧs���A������PC���׺ݾ���ܡC
//�[30H�O���F������ASCII�X�ǿ�C
        SBUF=key+0x30;
      }
      else
      {
//�_�h�N���ѤU��A��F���ȡC
//�[0x41�A��h0x0a�O���F������ASCII�X�ǿ�C
        SBUF=key+0x41-0x0a;
      }
//���ݶǿ駹���C
      while(TI==0)
      {
        ;
      }
//�ǿ駹���n��TI�Ȧs���M��0�C
      TI=0;
    }
  }
}