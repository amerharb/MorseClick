
#include <PS2Keyboard.h>
#include <LiquidCrystal.h>

const int DataPin = 4;
const int IRQpin =  3;

PS2Keyboard keyboard;
LiquidCrystal lcd(10, 11, 6, 7, 8, 9);

//this will determine the pase of morse code and fequancy
int unitLong = 50;
int dotLong = unitLong;
int dashLong = unitLong * 3;
int afterLetterLong = unitLong * 3;
int spaceLong = unitLong  * 7;
int freq = 450;

//pin of the buzzer that will beep the morse code
int buzzerPin = 5;
int ledPin = 13;

int interPin = 2;
unsigned long interPress;
boolean sosMode = false;

//morseMode
int mode = 1; // 1. morse directly each click 2. morse words after the Enter
String stmt = "";
String lastStmt = "";
int curCharMorse = 0;

void setup() {
  delay(1000);

  pinMode(interPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interPin), stopMorsing, FALLING);

  //set number of columns and ros
  lcd.begin(16, 2);
  lcd.noCursor();
  lcd.noAutoscroll();
  lcd.setCursor(2, 0);
  lcd.print("Morse  Click");

  byte AA[8] = { //Ä
    B10001,
    B00000,
    B00000,
    B01110,
    B10001,
    B11111,
    B10001,
  };

  byte AAA[8] = { //Å
    B00100,
    B01010,
    B00100,
    B01110,
    B10001,
    B11111,
    B10001,
  };

  byte OO[8] = { //Ö
    B10001,
    B00000,
    B01110,
    B10001,
    B10001,
    B10001,
    B01110,
  };

  lcd.createChar(0, AA); //Ä
  lcd.createChar(1, AAA); //Å
  lcd.createChar(2, OO); //Ö

  keyboard.begin(DataPin, IRQpin);
  Serial.begin(9600);
  Serial.println("Keyboard Test:");

  pinMode (buzzerPin, OUTPUT) ;// set the digital IO pin mode, OUTPUT out of Wen
  pinMode(ledPin, OUTPUT);

}

void loop() {

  unsigned long interHold = 0;

  if (digitalRead(interPin)) {
    interPress = millis() ;
    while (digitalRead(interPin) && interHold <= 5000) {
      interHold = (millis() - interPress);
      delay(10);
    }

    if (interHold > 5000) {
      Serial.println("SOS");
      lcd.clear();
      lcd.setCursor(6, 0);
      lcd.print("SOS");

      sosMode = true;
      while (sosMode) {
        morseS();
        morseO();
        morseS();
        morseSpace();
        morseSpace();
      }
      lcd.clear();
      lcd.noCursor();
      lcd.noBlink();
    }
  } else {
    interHold = 0;
    if (keyboard.available()) {

      // read the next key
      char c = keyboard.read();
      Serial.print(int(c));
      // check for some of the special keys
      if (c == PS2_ENTER) {
        Serial.println();
        if (mode == 1) {
          morseChar(' ');
        } else if (mode == 2) {
          lcd.home();
          lcd.noAutoscroll();
          lcd.blink();
          if (stmt == "") { // morse last statment
            lcd.setCursor(0, 0);
            lcd.print(lastStmt);
            for (int i = 0; i < lastStmt.length(); i++) {
              curCharMorse = i;
              morseChar(lastStmt.charAt(i));
            }
          } else {
            for (int i = 0; i < stmt.length(); i++) {
              curCharMorse = i;
              morseChar(stmt.charAt(i));
            }
            lastStmt = stmt;
            stmt = "";
          }
          lcd.noBlink();
          lcd.cursor();
        }
      } else if (c == PS2_TAB) {
        Serial.print("[Tab]");
      } else if (c == PS2_ESC) {
        Serial.print("[ESC]");
      } else if (c == PS2_PAGEDOWN) {
        Serial.println("mode = 2, morse stmt at Enter");
        mode = 2;
        stmt = "";
        lcd.clear();
        lcd.home();
        lcd.print("Morse on Enter");
        lcd.noCursor();
        lcd.noBlink();
      } else if (c == PS2_PAGEUP) {
        Serial.println("mode = 1 directly morse");
        mode = 1;
        lcd.noCursor();
        lcd.noBlink();
        lcd.noAutoscroll();
        lcd.clear();
        lcd.home();
        lcd.print("Morse on Click");
      } else if (c == PS2_LEFTARROW) {
        Serial.print("[Left]");
        if (freq > 100) {
          freq -= 10;
          lcd.clear();
          lcd.print("Freq = " + String(freq));
        }
      } else if (c == PS2_RIGHTARROW) {
        Serial.print("[Right]");
        if (freq < 1000) {
          freq += 10;
          lcd.clear();
          lcd.print("Freq = " + String(freq));
        }
      } else if (c  == PS2_UPARROW) {
        Serial.print("[Up]");
        if (unitLong < 100) {
          unitLong++;
          updateTimes();
          lcd.clear();
          lcd.print("Phase = " + String(unitLong));
        }
      } else if (c == PS2_DOWNARROW) {
        Serial.print("[Down]");
        if (unitLong > 10) {
          unitLong--;
          updateTimes();
          lcd.clear();
          lcd.print("Phase = " + String(unitLong));
        }
      } else if (c == PS2_DELETE) {
        Serial.print("[Del]");
        if (mode == 2) {
          stmt = "";
          lastStmt = "";
          lcd.clear();
          lcd.cursor();
        }
      } else {
        if (mode == 1) {
          morseChar(c);
        } else if (mode == 2) {
          lcd.cursor();
          if (stmt.length() == 0) {
            lcd.clear();
            lcd.home();
            lcd.noAutoscroll();
          } else if (stmt.length() <= 15) {
            lcd.noAutoscroll();
          } else if (stmt.length() > 15) {
            lcd.autoscroll();
          }
          stmt += c;
          //lcd.clear();
          //lcd.home();
          //lcd.print(stmt);
          if (c == ';' || c == ':') {
            lcd.write(byte(2));
          } else if (c == '\'' || c == '"') {
            lcd.write(byte(0));
          } else if (c == '[' || c == '{') {
            lcd.write(byte(1));
          } else {
            lcd.print(c);

          }
        }
      }
    }
  }
}

