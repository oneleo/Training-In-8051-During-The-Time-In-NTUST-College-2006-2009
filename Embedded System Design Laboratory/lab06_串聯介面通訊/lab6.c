#include <AT89X51.H>

#define RS 	P2_7
#define RW 	P2_6

#define Enable 	P2_5

void delay(void);
void CheckBusy(void);

void WriteIns(char instruction);
void InitialLCD(void);
void WriteData(char i);
main()
{
	TR1=1;
	SCOM=0x50;
	TMOD=0x20;
	TH1=0xfd;
	//TL1=0xfd;

	while(1)
	{
	}
}


void InitialLCD(void)
{
  WriteIns(0x38);	 
  WriteIns(0x38);
  WriteIns(0x38);
  WriteIns(0x38);
  //WriteIns(0x08);	/* off display */
  WriteIns(0x01);	/* clear buffer */
  WriteIns(0x0c);	/* on display */
  WriteIns(0x06);	/* set input mode */
} /* InitialLCD */

void WriteIns(char instruction)
{
  RS=0;
  RW=0;
  Enable=1;
  P0=instruction;
  Enable=0;
  CheckBusy();
} /* WriteIns */

void WriteData(char i)
{
  RS=1;
  RW=0;
  P0=i;
  Enable=1;
  P0=i;
  Enable=0;
  CheckBusy();
} /* WriteData */
void CheckBusy(void)
{
  char i=0x80;
  while(i&0x80)
    {
      RS=0;
      RW=1;
      Enable=1;
      i=P0;
      Enable=0;	
      delay();
    }  
} /* CheckBusy */

void delay(void)
{
  int i;
  for(i=0;i<100;i++)
    ;
}
