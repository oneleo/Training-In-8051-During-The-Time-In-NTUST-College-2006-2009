#include <AT89X51.H>  
int delay(unsigned int a)  //宣告延遲函數
{
  unsigned int i;    //宣告無號數
  for(i=a;i>0;i--)   //利用FOR回圈做延遲  使用變數i控制延遲時間    
  {
    ;
  }
}
void RotateRight(int count) //功能1:可以控制P1輸出  其預設為00101011  每500ms右旋一個bit  每次count右旋8個bit 
{
  int i,b,x=0;              //宣告i,b,並且設定x初始為0
  for(i=count*8+1;i>0;i--)  //將讀近來的count轉換為右旋的bit數=count*8  採用迴圈控制次數
  {
    delay(50000);             //每個bit延遲500mS
    if(x==0)                  //判斷是否為狀態x=0,如果是便把b(輸出到p1)改為0x2b  (00101011)
    {
      x=1;
      b=0x2b;
    }
    else if(x==1)       //判斷是否為狀態x=1           
    {
      if((b&0x01)==0)     //b的LSB若為0 將b右旋1個bit
      {
        b=b/2;            
      }
      else                //b的LSB若不為0 將b右旋一個bit 並且MSB+1  (10000000=0x80)              
      {
        b=b/2+0x80;
      }
    }
    P1=b;//將b的內容送至p1
  }
}
void Flash(int count)  //功能2:  可是p1控制的LED單雙號交互發亮,並且由雙數號先亮
{
  int i,a,x=0;   //宣告i,a, 並將x狀態機預設為0         
  for(i=count+1;i>0;i--) //以count的內容  當控制回圈的次數
  {
    delay(50000);//延遲500ms
    if(x==0)  //若x狀態為0, 將x狀態改為1 並且將0x55寫入變數a(單數顆發亮) 
    {
      x=1;  
      a=0x55;
    }
    else    //若x狀態不為0, 將x狀態改為0 並且將0xaa寫入變數a(雙數顆發亮) 
    {
      x=0;
      a=0xaa;
    }
    P1=a;   //將a寫入p1
  }
}
void light1(void)   //功能3: 使led依照規則發亮
{
  int i,a,b,x=0;  //宣告變數  將x狀態預設為0
  b=P3;           //將p3寫入變數b
  while(b==0x04)  //當b為0x04時便做以下回圈
  {
  b=P3;      //在回圈讓必須將p3再次寫進變數b,才能確保不會陷入無限迴圈(b一直維持在0x04)   
  
  delay(50000); //延遲
  
  if(x==0)//若為狀態0  將x狀態設為1  a=10000001  
  {
    x=1;
    a=0x81;
  }
  else if(x==1)  //若為狀態1  將x狀態設為2  a=01000010
  {
    x=2;
    a=0x42;
  }
  else if(x==2)  //若為狀態2  將x狀態設為3  a=10000001
  {
    x=3;
    a=0x24;
  }
  else if(x==3)  //若為狀態3  將x狀態設為4  a=00100100
  {
    x=4;
    a=0x18;
  }
  else if(x==4)  //若為狀態4  將x狀態設為5  a=00011000
  {
    x=5;
    a=0x24;
  }
  else if(x==5)  //若為狀態5  將x狀態設為0  a=00100100
  {
    x=0;
    a=0x42;
  }
  P1=~a;  //將a反向輸出到p1
  }
}
void light2(void) //功能: 使led依照規則發亮
{
  int a,b,x=0;  //宣告變數  將x狀態預設為0
  b=P3;         //將p3寫入變數b
  while(b==0x08)  //當b為0x08時便做以下回圈
  {
    b=P3;       //在回圈讓必須將p3再次寫進變數b,才能確保不會陷入無限迴圈(b一直維持在0x08)
    delay(50000); //延遲
    if(x==0)    //以下大略如功能3
    {
      x=1;
      a=0xc0;
    }
    else if(x==1)
    {
      x=2;
      a=0x50;
    }
    else if(x==2)
    {
      x=3;
      a=0x24;
    }
    else if(x==3)
    {
      x=4;
      a=0x11;
    }
    else if(x==4)
    {
      x=5;
      a=0x48;
    }
    else if(x==5)
    {
      x=6;
      a=0x14;
    }
    else if(x==6)
    {
      x=7;
      a=0x06;
    }
    else if(x==7)
    {
      x=0;
      a=0x01;
    }
    P1=~a;
  }
}
int main(void)  //主程式
{
unsigned char a,b; //宣告無號數字元
while(1)
{
    a=P3;   //將p3寫入a

    b=(a/16)&0x0f;   //將右移4個bit的a寫入b

    a=a&0x0f;  //取低4位元

    if(a==0x01)       //若a==0x01 呼叫功能1
    {
      RotateRight(b); //將b值傳到count
    }
    else if(a==0x02)  //若a==0x02 呼叫功能2
    {
      Flash(b);       //將b值傳到count
    }
    else if(a==0x04)  //若a==0x04 呼叫功能3
    {
      light1();       
    }
    else if(a==0x08)  //若a==0x04 呼叫功能4
    {
      light2();	      
    }
  }
}