boolean firstStopAfterSOS = true;
void stopMorsing() {
  Serial.println(" STOP ");
  if (sosMode) {
    if (firstStopAfterSOS ) {
      firstStopAfterSOS  = false;
    } else {
      sosMode = false;
      firstStopAfterSOS = true ;
      
    }
  } else {
    stmt = "";
    lastStmt = "";
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("STOP");
    while (keyboard.available()) {
      keyboard.read();
    }
  }
}

void buzz(int miliSec) {
  for (int i = 0; i < (miliSec / (1000 / freq)); i++) // Wen a frequency sound
  {
    digitalWrite (buzzerPin, HIGH) ;// send voice
    delay (1500 / freq / 2) ; // Delay 1ms
    digitalWrite (buzzerPin, LOW) ;// do not send voice
    delay (1500 / freq / 2) ; // delay ms
  }
}

void morseDot() {
  digitalWrite(ledPin, HIGH);
  buzz(dotLong);
  digitalWrite(ledPin, LOW);
  delay(unitLong); //
}

void morseDash() {
  digitalWrite(13, HIGH);
  buzz(dashLong);
  digitalWrite(ledPin, LOW);
  delay(unitLong);
}

void moresAfterLetter() {
  digitalWrite(ledPin, LOW);
  delay(afterLetterLong - unitLong);

}

void morseSpace() {
  digitalWrite(13, LOW);
  delay(spaceLong - unitLong); // we take one unitLong becuase the letter consume one unit after the letter
}

//A B C D E F G H I J K L M N O P Q R S T U V W X Y Z 1 2 3 4 5 6 7 8 9 0

void morseA() {
  //morse .-
  displayMorse("A", "._");

  morseDot();
  morseDash();
  moresAfterLetter();

}

void morseB() {
  //morse -...
  displayMorse("B", "_...");

  morseDash();
  morseDot();
  morseDot();
  morseDot();
  moresAfterLetter();
}

void morseC() {
  //morse -.-.
  displayMorse("C", "_._.");

  morseDash();
  morseDot();
  morseDash();
  morseDot();
  moresAfterLetter();
}

void morseD() {
  //morse --.
  displayMorse("D", "__.");

  morseDash();
  morseDash();
  morseDot();
  moresAfterLetter();
}

void morseE() {
  //morse .
  displayMorse("E", ".");

  morseDot();
  moresAfterLetter();
}

void morseF() {
  //morse ..-.
  displayMorse("F", ".._.");

  morseDot();
  morseDot();
  morseDash();
  morseDot();
  moresAfterLetter();
}

void morseG() {
  //morse --.
  displayMorse("G", "__.");

  morseDash();
  morseDash();
  morseDot();
  moresAfterLetter();
}

void morseH() {
  //morse ....
  displayMorse("H", "....");

  morseDot();
  morseDot();
  morseDot();
  morseDot();
  moresAfterLetter();
}

void morseI() {
  //morse ..
  displayMorse("I", "..");

  morseDot();
  morseDot();
  moresAfterLetter();
}

void morseJ() {
  //morse .---
  displayMorse("J", ".___");

  morseDot();
  morseDash();
  morseDash();
  morseDash();
  moresAfterLetter();
}

