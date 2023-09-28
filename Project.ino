/*
 * security of a door that safeguards rovers.ino
 *
 * Created: 27/9/2023 2:00:47 PM
 * Author : Mostafa Mohamed Abdel-Azzem
 * Code : 43
 *
 */ 


/*---------------------------------------------------------------------------------------*/
                              /*Including necessary libraries to use them*/
#include <Servo.h>                             /*DOOR*/
#include <LiquidCrystal.h>                     /*LCD*/
#include <Keypad.h>                   /* KEYPAD FOR PASSWORD */

/*---------------------------------------------------------------------------------------*/
                                  /*Some global variables*/
const byte ROWS     = 3   ; /*Number of rows used in keypad*/
const byte COLS     = 3   ; /*Number of columns used in keypad*/
const byte MotorPin = A3  ; /* Analog pin for the signal in servo motor */
const byte Open     = 90  ; /*Opening the door */
const byte Close    = 0   ; /*Closing the door */
const byte buzzer   = A1  ; /*Analog pin for the signal of buzzer*/
const byte RED      = A0  ; /*Analog pin for the red colour of RGB LED*/
const byte GREEN    = A2  ; /*Analog pin for the green colour of RGB LED*/
boolean flag              ; /*Flag to check the password*/
char key                  ; /*Variable to store the value of the pressed number of the keypad*/

Servo motor ;               /*Object of the Servo class*/
          /*LCD ( RS, E , D4 , D5 ,D6 ,D7)*/
LiquidCrystal Lcd(2 , 3 , 4 , 5 , 6 , 7 );
/* The Keymap of Keypad */
char KeyMap[ROWS][COLS] =
{

{'1' , '2' , '3' } ,
{'4' , '5' , '6' } ,
{'7' , '8' , '9' } ,

};
/*PINS of the rows and columns*/
byte RowPins[ROWS] = { 11 ,   12 ,    13      };
byte ColPins[COLS] = {  8 ,   9   ,   10      };

Keypad MyKeypad = Keypad( makeKeymap(KeyMap) , RowPins , ColPins , ROWS ,  COLS  ) ;
/*Password stored in array*/
char password[] ={ '7' , '4' , '3' , '2' , '1' , '6' , '5' , '8' }  ;

/*-------------------------------------------------------------------------------------------------*/

void setup()
{
  /*Defining RGB LED and Buzzer as output*/
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(buzzer, OUTPUT);
  /*The intial state of motor to is closed*/
  motor.write(Close);
  delay(500); 
  /*Pin of the motor*/
  motor.attach(MotorPin);
  /*Turn on the LCD with 16 columns and 2 rows and the cursor and print the message */
  Lcd.begin(16,2);
  Lcd.cursor();
  Lcd.print("Plz enter the ");
  Lcd.setCursor( 0 , 1 );
  Lcd.print("password.");
  delay(1000);
  
}

/*------------------------------------------------------------------------------------------------------*/

void loop()
{
  /* Store true in flag then clear the screen (perivious message) and print new one in the first row and wait for the password n the second row  */
  flag = true ;   
  Lcd.clear();
  Lcd.print("Password:");
  Lcd.setCursor( 0 , 1 );

  /*Loop to recieve the password from the user and compare it with the true password. 
  ,any number will be printed as # on the LCD for security purpose,
   number 9 on the keypad used as enter button so the loop will be broken. 
   flag state changes if the number entered is not correct.
    */
  for(int i = 0 ;  i < sizeof(password) ; i++ )
  {

    key = MyKeypad.waitForKey();
	
    if( key != password[i] )
    {
      flag = false ;
    }

    Lcd.print("#");
	
    if ( key == '9' )
    {
      break;
    }

  }

/*For true password the LCD will print 'Door Unlocked'.
the door will be opened.
the green colour will appear.
all these things will happen for only 2 seconds.*/
  if(  flag == true )
  {
    Lcd.clear();
    motor.write(Open);
    Lcd.print("Door Unlocked");
    analogWrite(GREEN , 255  );
    analogWrite(RED   , 0    );
    delay(2000);
    analogWrite(GREEN , 0    );
    analogWrite(RED   , 0    );
    motor.write(Close);
    
  }

/*For wrong password the LCD will print 'wrong password'.
the door will not open. 
the red colour will appear then gradually fade.
the buzzer will ring.
all these things will happen for only 2 seconds*/
  else
  {
    Lcd.clear();
    motor.write(Close);
    Lcd.print("Wrong password");
    analogWrite(RED , 255  );
    analogWrite(GREEN   , 0    );
    tone( buzzer , 1000 );
    for( int j = 255 ; j > 0 ; j-- )/*Controlling the brightness of the RGB LED*/
    {
      analogWrite(RED , j  );
      delay(5);
    }
    delay(2000);
    noTone( buzzer);
    analogWrite(GREEN , 0    );
    analogWrite(RED   , 0    );
  }
  
}
