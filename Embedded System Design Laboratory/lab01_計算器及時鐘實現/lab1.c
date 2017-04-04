//#include <REGX51.H>
#include <AT89X51.H>
#define RS 	P2_7
#define RW 	P2_6

#define Enable 	P2_5

int mode=0;
//mode = 0 -> clock mode
//mode = 1 -> set clock mode
//mode = 2 -> calculator
int KeyTemp=0xff;
int key=0;  
int KeyData=0xff;
char col=0;
char row;
char one=0;
char zero=0;
char ScanLine=0x08;
//above is for KeyScan function used
int timeoutcnt=0; //count 10 second then go to mode 0 
int timenum[6]={0};//stored clock number 
int secondcnt = 0;//use to count every second 
char flag_timeout;
//function
void delay(void);
void CheckBusy(void);
void KeyScan(void);
void WriteIns(char instruction);
void InitialLCD(void);
void WriteData(char i);
void Clock(void);
void ShowTime(void);
//timer 0
void T0_int(void) interrupt 1
{
  	TH0=(65536-50000)/256;
  	TL0=(65536-50000)%256;
  	secondcnt++;
  	if(secondcnt == 20)
  	{	
		if(flag_timeout==1)
		{
			timeoutcnt++;
			if(timeoutcnt==10)
			{
				timeoutcnt=0;
				mode=0;
				WriteIns(0x01);
				WriteIns(0x82);
			}
		}
  		Clock();
		secondcnt=0;
	}
}
//clock function  
void Clock(void)
{
	timenum[0]++;
	if(timenum[0]>9)
	{
		timenum[1]++;
		timenum[0]=0;
		if(timenum[1]>5)
		{
			timenum[2]++;
			timenum[1]=0;
			if(timenum[2]>9)
			{
				timenum[3]++;
				timenum[2]=0;
				if(timenum[3]>5)
				{
					timenum[4]++;
					timenum[3]=0;
					//hour
					if( (timenum[4]>9)&(timenum[5]<2) )
					{
						timenum[5]++;
						timenum[4]=0;		
					}
					if( (timenum[4]>3)&(timenum[5]==2) )
					{
						timenum[5]=0;
						timenum[4]=0;
					}
				}
			}
		}
	}
}

void ShowTime(void)
{	
		WriteIns(0xc4);
		WriteData(0x30+timenum[5]);
		WriteData(0x30+timenum[4]);
		WriteData(0x3A);
		WriteData(0x30+timenum[3]);
		WriteData(0x30+timenum[2]);
		WriteData(0x3A);
		WriteData(0x30+timenum[1]);
		WriteData(0x30+timenum[0]);	
}