void morseK() {
  //morse -.-
  displayMorse("K", "_._");

  morseDash();
  morseDot();
  morseDash();
  moresAfterLetter();
}

void morseL() {
  //morse .-..
  displayMorse("L", "._..");

  morseDot();
  morseDash();
  morseDot();
  morseDot();
  moresAfterLetter();
}

void morseM() {
  //morse --
  displayMorse("M", "__");

  morseDash();
  morseDash();
  moresAfterLetter();
}

void morseN() {
  //morse -.
  displayMorse("N", "_.");

  morseDash();
  morseDot();
  moresAfterLetter();
}

void morseO() {
  //morse ---
  displayMorse("O", "___");

  morseDash();
  morseDash();
  morseDash();
  moresAfterLetter();
}

void morseP() {
  //morse .--.
  displayMorse("P", ".__.");

  morseDot();
  morseDash();
  morseDash();
  morseDot();
  moresAfterLetter();
}

void morseQ() {
  //morse --.-
  displayMorse("Q", "__._");

  morseDash();
  morseDash();
  morseDot();
  morseDash();
  moresAfterLetter();
}

void morseR() {
  //morse .-.
  displayMorse("R", "._.");

  morseDot();
  morseDash();
  morseDot();
  moresAfterLetter();
}

void morseS() {
  //morse ...
  displayMorse("S", "...");

  morseDot();
  morseDot();
  morseDot();
  moresAfterLetter();
}

void morseT() {
  //morse -
  displayMorse("T", "_");

  morseDash();
  moresAfterLetter();
}

void morseU() {
  //morse ..-
  displayMorse("U", ".._");

  morseDot();
  morseDot();
  morseDash();
  moresAfterLetter();
}

void morseV() {
  //morse ...-
  displayMorse("V", "..._");

  morseDot();
  morseDot();
  morseDot();
  morseDash();
  moresAfterLetter();
}

void morseW() {
  //morse .--
  displayMorse("W", ".__");

  morseDot();
  morseDash();
  morseDash();
  moresAfterLetter();
}

void morseX() {
  //morse -..-
  displayMorse("X", "_.._");

  morseDash();
  morseDot();
  morseDot();
  morseDash();
  moresAfterLetter();
}

void morseY() {
  //morse -.--
  displayMorse("Y", "_.__");

  morseDash();
  morseDot();
  morseDash();
  morseDash();
  moresAfterLetter();
}

void morseZ() {
  //morse --..
  displayMorse("Z", "__..");

  morseDash();
  morseDash();
  morseDot();
  morseDot();
  moresAfterLetter();
}

void morseOO() { //Ö
  //morse ---.
  displayMorse("Ö", "___.");

  morseDash();
  morseDash();
  morseDash();
  morseDot();
  moresAfterLetter();
}

void morseAA() { //Ä
  //morse .-.-
  displayMorse("Ä", "._._");

  morseDot();
  morseDash();
  morseDot();
  morseDash();
  moresAfterLetter();
}

void morseAAA() { //Å
  //morse .--.-
  displayMorse("Å", ".__._");

  morseDot();
  morseDash();
  morseDash();
  morseDot();
  morseDash();
  moresAfterLetter();
}

void morse1() {
  //morse .----
  displayMorse("1", ".____");

  morseDot();
  morseDash();
  morseDash();
  morseDash();
  morseDash();
  moresAfterLetter();
}

void morse2() {
  //morse ..---
  displayMorse("2", "..___");

  morseDot();
  morseDot();
  morseDash();
  morseDash();
  morseDash();
  moresAfterLetter();
}

void morse3() {
  //morse ...--
  displayMorse("3", "...__");

  morseDot();
  morseDot();
  morseDot();
  morseDash();
  morseDash();
  moresAfterLetter();
}

void morse4() {
  //morse ....-
  displayMorse("4", "...._");

  morseDot();
  morseDot();
  morseDot();
  morseDot();
  morseDash();
  moresAfterLetter();
}

void morse5() {
  //morse .....
  displayMorse("5", ".....");

  morseDot();
  morseDot();
  morseDot();
  morseDot();
  morseDot();
  moresAfterLetter();
}

void morse6() {
  //morse -....
  displayMorse("6", "_....");

  morseDash();
  morseDot();
  morseDot();
  morseDot();
  morseDot();
  moresAfterLetter();
}

void morse7() {
  //morse --...
  displayMorse("7", "__...");

  morseDash();
  morseDash();
  morseDot();
  morseDot();
  morseDot();
  moresAfterLetter();
}

void morse8() {
  //morse ---..
  displayMorse("8", "___..");

  morseDash();
  morseDash();
  morseDash();
  morseDot();
  morseDot();
  moresAfterLetter();
}

void morse9() {
  //morse ----.
  displayMorse("9", "____.");

  morseDash();
  morseDash();
  morseDash();
  morseDash();
  morseDot();
  moresAfterLetter();
}

