//加入AT89X51.H檔。
#include <AT89X51.H>
//鍵盤與七段顯示器掃描。
char act[4]={0xfe,0xfd,0xfb,0xf7};
///七段顯示器的顯示方式。
seg[16]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xa7,0xa1,0x86,0x8e};
//宣告延遲函數。
void delay(int d)
{
  for(d=d*9216;d>0;d--)
  {
    ;
  }
}

//鍵盤掃描函數。
char scan_key(void)
{
//宣告列控制迴圈r、行控制迴圈c與按下鍵暫存器in。
  char r,c,in;
  for(c=0;c<4;c++)
  {
//先給予行掃描資料。
    P3=act[c];
//延遲的目的為了排除彈跳效應。
    delay(20);
//讀出列的資料並儲存至in，若無資料則為空接資料。
    in=(P3>>4)|0xf0;
//開始判斷是否有接收資料，並判斷落在列的哪個位置。
    for(r=0;r<4;r++)
    {
      if(in==act[r])
      {
//如果找到符合資料，則回傳你所按下的數值（列乘以四加行）。
        return r*4+c;
      }
    }
  }
//如果沒有，則回傳-1。
  return -1;
}

//宣告按下放開才回傳數值控制函數。
char give_key(void)
{
//先呼叫取回鍵盤掃描函數得到按鍵數值。
  char key=scan_key();
//如果一直為為原來的數值或沒有按下按鍵，則陷入無限迴圈，直到放開為止。
  while((key==scan_key()))
  {
    ;
  }
//脫離迴圈，表示已放開，則回傳數值。
  return key;
}

//主程式。
int main(void)
{
//宣告按鍵數值暫存器key。
  char key;
//為了不斷執行，用無窮迴圈包住。
  while(1)
  {
//呼叫函式取回按鍵數值。
    key=give_key();
//只要不是-1（表示沒有按下）數值，就讓它顯示。
    if(key!=-1)
    {
//Port2輸出掃描訊號。
      P2=0xfe;
//Port1輸出數值資訊。
      P1=seg[key];
    }
  }
}
