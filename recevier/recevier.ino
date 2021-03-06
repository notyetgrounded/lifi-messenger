/*

  Advanced BH1750 library usage example

  This example has some comments about advanced usage features.

  Connection:

    VCC -> 3V3 or 5V
    GND -> GND
    SCL -> SCL (A5 on Arduino Uno, Leonardo, etc or 21 on Mega and Due, on esp8266 free selectable)(22 in esp32)
    SDA -> SDA (A4 on Arduino Uno, Leonardo, etc or 20 on Mega and Due, on esp8266 free selectable)(21 in esp32)
    ADD -> (not connected) or GND

  ADD pin is used to set sensor I2C address. If it has voltage greater or equal to
  0.7VCC voltage (e.g. you've connected it to VCC) the sensor address will be
  0x5C. In other case (if ADD voltage less than 0.7 * VCC) the sensor address will
  be 0x23 (by default).

*/

#include <Wire.h>//for sensor BH1750
#include <BH1750.h>
//#include <LiquidCrystal.h>  // for lcd
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);



float lux_initial,lux_high,avg_fluc;
int ticktime,bittime;    //ticktime is time after which light is checked for start and stop bit, bittime is the time of each bit
String binary;   //to store the bits
char text[100]; //to store the text
int integervalue,t;   // integer value isto store the interger equivalent value of binary received


/*
  BH1750 can be physically configured to use two I2C addresses:
    - 0x23 (most common) (if ADD pin had < 0.7VCC voltage)
    - 0x5C (if ADD pin had > 0.7VCC voltage)

  Library uses 0x23 address as default, but you can define any other address.
  If you had troubles with default value - try to change it to 0x5C.

*/
BH1750 lightMeter(0x23);

void setup(){

  Serial.begin(9600);

  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);

  /*

    BH1750 has six different measurement modes. They are divided in two groups;
    continuous and one-time measurements. In continuous mode, sensor continuously
    measures lightness value. In one-time mode the sensor makes only one
    measurement and then goes into Power Down mode.

    Each mode, has three different precisions:

      - Low Resolution Mode - (4 lx precision, 16ms measurement time)
      - High Resolution Mode - (1 lx precision, 120ms measurement time)
      - High Resolution Mode 2 - (0.5 lx precision, 120ms measurement time)

    By default, the library uses Continuous High Resolution Mode, but you can
    set any other mode, by passing it to BH1750.begin() or BH1750.configure()
    functions.

    [!] Remember, if you use One-Time mode, your sensor will go to Power Down
    mode each time, when it completes a measurement and you've read it.

    Full mode list:

      BH1750_CONTINUOUS_LOW_RES_MODE
      BH1750_CONTINUOUS_HIGH_RES_MODE (default)
      BH1750_CONTINUOUS_HIGH_RES_MODE_2

      BH1750_ONE_TIME_LOW_RES_MODE
      BH1750_ONE_TIME_HIGH_RES_MODE
      BH1750_ONE_TIME_HIGH_RES_MODE_2

  */

  // begin returns a boolean that can be used to detect setup problems.
  if (lightMeter.begin(BH1750::CONTINUOUS_LOW_RES_MODE)) {
    Serial.println(F("BH1750 Advanced begin"));
  }
  else {
    Serial.println(F("Error initialising BH1750"));
  }


 // index_avg = 0;
  lux_high=0;
  ticktime=30;
  bittime=160;
  t=10;// check for t times before stoping
  //lcd.begin(16, 2);
  //lcd.print("hello, world!");

}


void loop() {
  start();
  /*if (lightMeter.measurementReady()) {
    float lux = lightMeter.readLightLevel();
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");
  }
  */

}

 void start()
 {
  //Serial.println ("in start");
  int state,fixstate,i;   //state(current state( 0or1) if 1 denotes stop bit 0 denotes start bit
  //fixstate to conforms the current bit
  for(i=0;i<t;i++)     
  {
    state=lux(ticktime);
    if(state==0)
    {
      fixstate=lux(bittime/2);
      //bittime/2 in order to record the value at middile of bit
      if(fixstate==0)  // if  conformed bit is a start bit 
      {
        bitin();// we start recording the values
      }
    }
  }
  if(binary.length()>7)
  {
    binarytotext();
  }     
  else
  {
    start();
  }
  
 }
 int lux(int Delay)
 {

  delay(Delay);    // typical 16ms, maximum 24ms
  float lux_val=lightMeter.readLightLevel();
  //Serial.println("in measurement");

  if (lux_high==0)// at starting if we dont have any high value recorded
  {
    if (lux_initial==0) 
    {
      lux_initial=lux_val;  
      avg_fluc=lux_initial/10;
      Serial.print(lux_initial);
      Serial.println("lux initial");
    }
    else
    {
      if ((lux_val+avg_fluc)>lux_initial>(lux_val-avg_fluc))
      {
        lux(Delay);
      }
      else if(lux_initial<lux_val)
      {
        lux_high=lux_val-avg_fluc;
      }
      else{
        lux_high=lux_initial-avg_fluc;
        return 0;   //returning 0 if it is a start bit
      }
      
    }
  }
 else{

  if(lux_val>=lux_high)
  {
    return 1;
  }
  else
  {
    return 0;
  }
  //Serial.println(millis());
 }
 }
 void bitin()
 {
  Serial.println("taking in data");
  int i;
  for(i=0;i<8;i++)
  {
    binary.concat(lux(bittime));
  }
  start();
}
void binarytotext()  //converting detected binary to text
{
  lcd.clear();
  Serial.println("in convertion");
  int i,j,len;
  Serial.println(binary);
  for(i=0;i<binary.length()/8;i++)
  {
    integervalue=0;
    for(j=0;j<8;j++)
    {
      Serial.println(binary[j+(i)*8]);
      if(binary[j+(i)*8]=='1')
      {
        integervalue=integervalue+pow(2,7-j); // 1101 = 1x2^3+1x2^2+0x2^1+1x2^0
        //Serial.println("1 found");
      }
    }
    text[i]=integervalue+1;
  }
  Serial.println(text);
  binary.remove(0);
  
}
