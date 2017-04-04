//#include <REGX51.H>

#include <AT89X51.H>

void Delay(int num)
{
	int i,j;
	for(i=0;i<100;i++)
		for(j=0;j<num;j++);
}


main()
{
	char segment[10]={0xC0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x98}; 
	char segment2[10]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x18};
	char F = 0x8E;
	char C = 0xc6;
	char tempt=0;
	char scanline[4]={0x0e,0x0d,0x0b,0x07};
	unsigned int ad_data=0;
	unsigned char segnum[4]={0,7,8,9};
	float ad_temp=0;
	int i=0; 
	P0=0xFF;// input adc
	P2=0x00;// scan
	P1=0x00;// scan segment
	P3=0x01;// mode 
	while(1)
	{
		ad_temp =15;
		//ad_temp = P0;
		ad_temp = (ad_temp*1000)/ 51 ;
		if(P3&0x01)
		{
			ad_data = ad_temp;
			tempt = C;
		}
		else
		{	
			ad_temp = ((ad_temp * 9)/ 5 ) + 320;
			ad_data = ad_temp;
			tempt = F;
		}

		segnum[1] = ad_data % 10;
		segnum[2] = (ad_data/10)%10;
		segnum[3] = (ad_data/100)%10;
		//segnum[3] = ad_data /1000;
		//display
		for(i=0;i<4;i++)
		{
			P2=scanline[i];
			P1=tempt;
			if((i==1)|(i==3))
            {
				P1=segment[segnum[i]];
				
			}
			if(i==2)
				P1=segment2[segnum[i]];
			Delay(1);
		}	
	}
}                                                                                
