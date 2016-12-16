/*  ------------LCDKeypadAR.ino---------------
 *  LCD and Membrane Keypad Driver
 *  for KOEVA - Arabican Variety Green Bean Coffee Grader
 *  Author : Al Rayan
 *  Libraries included : LiquidCrystal_I2C.h (need citation of the author)
 *                       Wire.h              (need citationn of the author)
 *                       Keypad.h            (need citation of the author)
 */

#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

/*-----( Declare Variables )-----*/
String str;
boolean isDeviceReady;
char print_temp[24];
char temp[3];
unsigned int posx, posy;
const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {2,3,4,5}; //connect to row pinouts 
byte colPins[COLS] = {6,7,8,9}; //connect to column pinouts

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup()
{
  isDeviceReady = 0;
  Serial.begin(9600);
  koeva_isDeviceReady();
  koeva_init();
}

void loop()
{
  koeva_readSerialLcd();
  koeva_sentSerialKeypad();
}

void koeva_sentSerialKeypad()
{
  char key = keypad.getKey();

  if (key != NO_KEY){
    Serial.println(key);
  }
}

void koeva_init()
{
  lcd.noBacklight();
  delay(250);
  lcd.backlight(); // finish with backlight on  

//-------- Write characters on the display ------------------
// NOTE: Cursor Position: (CHAR, LINE) start at 0  
  lcd.setCursor(0,0); //Start at character 4 on line 0
  lcd.print("-------KOEVA--------");
  delay(100);
  lcd.setCursor(0,1);
  lcd.print("   Coffee Grader    ");
  delay(100);
  lcd.setCursor(0,2);
  lcd.print(" Variety  : Arabica");
  delay(100);
  lcd.setCursor(0,3);
  lcd.print("--------------------");
  delay(2000);
}

void koeva_readSerialLcd()
{
    // when characters arrive over the serial port...
    if (Serial.available()) {
      // wait a bit for the entire message to arrive
      delay(50);
      
      // read all the available characters
      while (Serial.available() > 0) {
        
        // display each character to the LCD
         str = Serial.readStringUntil(';');
         if (str.charAt(0) == 'c') {
                 lcd.clear();
         }
         
         temp[0] = str.charAt(0);
         temp[1] = str.charAt(1);
         temp[2] = '\0';
         posx = atoi(temp);
         
         temp[0] = str.charAt(3);
         temp[1] = str.charAt(4);
         temp[2] = '\0';
         posy = atoi(temp);

         lcd.setCursor(posx, posy);
         for (int i = 6; i < 30; i++) {
                 print_temp[i - 6] = str[i];
         }

         lcd.print(print_temp);
      }
    }
}


void koeva_isDeviceReady()
{
        lcd.begin(20,4);
        lcd.setCursor(0,1);
        lcd.print(" Waiting device...  ");
        while (!isDeviceReady) {
                  // wait a bit for the entire message to arrive
                  delay(50);
                  
                  if (Serial.available() > 0) {
                            char r = Serial.read();
                            if ( r == 'R' ) {
                                  isDeviceReady = 1;
                                  Serial.write("R");
                            }
                  }
        }
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("   Device ready!    ");
        delay(1000);
}

