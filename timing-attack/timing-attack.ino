#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define PASSWORD_LENGTH 4

#define BUTTON_LED    9
#define SUCCESS_LED   10

#define ROWS 4
#define COLS 3

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {8, 7, 6, 5};
byte colPins[COLS] = {4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(0x27,16,2);

const char* password = "5291";
char buffer[PASSWORD_LENGTH];

int currentBufferPosition = 0;
int entering = 0;

void displayEnteredPin() {
  lcd.setCursor(0, 0);
  lcd.print("Enter code:");

  for(uint8_t i = 0; i < currentBufferPosition; i++) {
    lcd.setCursor(i, 1);
    lcd.print(buffer[i]);
  }
  for(uint8_t i = currentBufferPosition; i < PASSWORD_LENGTH; i++) {
    lcd.setCursor(i, 1);
    lcd.print("_");
  }
}

void setup() {
  // Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_LED, OUTPUT);
  pinMode(SUCCESS_LED, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(BUTTON_LED, LOW);
  digitalWrite(SUCCESS_LED, LOW);

  lcd.init();
  lcd.backlight();

  displayEnteredPin();
}

int check(char *input) {
  int i;

  for(i = 0 ; i < PASSWORD_LENGTH ; i++) {
    if(input[i] != password[i]) {
      return 0;
    }
  }

  return 1;
}

void loop() {
  char key = keypad.getKey();
  if(key){
    if(key == '#') {
      currentBufferPosition = 0;
      entering = 1;
      displayEnteredPin();
    }

    if(entering == 1) {
      switch(key) {
        case '0':
          buffer[currentBufferPosition] = '0';
          currentBufferPosition++;
          break;
        case '1':
          buffer[currentBufferPosition] = '1';
          currentBufferPosition++;
          break;
        case '2':
          buffer[currentBufferPosition] = '2';
          currentBufferPosition++;
          break;
        case '3':
          buffer[currentBufferPosition] = '3';
          currentBufferPosition++;
          break;
        case '4':
          buffer[currentBufferPosition] = '4';
          currentBufferPosition++;
          break;
        case '5':
          buffer[currentBufferPosition] = '5';
          currentBufferPosition++;
          break;
        case '6':
          buffer[currentBufferPosition] = '6';
          currentBufferPosition++;
          break;
        case '7':
          buffer[currentBufferPosition] = '7';
          currentBufferPosition++;
          break;
        case '8':
          buffer[currentBufferPosition] = '8';
          currentBufferPosition++;
          break;
        case '9':
          buffer[currentBufferPosition] = '9';
          currentBufferPosition++;
          break;
      }
      displayEnteredPin();
      digitalWrite(BUTTON_LED, HIGH);
      delay(100);
      digitalWrite(BUTTON_LED, LOW);
    }
  }

  if(currentBufferPosition == PASSWORD_LENGTH) {
    currentBufferPosition = 0;
    entering = 0;

    int r;

    noInterrupts();
    digitalWrite(LED_BUILTIN, HIGH);
    r = check(buffer);
    digitalWrite(LED_BUILTIN, LOW);
    interrupts(); 

    // for(int i = 0; i < PASSWORD_LENGTH; i++) {
    //   Serial.print(buffer[i]);
    // }
    // Serial.println("");

    if(r) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Success");
      digitalWrite(SUCCESS_LED, HIGH);
      delay(1000);
      digitalWrite(SUCCESS_LED, LOW);
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Fail");
      digitalWrite(BUTTON_LED, HIGH);
      delay(1000);
      digitalWrite(BUTTON_LED, LOW);
    }
  }
}