void morse0() {
  //morse -----
  displayMorse("0", "_____");

  morseDash();
  morseDash();
  morseDash();
  morseDash();
  morseDash();
  moresAfterLetter();
}

void morseChar(char c) {

  if (c == 'a' || c == 'A') {
    Serial.print("A");
    morseA();
  } else if (c == 'b' || c == 'B') {
    Serial.print("B");
    morseB();
  } else if (c == 'c' || c == 'C') {
    Serial.print("C");
    morseC();
  } else if (c == 'd' || c == 'D') {
    Serial.print("D");
    morseD();
  } else if (c == 'e' || c == 'E') {
    Serial.print("E");
    morseE();
  } else if (c == 'f' || c == 'F') {
    Serial.print("F");
    morseF();
  } else if (c == 'g' || c == 'G') {
    Serial.print("G");
    morseG();
  } else if (c == 'h' || c == 'H') {
    Serial.print("H");
    morseH();
  } else if (c == 'i' || c == 'I') {
    Serial.print("I");
    morseI();
  } else if (c == 'j' || c == 'J') {
    Serial.print("J");
    morseJ();
  } else if (c == 'k' || c == 'K') {
    Serial.print("K");
    morseK();
  } else if (c == 'l' || c == 'L') {
    Serial.print("L");
    morseL();
  } else if (c == 'm' || c == 'M') {
    Serial.print("M");
    morseM();
  } else if (c == 'n' || c == 'N') {
    Serial.print("N");
    morseN();
  } else if (c == 'o' || c == 'O') {
    Serial.print("O");
    morseO();
  } else if (c == 'p' || c == 'P') {
    Serial.print("P");
    morseP();
  } else if (c == 'q' || c == 'Q') {
    Serial.print("Q");
    morseQ();
  } else if (c == 'r' || c == 'R') {
    Serial.print("R");
    morseR();
  } else if (c == 's' || c == 'S') {
    Serial.print("S");
    morseS();
  } else if (c == 't' || c == 'T') {
    Serial.print("T");
    morseT();
  } else if (c == 'u' || c == 'U') {
    Serial.print("U");
    morseU();
  } else if (c == 'v' || c == 'V') {
    Serial.print("V");
    morseV();
  } else if (c == 'w' || c == 'W') {
    Serial.print("W");
    morseW();
  } else if (c == 'x' || c == 'X') {
    Serial.print("X");
    morseX();
  } else if (c == 'y' || c == 'Y') {
    Serial.print("Y");
    morseY();
  } else if (c == 'z' || c == 'Z') {
    Serial.print("Z");
    morseZ();
  } else if (c == ';' || c == ':') {
    Serial.print("Ö");
    morseOO();
  } else if (c == '\'' || c == '"') {
    Serial.print("Ä");
    morseAA();
  } else if (c == '[' || c == '{') {
    Serial.print("Å");
    morseAAA();
  } else if (c == '1') {
    Serial.print("1");
    morse1();
  } else if (c == '2') {
    Serial.print("2");
    morse2();
  } else if (c == '3') {
    Serial.print("3");
    morse3();
  } else if (c == '4') {
    Serial.print("4");
    morse4();
  } else if (c == '5') {
    Serial.print("5");
    morse5();
  } else if (c == '6') {
    Serial.print("6");
    morse6();
  } else if (c == '7') {
    Serial.print("7");
    morse7();
  } else if (c == '8') {
    Serial.print("8");
    morse8();
  } else if (c == '9') {
    Serial.print("9");
    morse9();
  } else if (c == '0') {
    Serial.print("0");
    morse0();
  } else if (c == ' ') {
    Serial.print(" ");
    morseSpace();
  } else {
    Serial.print(c);
  }
}

void displayMorse(String letter, String morse) {
  //clear 2nd line
  for (int i = 0; i < 16; i++) {
    lcd.setCursor(i, 1);
    lcd.write(' ');
  }
  lcd.setCursor(0, 1);
  if (letter == "Ä") {
    lcd.write(byte(0));
    lcd.setCursor(1, 1);
    lcd.print("  " + morse);
  } else if (letter == "Å") {
    lcd.write(byte(1));
    lcd.setCursor(1, 1);
    lcd.print("  " + morse);
  } else if (letter == "Ö") {
    lcd.write(byte(2));
    lcd.setCursor(1, 1);
    lcd.print("  " + morse);
  } else {
    lcd.print(letter + "  " + morse);
  }
  //return back the blinking cursor
  lcd.setCursor(curCharMorse, 0);
}

void updateTimes() {
  dotLong = unitLong;
  dashLong = unitLong * 3;
  afterLetterLong = unitLong * 3;
  spaceLong = unitLong  * 7;
}