main()
{
	int resultnum[4]={0};
	int inputnum[4]={0};
	int result=0;
	char inputcnt=0;
	char calcmd=0;
	char flag_mode2=0;
	char flag_mode1=0;
	char flag_next=0;
	char setcursor=5;
	float refloat=0;
	float ftemp1,ftemp2;
	unsigned char shinecnt=0;	
	int resultmod = 0;
	int tempf1=0,tempf2=0;
	P0=0x00;
	P2=0x00;
	P3=0x0F;
	TMOD=0x11;
 	IE=0x8a;
 	TH0=(65536-50000)/256;
 	TL0=(65536-50000)%256;
 	TR0=1;
	InitialLCD();
	WriteIns(0x01);
	WriteIns(0x82);
	WriteIns(0x0e);
	WriteIns(0x0c);
	while(1)
	{
		KeyScan();//detect key
		if( KeyData != 0xff )//if any have key  
		{
			if(KeyData==0x0f)//if key is 15 then set mode=1 
			{
				mode = 1;
			}
			else
			{
				if(flag_mode1 != 1)
					mode = 2;
			}
			flag_timeout = 1 ;
			timeoutcnt=0;	
		}

		if( mode == 0 )
		{
			if(secondcnt<3)
				ShowTime();
			flag_mode1 = 0;
			flag_mode2 = 0;
		}

		if( mode == 1 )
		{
			flag_mode2 = 0;
			if(flag_mode1 == 0)
			{
				flag_mode1 = 1;
				TR0=0;
				setcursor=5;
			}
			else
			{
				switch(KeyData)//set clock number
				{
					case 0:
					case 1:
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
					case 7:
					case 8:
					case 9:
						switch(setcursor)
						{
							case 0:
							case 2:
								timenum[setcursor] = KeyData;
							break;
							case 1:
							case 3:
								if(KeyData <6)
									timenum[setcursor] = KeyData;
								else
									timenum[setcursor] = 5;
							break;
							case 4:
								if( (timenum[5]==2)&(KeyData>3) )
									timenum[4] = 3;
								else
									timenum[4] = KeyData;	
							break;
							case 5:
								if(KeyData<3)	
								{
									if( (timenum[4]>3)&(KeyData==2) )
									{
										timenum[5] = 1;
									}
									else
										timenum[5] = KeyData;
								}
								else
								{
									if(timenum[4]<4)
										timenum[5] = 2;
									else
										timenum[5] = 1;	
								}
							break;
							default:break;
						}
						setcursor--;
						if(setcursor<0)
							setcursor=5;	
					break;
					case 11://set cursor
						setcursor--;
						if(setcursor<0)
							setcursor=5; 
					break;
					case 10:
						setcursor++;
						if(setcursor>5)
							setcursor=0;
					break;
					case 12:break;
					case 13:break;
					case 14:break;
					case 15:
						mode = 0;
						TR0=1;   
					break;
					default:break;
				}
				shinecnt++;
				if(shinecnt==150)
					shinecnt=0;
				if(shinecnt>=75)//shine which will be setted
				{
					switch(setcursor)
					{
						case 0:
							WriteIns(0xc4+0x07);
						break;
						case 1:
							WriteIns(0xc4+0x06);
						break;
						case 2:
							WriteIns(0xc4+0x04);
						break;
						case 3:
							WriteIns(0xc4+0x03);
						break;
						case 4:
							WriteIns(0xc4+0x01);
						break;
						case 5:
							WriteIns(0xc4);
						break;
					}
					WriteData(' ');
				}
				else
				{	
					ShowTime();
				}
				
			}
		}
		if( (mode == 2)&(KeyData!=0xFF) )//calculator
		{
			flag_mode1 = 0;
			if(flag_mode2 == 0)//initail
			{
				flag_mode2 = 1;
				inputcnt=0;
				WriteIns(0x01);
				calcmd = 0;
				flag_next=0;
				inputnum[0]=0;
				inputnum[1]=0;
				inputnum[2]=0;
				inputnum[3]=0;
			}
			else
			{
				if( (KeyData>9)&(KeyData<14) )//set command + - * /
				{
					switch(KeyData)
					{
						case 10:
							calcmd = 1; 
							WriteIns(0xc4+0x02);
							WriteData(0x2B);
						break;
						case 11:
							calcmd = 2;
							WriteIns(0xc4+0x02);
							WriteData(0x2D);
						break;
						case 12:
							calcmd = 3;
							WriteIns(0xc4+0x02);
							WriteData(0x2A);
						break;
						case 13:
							calcmd = 4;
							WriteIns(0xc4+0x02);
							WriteData(0x2F);
						break;
						default:break;
					}
					if(flag_next==0)
					{
						flag_next=1;
						inputcnt=2;
					}
					else
					{
						flag_next=0;
						inputcnt=0;
					}	
				}
				if( (KeyData<10) ) // set operand number
				{
					if(flag_next==0)
					{
						if(inputcnt==0)
						{
							inputnum[1] = KeyData;
						}
						if(inputcnt==1)
						{
							inputnum[0] = inputnum[1];
							WriteIns(0xc4+inputcnt-0x01);
							WriteData(0x30 + inputnum[0]);
							inputnum[1] = KeyData;
						}
						WriteIns(0xc4+inputcnt);
						WriteData(0x30 + KeyData);
						inputcnt++;
						if(inputcnt>=2)
							inputcnt=0;
					}
					else
					{
						if(inputcnt==2)
						{
							inputnum[3] = KeyData;
						}
						if(inputcnt==3)
						{
							inputnum[2] = inputnum[3];
							WriteIns(0xc4+inputcnt);
							WriteData(0x30 + inputnum[2]);
							inputnum[3] = KeyData;
						}
						WriteIns(0xc4+inputcnt+0x01);
						WriteData(0x30 + KeyData);
						inputcnt++;
						if(inputcnt>=4)
							inputcnt=2;
					}
				}
				if(KeyData==14)
				{
					flag_mode2 = 0;
					switch(calcmd) //if it doesn't set the command and press the answer then display Error
					{
						case 0:
							WriteIns(0x01);
							WriteIns(0x82);
							WriteIns(0xc4+0x01);
							WriteData('E');
							WriteData('r');
							WriteData('r');
							WriteData('o');
							WriteData('r');
						break;
						case 1:  //add
							result = (inputnum[0]*10 + inputnum[1])+(inputnum[2]*10 + inputnum[3]); 
							resultnum[0] = result%10;
							resultnum[1] = (result/10)%10;
							resultnum[2] = result/100;
							WriteIns(0x01);
							WriteIns(0x82);
							WriteIns(0xc4+0x01);
							if(resultnum[2]>0)
								WriteData(0x30 + resultnum[2]);
							if( (resultnum[1]>0)|(resultnum[2]!=0) )
								WriteData(0x30 + resultnum[1]);
							WriteData(0x30 + resultnum[0]);
						break;
						case 2: //minus
							result = (inputnum[0]*10 + inputnum[1])-(inputnum[2]*10 + inputnum[3]);
							WriteIns(0x01);
							WriteIns(0x82);
							WriteIns(0xc4);
							if(result<0)
							{
								result = (~result)+1;
								WriteData(0x2D); 
							}
							resultnum[0] = result%10;
							resultnum[1] = (result/10)%10;
							if(resultnum[1]>0)
								WriteData(0x30 + resultnum[1]);
							WriteData(0x30 + resultnum[0]);
						break;
						case 3: //product
							result = (inputnum[0]*10 + inputnum[1])*(inputnum[2]*10 + inputnum[3]);
							resultnum[0] = result%10;
							resultnum[1] = (result/10)%10;
							resultnum[2] = (result/100)%10;
							resultnum[3] = result/1000;
							WriteIns(0x01);
							WriteIns(0x82);
							WriteIns(0xc4+0x01);
							if(resultnum[3]>0)
								WriteData(0x30 + resultnum[3]);
							if( (resultnum[2]>0)|(resultnum[3]!=0) )
								WriteData(0x30 + resultnum[2]);
							if( (resultnum[1]>0)|(resultnum[2]!=0)|(resultnum[3]!=0) )
								WriteData(0x30 + resultnum[1]);
							WriteData(0x30 + resultnum[0]); 
						break;
						case 4: //divid
							WriteIns(0x01);
							WriteIns(0x82);
							WriteIns(0xc4+0x01);
							ftemp1 = inputnum[0]*10 + inputnum[1];
							ftemp2 = inputnum[2]*10 + inputnum[3];
							tempf1 = inputnum[0]*10 + inputnum[1];
							tempf2 = inputnum[2]*10 + inputnum[3];
							if( ftemp2!=0 ) //if ftemp2 = 0 then show Error 
							{ 		
 							    refloat = ftemp1/ftemp2;
								result = refloat;
								resultmod = tempf1%tempf2; 
								

								resultnum[0] = resultmod%10;
								resultnum[1] = resultmod/10;

								resultnum[2] = result%10;
								resultnum[3] = result/10;

								if(resultnum[3]>0)
									WriteData(0x30 + resultnum[3]);
								WriteData(0x30 + resultnum[2]);
								WriteData('.');
								WriteData('.');
								WriteData('.');
								if(resultnum[1]>0)
									WriteData(0x30 + resultnum[1]);
								WriteData(0x30 + resultnum[0]);
							}
							else
							{
								WriteData('E');
								WriteData('r');
								WriteData('r');
								WriteData('o');
								WriteData('r');
							}
						break;
						default:break;
					}
					
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
  for(i=0;i<100;i++)
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
 		     if(zero==5)
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

