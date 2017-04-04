//#include <AT89X51.H>
//#include <REGX51.H>
#include <AT89X51.H>
#define RS 	P2_7
#define RW 	P2_6

#define Enable 	P2_5
#define Out_sig P2_4
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
unsigned char state=0;
//above is for KeyScan function used
int timecnt1=0; //count 10 second then go to mode 0 
//int timecnt2=0; //count 10 second then go to mode 0 
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
			timecnt1++;
			//timecnt2++;
			if(timecnt1==10)
			{
				timecnt1=0;
				flag_timeout=0;
				if(state==1)
					state=2;
				if(state==3)
					state=0;
				//timecnt2=0;
				//mode=0;
				//WriteIns(0x01);
				//WriteIns(0x82);
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
//void ShowTime2(void)
//{	
			
//}
main()
{
	int resultnum[4]={0};
	int inputnum[4]={0};
	//int result=0;
	char inputcnt=0;
	//char calcmd=0;
	char flag_mode2=0;
	char flag_mode1=0;
	//char flag_next=0;
	char setcursor=5;
	//float refloat=0;
	//float ftemp1,ftemp2;
	unsigned int shinecnt=0;	
	//int resultmod = 0;
	//int tempf1=0,tempf2=0;
	int timenum_temp[6]={0};
	int timenum_start[6]={0};
	int timenum_end[6]={0,5,9,5,3,2};
	int start_flag=0;
	int end_flag =0;
	//unsigned long start_t=0;
	//unsigned long end_t=0;
	//unsigned long current_t=0;
	unsigned int fre_cnt=0;
	int setmode=0;
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
			if( (KeyData==0x0A)|(KeyData==0x0B)|(KeyData==0x0F) )//if key is 15 then set mode=1 
			{
				mode = 1;
				if(KeyData==0x0A)
				{
					setmode=1;
				}
				if(KeyData==0x0B)
				{
					setmode=2;
				}
				if(KeyData==0x0F)
				{
					//setmode=3;
				}
			}
			else
			{
				
				if(flag_mode1 == 0)
					mode=0;
				//setmode=0;
				//if(flag_mode1 != 1)
				//	mode = 0;
			}
			//flag_timeout = 1 ;
			timecnt1=0;
			//timecnt1=0;
			fre_cnt=0;
		}
		if( mode == 0 )
		{
			if(secondcnt<3)
			{
				ShowTime();
				if(start_flag==1)
				{
					 if(
						(timenum_start[0]==timenum[0])& 
						(timenum_start[1]==timenum[1])&
						(timenum_start[2]==timenum[2])&
						(timenum_start[3]==timenum[3])&
						(timenum_start[4]==timenum[4])&
						(timenum_start[5]==timenum[5])
						)
					 {
						flag_timeout = 1;
						state=1;
						WriteIns(0x80);
						WriteData(' ');
						WriteData(' ');
						WriteData(' ');
						WriteData(' ');
						WriteData(' ');
						WriteData(' ');
						WriteData(' ');
						start_flag=0;
	
					 }
				}
				if(end_flag==1)
				{
					if(
							(timenum_end[0]==timenum[0])& 
							(timenum_end[1]==timenum[1])&
							(timenum_end[2]==timenum[2])&
							(timenum_end[3]==timenum[3])&
							(timenum_end[4]==timenum[4])&
							(timenum_end[5]==timenum[5])
							)
						 {	
							flag_timeout = 1;
							state=3;
						WriteIns(0x88);
						WriteData(' ');
						WriteData(' ');
						WriteData(' ');
						WriteData(' ');
						WriteData(' ');
						WriteData(' ');
						WriteData(' ');
						end_flag=0;
						 }
			 	 }
			
			}
			switch(state)
			{
				case 1:
				case 3:
					if(fre_cnt<50)
					{
						Out_sig = 0;
					}
					else
					{
						Out_sig = 1;
					}
				break;
				case 2:
					Out_sig = 1;
				break;
				default:Out_sig = 0;break;
			}

			fre_cnt++;
			if(fre_cnt>=100)
				fre_cnt=0;
			flag_mode1 = 0;
			//flag_mode2 = 0;
		}

		if( mode == 1 )
		{
			//flag_mode2 = 0;
			if(flag_mode1 == 0)
			{
				flag_mode1 = 1;
				TR0=0;
				setcursor=5;
				timenum_temp[0]=0;
				timenum_temp[1]=0;
				timenum_temp[2]=0;
				timenum_temp[3]=0;
				timenum_temp[4]=0;
				timenum_temp[5]=0;
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
								timenum_temp[setcursor] = KeyData;
							break;
							case 1:
							case 3:
								if(KeyData <6)
									timenum_temp[setcursor] = KeyData;
								else
									timenum_temp[setcursor] = 5;
							break;
							case 4:
								if( (timenum_temp[5]==2)&(KeyData>3) )
									timenum_temp[4] = 3;
								else
									timenum_temp[4] = KeyData;	
							break;
							case 5:
								if(KeyData<3)	
								{
									if( (timenum_temp[4]>3)&(KeyData==2) )
									{
										timenum_temp[5] = 1;
									}
									else
										timenum_temp[5] = KeyData;
								}
								else
								{
									if(timenum_temp[4]<4)
										timenum_temp[5] = 2;
									else
										timenum_temp[5] = 1;	
								}
							break;
							default:break;
						}
						setcursor--;
						if(setcursor<0)
							setcursor=5;	
					break;
					case 12:break;
					case 13:break;
					case 14:break;
					case 15:
						mode = 0;
						TR0=1;
						
						flag_mode1 =0;
   					    switch(setmode)
						{
							case 1:
								timenum_start[0] = timenum_temp[0];
								timenum_start[1] = timenum_temp[1];
								timenum_start[2] = timenum_temp[2];
								timenum_start[3] = timenum_temp[3];
								timenum_start[4] = timenum_temp[4];
								timenum_start[5] = timenum_temp[5];
								WriteIns(0x80);
								WriteData('S');
								WriteData(0x30+timenum_start[5]);
								WriteData(0x30+timenum_start[4]);
								WriteData(0x30+timenum_start[3]);
								WriteData(0x30+timenum_start[2]);
								WriteData(0x30+timenum_start[1]);
								WriteData(0x30+timenum_start[0]);	
								start_flag=1;
							break;
							case 2:
								end_flag=1;
								timenum_end[0] = timenum_temp[0];
								timenum_end[1] = timenum_temp[1];
								timenum_end[2] = timenum_temp[2];
								timenum_end[3] = timenum_temp[3];
								timenum_end[4] = timenum_temp[4];
								timenum_end[5] = timenum_temp[5];
								WriteIns(0x88);
								WriteData('E');
								WriteData(0x30+timenum_end[5]);
								WriteData(0x30+timenum_end[4]);
								WriteData(0x30+timenum_end[3]);
								WriteData(0x30+timenum_end[2]);
								WriteData(0x30+timenum_end[1]);
								WriteData(0x30+timenum_end[0]);
								if(timenum_end[1]==0)
								{
									if(timenum_end[0]==0)
									{
										timenum_end[1]=5;
									}
									else
									{
										timenum_end[1]=4;
										timenum_end[0]=10-timenum_end[0];
									}
									if(timenum_end[2]!=0)
									{
										timenum_end[2]--;
									}
									else
									{
										timenum_end[2]=9;
										if(timenum_end[3]!=0)
										{
											timenum_end[3]--;
										}
										else
										{
											timenum_end[3]=5;
											if(timenum_end[4]!=0)
											{
												timenum_end[4]--;
											}
											else
											{
												timenum_end[4]=3;											
												if(timenum_end[5]!=0)
												{
													timenum_end[5]--;
												}
												else
												{
				      								timenum_end[5]=2;
												}
											}
										}
									}
								}
								else
								{timenum_end[1]=timenum_end[1]-1;}
							break;
							default:break;
						}
					setmode =0 ;
					break;
					default:break;
				}
				shinecnt++;
				if(shinecnt==500)
					shinecnt=0;
				if(shinecnt>=250)//shine which will be setted
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
					WriteIns(0xc4);
					WriteData(0x30+timenum_temp[5]);
					WriteData(0x30+timenum_temp[4]);
					WriteData(0x3A);
					WriteData(0x30+timenum_temp[3]);
					WriteData(0x30+timenum_temp[2]);
					WriteData(0x3A);
					WriteData(0x30+timenum_temp[1]);
					WriteData(0x30+timenum_temp[0]);
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
  for(i=0;i<30;i++)
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

