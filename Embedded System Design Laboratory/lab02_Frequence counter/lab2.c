#include <AT89X51.H>
#define RS 	P2_7
#define RW 	P2_6

#define Enable 	P2_5
unsigned short t0_fre_cou=0,t1_int_del=0,seg_scan=0;
unsigned long fre;
unsigned char bin_num[4],
              bin_tran_seg_num[16]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e},
              act[4]={0xfe,0xfd,0xfb,0xf7};
			  void WriteIns(char instruction);
void InitialLCD(void);
void WriteData(char i);
void CheckBusy(void);
void delay(void);

void T0_int(void) interrupt 1
{
  t0_fre_cou++;
  TH0=(65536-0)/256;
  TL0=(65536-0);
}

void T1_int(void) interrupt 3
{
  TH1=(65536-9216)/256;
  TL1=(65536-9216);
  t1_int_del++;
  if(t1_int_del>=100)
  {
    t1_int_del=0;
    fre=(t0_fre_cou*65536)+((TH0*256)+TL0);
    t0_fre_cou=0;
    TH0=(65536-0)/256;
    TL0=(65536-0);

    if(fre>=1000000)
    {
	WriteIns(0x01);
							WriteIns(0x82);
	  WriteIns(0xc4);
	  
      //bin_num[0]=14;
      bin_num[0]=(fre/1000000)%10;
      bin_num[1]=(fre/1000000)/10%10;
      bin_num[2]=(fre/1000000)/100%10;
	  bin_num[3]=(fre/1000000)/1000;
	  WriteData(0x30+bin_num[3]);
	  WriteData(0x30+bin_num[2]);
	  WriteData(0x30+bin_num[1]);
	  WriteData(0x30+bin_num[0]);
	  WriteData('M');
    }
    else if((fre>=1000)&&(fre<100000))
    {
      //bin_num[0]=15;
	  WriteIns(0x01);
							WriteIns(0x82);
	  WriteIns(0xc4);
      bin_num[0]=(fre/100)%10;
      bin_num[1]=((fre/100)/10)%10;
      bin_num[2]=(fre/100)/100%10;
	  bin_num[3]=((fre/100)/1000);
	  if(bin_num[3]>0)
	  	WriteData(0x30+bin_num[3]);
	  if( (bin_num[2]>0)|(bin_num[3]!=0) )
	  	WriteData(0x30+bin_num[2]);
	  WriteData(0x30+bin_num[1]);
	  WriteData('.');
	  WriteData(0x30+bin_num[0]);
	  WriteData('K');
    }
    else if((fre>=100000)&&(fre<1000000))
	{
	WriteIns(0x01);
							WriteIns(0x82);
	  WriteIns(0xc4);
      bin_num[0]=(fre/1000)%10;
      bin_num[1]=((fre/1000)/10)%10;
      bin_num[2]=(fre/1000)/100%10;
	  bin_num[3]=((fre/1000)/1000);
	  if(bin_num[3]>0)
	  	WriteData(0x30+bin_num[3]);
	  if( (bin_num[2]>0)|(bin_num[3]!=0) )
	  	WriteData(0x30+bin_num[2]);
	  WriteData(0x30+bin_num[1]);
	  //WriteData('.');
	  WriteData(0x30+bin_num[0]);
	  WriteData('K');
	}
	else if(fre<1000)
    {
	WriteIns(0x01);
							WriteIns(0x82);
	WriteIns(0xc4);
      bin_num[0]=fre%10;
      bin_num[1]=fre/10%10;
      bin_num[2]=fre/100;
      //bin_num[3]=fre/1000%10;
	  //WriteData(0x30+bin_num[3]);
	  if(bin_num[2]>0)
	  	WriteData(0x30+bin_num[2]);
	  if( (bin_num[1]>0)|(bin_num[2]!=0) )
	  	WriteData(0x30+bin_num[1]);
	  WriteData(0x30+bin_num[0]);
	  WriteData(' ');
	  WriteData(' ');
	  WriteData(' ');
    }
    else
    {
      bin_num[0]=0;
      bin_num[1]=0;
      bin_num[2]=0;
      bin_num[3]=0;
    }
  }
}

void delaycs(unsigned int d)
{
  for(;d<100;d++)
  {
    ;
  }
}
//int0當頻率計數，int1當1s計時。 
int main(void)
{
  IE=0x8a;
  IT0=1;
  IT1=1;
  TMOD=0x15;
  TR0=1;
  TR1=1;
  TH0=(65536-0)/256;
  TL0=(65536-0);
  TH1=(65536-9216)/256;
  TL1=(65536-9216);
  InitialLCD();
	WriteIns(0x01);
	WriteIns(0x82);
	WriteIns(0x0e);
	WriteIns(0x0c);
  for(;;)
  {
    //P0=bin_tran_seg_num[t0_fre_cou];
    //P2=act[seg_scan];
    //P1=bin_tran_seg_num[bin_num[seg_scan]];
    //seg_scan++;
    //if(seg_scan>=4)
    //{
    //  seg_scan=0;
   // }
    //delaycs(1);
  }
  //return 0;
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