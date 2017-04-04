//�[�JAT89X51.H�ɡC
#include <AT89X51.H>
//��L�P�C�q��ܾ����y�C
char act[4]={0xfe,0xfd,0xfb,0xf7};
///�C�q��ܾ�����ܤ覡�C
seg[16]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xa7,0xa1,0x86,0x8e};
//�ŧi�����ơC
void delay(int d)
{
  for(d=d*9216;d>0;d--)
  {
    ;
  }
}

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
//�p�G���ŦX��ơA�h�^�ǧA�ҫ��U���ƭȡ]�C���H�|�[��^�C
        return r*4+c;
      }
    }
  }
//�p�G�S���A�h�^��-1�C
  return -1;
}

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

//�D�{���C
int main(void)
{
//�ŧi����ƭȼȦs��key�C
  char key;
//���F���_����A�εL�a�j��]��C
  while(1)
  {
//�I�s�禡���^����ƭȡC
    key=give_key();
//�u�n���O-1�]��ܨS�����U�^�ƭȡA�N������ܡC
    if(key!=-1)
    {
//Port2��X���y�T���C
      P2=0xfe;
//Port1��X�ƭȸ�T�C
      P1=seg[key];
    }
  }
}
