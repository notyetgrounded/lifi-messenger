String input;  //text to be sent 
char binary[1000]; // binary value of the text
double  integervalue;// integervalue value of the text
int i,j,bittime,led; //bittime is the time of bit  , led is the pin to which led is connected
double p;
//takes user input and coverts to binary
void texttobinary()
{
  Serial.println("enter the text");
  while(1)
  {
    input =Serial.readString();
    if(input.length()>0)
    {
      break;
    }
  }
  
  Serial.print("input is");
  Serial.println( input);

  if(input[0]=='\0')
  {
    Serial.println("empty");
    loop();
  }
  for(i=0;i<input.length();i++)
  {
    integervalue=input[i];
    for(j=0;j<8;j++)
    {
      p=pow(2,7-j);
      if(integervalue>p)
      {
        strcat(binary,"1");
        integervalue=integervalue-p;
      }
      else{
        strcat(binary,"0");
      }
    }
  }
Serial.println(binary);
}
//revives binary data and sends data through light

void senddata()
{
  int bi;
  int siz=strlen(binary)/8;
  Serial.println (siz);
  for(i=0;i<siz;i++)
  {
      digitalWrite(led,LOW); //start signal
  delay(bittime);
    for(int j=0;j<8;j++)
    {
      bi=i*8+j;
    Serial.println(binary[bi]);
    if(binary[bi]==NULL)
    {
      break;
    }
    else if(binary[bi]=='1')
    {
      digitalWrite(led,HIGH);
    }
    else
    {
      digitalWrite(led,LOW);
    }
    delay(bittime);
  }
  digitalWrite(led,HIGH);
  delay(bittime);
  }
  memset(binary,0,sizeof(binary));
}
  
  

void setup() {
  led=13;
  bittime=160; 
  Serial.begin(9600);
  pinMode(led,OUTPUT);
  //Serial.setTimeout(5000);
    if(Serial.available()!=0)
  {
    Serial.println("enter the text");
  }

}

void loop() {
  digitalWrite(led,HIGH);
  texttobinary();
  senddata();
    
  }
