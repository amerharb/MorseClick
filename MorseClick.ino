
#include <PS2Keyboard.h>

const int DataPin = 4;
const int IRQpin =  3;

PS2Keyboard keyboard;

//this will determine the pase of morse code and fequancy
int unitLong = 55;
int dotLong = unitLong;
int dashLong = unitLong * 3;
int afterLetterLong = unitLong * 3;
int spaceLong = unitLong  * 7;
int freq = 450;

//pin of the buzzer that will beep the morse code
int buzzerPin = 9;
int ledPin = 13;

void setup() {
  delay(1000);
  keyboard.begin(DataPin, IRQpin);
  Serial.begin(9600);
  Serial.println("Keyboard Test:");

  pinMode (buzzerPin, OUTPUT) ;// set the digital IO pin mode, OUTPUT out of Wen
  pinMode(ledPin, OUTPUT);

}

void loop() {
  if (keyboard.available()) {

    // read the next key
    char c = keyboard.read();

    // check for some of the special keys
    if (c == PS2_ENTER) {
      Serial.println();
    } else if (c == 'a' || c == 'A') {
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

    } else if (c == PS2_TAB) {
      Serial.print("[Tab]");
    } else if (c == PS2_ESC) {
      Serial.print("[ESC]");
    } else if (c == PS2_PAGEDOWN) {
      Serial.print("[PgDn]");
    } else if (c == PS2_PAGEUP) {
      Serial.print("[PgUp]");
    } else if (c == PS2_LEFTARROW) {
      Serial.print("[Left]");
    } else if (c == PS2_RIGHTARROW) {
      Serial.print("[Right]");
    } else if (c == PS2_UPARROW) {
      Serial.print("[Up]");
    } else if (c == PS2_DOWNARROW) {
      Serial.print("[Down]");
    } else if (c == PS2_DELETE) {
      Serial.print("[Del]");
    } else {

      // otherwise, just print all normal characters
      Serial.print(c);
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
  morseDot();
  morseDash();
  moresAfterLetter();
}

void morseB() {
  //morse -...
  morseDash();
  morseDot();
  morseDot();
  morseDot();
  moresAfterLetter();
}

void morseC() {
  //morse -.-.
  morseDash();
  morseDot();
  morseDash();
  morseDot();
  moresAfterLetter();
}

void morseD() {
  //morse --.
  morseDash();
  morseDash();
  morseDot();
  moresAfterLetter();
}

void morseE() {
  //morse .
  morseDot();
  moresAfterLetter();
}

void morseF() {
  //morse ..-.
  morseDot();
  morseDot();
  morseDash();
  morseDot();
  moresAfterLetter();
}

void morseG() {
  //morse --.
  morseDash();
  morseDash();
  morseDot();
  moresAfterLetter();
}

void morseH() {
  //morse ....
  morseDot();
  morseDot();
  morseDot();
  morseDot();
  moresAfterLetter();
}

void morseI() {
  //morse ..
  morseDot();
  morseDot();
  moresAfterLetter();
}

void morseJ() {
  //morse .---
  morseDot();
  morseDash();
  morseDash();
  morseDash();
  moresAfterLetter();
}

void morseK() {
  //morse -.-
  morseDash();
  morseDot();
  morseDash();
  moresAfterLetter();
}

void morseL() {
  //morse .-..
  morseDot();
  morseDash();
  morseDot();
  morseDot();
  moresAfterLetter();
}

void morseM() {
  //morse --
  morseDash();
  morseDash();
  moresAfterLetter();
}

void morseN() {
  //morse -.
  morseDash();
  morseDot();
  moresAfterLetter();
}

void morseO() {
  //morse ---
  morseDash();
  morseDash();
  morseDash();
  moresAfterLetter();
}

void morseP() {
  //morse .--.
  morseDot();
  morseDash();
  morseDash();
  morseDot();
  moresAfterLetter();
}

void morseQ() {
  //morse --.-
  morseDash();
  morseDash();
  morseDot();
  morseDash();
  moresAfterLetter();
}

void morseR() {
  //morse .-.
  morseDot();
  morseDash();
  morseDot();
  moresAfterLetter();
}

void morseS() {
  //morse ...
  morseDot();
  morseDot();
  morseDot();
  moresAfterLetter();
}

void morseT() {
  //morse -
  morseDash();
  moresAfterLetter();
}

void morseU() {
  //morse ..-
  morseDot();
  morseDot();
  morseDash();
  moresAfterLetter();
}

void morseV() {
  //morse ...-
  morseDot();
  morseDot();
  morseDot();
  morseDash();
  moresAfterLetter();
}

void morseW() {
  //morse .--
  morseDot();
  morseDash();
  morseDash();
  moresAfterLetter();
}

void morseX() {
  //morse -..-
  morseDash();
  morseDot();
  morseDot();
  morseDash();
  moresAfterLetter();
}

void morseY() {
  //morse -.--
  morseDash();
  morseDot();
  morseDash();
  morseDash();
  moresAfterLetter();
}

void morseZ() {
  //morse --..
  morseDash();
  morseDash();
  morseDot();
  morseDot();
  moresAfterLetter();
}

void morseÖ() {
  //morse --..
  morseDash();
  morseDash();
  morseDot();
  morseDot();
  moresAfterLetter();
}

void morseÄ() {
  //morse --..
  morseDash();
  morseDash();
  morseDot();
  morseDot();
  moresAfterLetter();
}

void morse1() {
  //morse .----
  morseDot();
  morseDash();
  morseDash();
  morseDash();
  morseDash();
  moresAfterLetter();
}

void morse2() {
  //morse ..---
  morseDot();
  morseDot();
  morseDash();
  morseDash();
  morseDash();
  moresAfterLetter();
}

void morse3() {
  //morse ...--
  morseDot();
  morseDot();
  morseDot();
  morseDash();
  morseDash();
  moresAfterLetter();
}

void morse4() {
  //morse ....-
  morseDot();
  morseDot();
  morseDot();
  morseDot();
  morseDash();
  moresAfterLetter();
}

void morse5() {
  //morse .....
  morseDot();
  morseDot();
  morseDot();
  morseDot();
  morseDot();
  moresAfterLetter();
}

void morse6() {
  //morse -....
  morseDash();
  morseDot();
  morseDot();
  morseDot();
  morseDot();
  moresAfterLetter();
}

void morse7() {
  //morse --...
  morseDash();
  morseDash();
  morseDot();
  morseDot();
  morseDot();
  moresAfterLetter();
}

void morse8() {
  //morse ---..
  morseDash();
  morseDash();
  morseDash();
  morseDot();
  morseDot();
  moresAfterLetter();
}

void morse9() {
  //morse ----.
  morseDash();
  morseDash();
  morseDash();
  morseDash();
  morseDot();
  moresAfterLetter();
}

void morse0() {
  //morse -----
  morseDash();
  morseDash();
  morseDash();
  morseDash();
  morseDash();
  moresAfterLetter();
}


