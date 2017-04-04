//#include <REGX51.H>
#include <AT89X51.H>

#define RS 	P2_7
#define RW 	P2_6
#define Enable 	P2_5

int KeyTemp=0xff;
int key=0;  
int KeyData=0xff;
char col=0;
char row;
char one=0;
char zero=0;
char ScanLine=0x08;
//above is for KeyScan function used
int i=0;
int j=0;
int fre=0;
unsigned int t0h=0;
unsigned int t0l=0;
unsigned char out1=0x00;
int fre2=0;
//function
void delay(void);
void CheckBusy(void);
void KeyScan(void);
void WriteIns(char instruction);
void InitialLCD(void);
void WriteData(char i);
code unsigned char sindata[100]={
128,
135,
143,
151,
159,
166,
174,
181,
188,
194,
201,
207,
213,
219,
224,
229,
233,
237,
241,
244,
246,
249,
250,
252,
252,
253,
252,
252,
250,
249,
246,
244,
241,
237,
233,
229,
224,
219,
213,
207,
201,
194,
188,
181,
174,
166,
159,
151,
143,
135,
128,
120,
112,
104,
96,
89,
81,
74,
67,
61,
54,
48,
42,
36,
31,
26,
22,
18,
14,
11,
9,
6,
5,
3,
3,
3,
3,
3,
5,
6,
9,
11,
14,
18,
22,
26,
31,
36,
42,
48,
54,
61,
67,
74,
81,
89,
96,
104,
112,
120
};
//timer 0
void T0_int(void) interrupt 1
{

	P1=sindata[i];
	i++;
	if(i==100)
	{
		i=0;
	}
	TH0=t0h/256;
 	TL0=t0l%256;
}

main()
{
	int indata[3]={0,0,0};
	int curser=0;
	int set=0;
	int en=0;

	P0=0x00;
	P2=0x00;
	P3=0x0F;
	P1=0x00;
	TMOD=0x11;
 	IE=0x8a;
 	TH0=t0h/256;
 	TL0=t0l%256;
 	TR0=0;
	InitialLCD();
	WriteIns(0x01);
	WriteIns(0x82);
	WriteIns(0x0e);
	WriteIns(0x0c);
	while(1)
	{
		KeyScan();//detect key
		if(KeyData==10)
		{
			set=1;
			TR0=0;
			i=0;
			j=0;
			fre=0;
			indata[2]=0;
			indata[1]=0;
			indata[0]=0;
			WriteIns(0x84);
			WriteData('S');
			P1=0x00;
			en=0;
		}
		if(set==1)
		{
			if(KeyData<=9)
			{	
				indata[curser]=KeyData;
				//fre = indata[0] *100+ indata[1]*10 +indata[2];
				WriteIns(0xc4);
				WriteData(0x30+indata[0]);
				WriteData(0x30+indata[1]);
				WriteData(0x30+indata[2]);
				curser++;
				if(curser==3)
				{
					curser =0;
					set=0;
					WriteIns(0x84);
					WriteData('E');
					fre = indata[0] *100+ indata[1]*10 +indata[2];
					en=1;
		
					if(fre>=100)
					{
						fre=100;
						WriteIns(0xc4);
						WriteData(0x31);
						WriteData(0x30);
						WriteData(0x30);
					}
					if((fre>75)&(fre<=100))
					{
						t0h = 65536 - 57 - (100-fre)*1.05;
						t0l = t0h;
					}
					if((fre>65)&(fre<=75))
					{
						t0h = 65536 - 57 - (100-fre)*1.33;
						t0l = t0h;
					}
					if((fre>55)&(fre<=65))
					{
						t0h = 65536 - (7080/fre);
						t0l = t0h;
					}
					if((fre>45)&(fre<=55))
					{
						if(fre>0)
						{	
							t0h = 65536 - (7550/fre);
							t0l = t0h;
						}
					}
					if((fre>35)&(fre<=45))
					{
						if(fre>0)
						{	
							t0h = 65536 - (7650/fre);
							t0l = t0h;
						}
					}
					if((fre>15)&(fre<=35))
					{
						if(fre>0)
						{	
							t0h = 65536 - (8550/fre);
							t0l = t0h;
						}
					}
					if(fre<=15)
					{
						if(fre>0)
						{	
							t0h = 65536 - (8950/fre);
							t0l = t0h;
						}
					}
/*
					switch(fre)
					{	
						case 15:
						t0h = 65536 - 57 - (100-fre)*6;
						t0l = t0h;
						break;
						case 14:
						t0h = 65536 - 57 - (100-fre)*7.7;
						t0l = t0h;
						break;
						case 13:
						t0h = 65536 - 57 - (100-fre)*9;
						t0l = t0h;
						break;
						case 12:
						t0h = 65536 - 833;
						t0l = t0h;
						break;
						case 11:
						t0h = 65536 - 909;
						t0l = t0h;
						break;
						case 10:
						t0h = 65536 - 1000;
						t0l = t0h;
						break;
						case 9:
						t0h = 65536 - 1111;
						t0l = t0h;
						break;
						case 8:
						t0h = 65536 - 1250;
						t0l = t0h;
						break;
						case 7:
						t0h = 65536 - 1428;
						t0l = t0h;
						break;
						case 6:
						t0h = 65536 - 1667;
						t0l = t0h;
						break;
						case 5:
						t0h = 65536 - 2000;//19 - 5.2
						t0l = t0h;
						break;
						case 4:
						t0h = 65536 - 2500;
						t0l = t0h;
						break;
						case 3:
						t0h = 65536 - 3300;
						t0l = t0h;
						break;
						case 2:
						t0h = 65536 - 5000;
						t0l = t0h;
						break;
						case 1:
						t0h = 65536 - 10000;
						t0l = t0h;
						break;
					}
	*/
					//fre2 = 100 - fre;
					if(fre>0)
						TR0=1;
				}
			}
		}
		KeyData = 0xff;
		
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
  for(i=0;i<50;i++)
    ;
}

void KeyScan(void)
{
  int KeyStatus; 
	
	
  P3=~ScanLine;
  KeyStatus=~P3;
  KeyStatus&=0xf0;      
  for(row=0;row<4;row++)
    {
       if(KeyStatus==0x80)
          {	
	    one=0;
       	    if(KeyTemp!=key)
	       {
		 KeyTemp=key;
		 one=0;
		 zero=1;		
               }  
            else    
	       {
             zero++;
 		     if(zero==8)
		       KeyData=KeyTemp;   		
	      }		
	  }
       key+=1;		
       KeyStatus<<=1;
    } /* row */ 
  ScanLine>>=1;	
  if(ScanLine==0)
     ScanLine=0x08;
  col++;
  if(col==4)
    {
      col=0;
      key=0;
      one++;
      if(one==5)
        {	
          zero=0;
          KeyTemp=0xff;
          KeyData=0xff;
        }
    }
} /* KeyScan */
