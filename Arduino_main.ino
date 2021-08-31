#define ds A0
#define sh_c A2
#define st_c A1

#define ce 10

#define vpp 11
#define A9 12

int data[8]={2,3,4,5,6,7,8,9};

void set_adrres(unsigned int adr)
{
  for(int i=0;i<16;i++)
  {
    int x=adr%2;
    digitalWrite(ds,bool(x)? HIGH:LOW);
    adr=adr/2;
    digitalWrite(sh_c, HIGH);
    delay(1);
    digitalWrite(sh_c, LOW);
  }
  digitalWrite(st_c, HIGH);
  delay(1);
  digitalWrite(st_c, LOW);
}

byte read_byte()
{
  byte B=0;
  for(int i=0;i<8;i++)
  {
    pinMode(data[i],INPUT);
    delay(1);
    int t=digitalRead(data[i]);
    if(t==1)
    {
      int p=1;
      for(int j=0;j<i;j++)
      {
        p=p*2;
      }
      B=B+p;
    }
  }
  return B;
}

void write_byte(byte B)
{
  for(int i=0;i<8;i++)
  {
    pinMode(data[i],OUTPUT);
    delay(1);
    int x=B%2;
    digitalWrite(data[i],bool(x)? HIGH:LOW);
    B=B/=2;
  }
}
void read_data()
{
  long long int suma=0;

  digitalWrite(ce,LOW);
  for(int i=0;i<128;i++)
  {
    byte B;
    set_adrres(i);
    delayMicroseconds(1);
    B=read_byte();
  
    char buf[10];
    sprintf(buf,"%02X",B);
    
    Serial.print("1");
    Serial.print(buf);
  }
  digitalWrite(ce,HIGH);
}

void erase_data()
{
  set_adrres(0);
  for(int i=0;i<8;i++)
  {
    pinMode(data[i],OUTPUT);
    delay(10);
    digitalWrite(data[i],HIGH);
  }
  digitalWrite(vpp,LOW);
  digitalWrite(A9,LOW);
  delay(1000);
  digitalWrite(ce,LOW);
  delay(101);
  digitalWrite(ce,HIGH);
  digitalWrite(vpp,HIGH);
  digitalWrite(A9,HIGH);
  Serial.println("done");
}

void write_data()
{
  digitalWrite(vpp,LOW);
  delay(100);
  for(int i=-1;i<64;i++)
  {
    int b;
    Serial.print("1");
    b=Serial.read();
    if(b<0)
    {
        b=b+256;
    }
    byte c=b;
    
    set_adrres(i);
    write_byte(c);
    delayMicroseconds(4);
    digitalWrite(ce,LOW);
    delayMicroseconds(100);
    digitalWrite(ce,HIGH);
    delayMicroseconds(4);
  }
  delay(100);
  digitalWrite(vpp,HIGH);
}
void setup() {
  Serial.begin(115200);
  pinMode(ds,OUTPUT);
  digitalWrite(sh_c,LOW);
  pinMode(sh_c,OUTPUT);
  digitalWrite(st_c,LOW);
  pinMode(st_c,OUTPUT);
  digitalWrite(ce,HIGH);
  pinMode(ce,OUTPUT);
  digitalWrite(A9,HIGH);
  pinMode(A9,OUTPUT);
  digitalWrite(vpp,HIGH);
  pinMode(vpp,OUTPUT);
  int a=-1;
  while(a<=-1)
  {
    a=Serial.read()-48; 
  }
  if(a==1)
    read_data();
  else if(a==2)
    erase_data();
  else if(a==3)
    write_data();
}
void loop() {
}
