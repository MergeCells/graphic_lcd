
# define RS 12
# define RW
# define E  13

# define DB0 2
# define DB1 3
# define DB2 4
# define DB3 5

# define DB4 6
# define DB5 7
# define DB6 8
# define DB7 9

# define CS1 10
# define CS2 11

# define RST


/* //////////
 * 
 * This sketch is for 'TG12864E' 
 * 
 * RW to GND
 * RST to Vcc
 * Vo is about -2.0V
 * 
 * 
*/ //////////


int aIn = 0;

byte image[2][8][64] = {0};

void setup() {

  pinMode(RS, OUTPUT);
  pinMode(E, OUTPUT);

  pinMode(DB0, OUTPUT);
  pinMode(DB1, OUTPUT);
  pinMode(DB2, OUTPUT);
  pinMode(DB3, OUTPUT);

  pinMode(DB4, OUTPUT);
  pinMode(DB5, OUTPUT);
  pinMode(DB6, OUTPUT);
  pinMode(DB7, OUTPUT);

  pinMode(CS1, OUTPUT);
  pinMode(CS2, OUTPUT);
  
  delayMicroseconds(1);
  digitalWrite(E, LOW);

  delay(50);

  // set Start Line to first
  //transmit(0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0);
  transmit(B0000,B11000000);

  // ON
  //transmit(0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1);
  transmit(B0011,B00111111);

  delay(1);

  // Z
  //transmit(0, 0, 1, 1, 1, 1, 0,0,0,0,0,0);
  transmit(B0011,B11000000);

  // X
  setX(0);

  //Y
  setY(0);

}

void loop() {
  for (byte x = 0; x < 8; x++) {
    setX(x);
    for (int y = 0; y < 64; y++) {
      //transmit(1, 0, 1, 0, (byte)random(0, 2), (byte)random(0, 2), (byte)random(0, 2), (byte)random(0, 2), (byte)random(0, 2), (byte)random(0, 2), (byte)random(0, 2), 1);
      //transmit(B1010, random(255));
      transmit(B1010, millis()%256);
    }
    for (int y = 0; y < 64; y++) {
      /*
      aIn = analogRead(A0)+1; // 0-1023
      aIn /= 32; // 0-32
      aIn -= 24; // 0-8
      aIn = B00000001 << aIn;
      transmit(1, 0, 0, 1, (B10000000 & aIn), (B01000000 & aIn), (B00100000 & aIn), (B00010000 & aIn), (B00001000 & aIn), (B00000100 & aIn), (B00000010 & aIn), (B00000001 & aIn));
      */
      transmit(B1001, analogRead(A0)/4);
    }
  }
  delay(1000-millis()%1000);
}


void setX(byte page) {
  //transmit(0, 0, 1, 1, 1, 0, 1, 1, 1, (B00000100 & page), (B00000010 & page), (B00000001 & page));
  transmit(B0011, (B10111000|page));
}


void setY(byte address) {
  //transmit(0, 0, 1, 1, 0, 1, (B00100000 & address), (B00010000 & address), (B00001000 & address), (B00000100 & address), (B00000010 & address), (B00000001 & address));
  transmit(B0011, (B01000000|address));
}




void transmit(byte RSdata, byte RWdata, byte CS1data, byte CS2data, byte DB7data, byte DB6data, byte DB5data, byte DB4data, byte DB3data, byte DB2data, byte DB1data, byte DB0data) {

  //delayMicroseconds(1);
  //digitalWrite(E, LOW);

  digitalWrite(RS, RSdata);
  //digitalWrite(RW, RWdata);
  digitalWrite(CS1, CS1data);
  digitalWrite(CS2, CS2data);

  //delayMicroseconds(1);
  digitalWrite(E, HIGH);

  digitalWrite(DB7, DB7data);
  digitalWrite(DB6, DB6data);
  digitalWrite(DB5, DB5data);
  digitalWrite(DB4, DB4data);

  digitalWrite(DB3, DB3data);
  digitalWrite(DB2, DB2data);
  digitalWrite(DB1, DB1data);
  digitalWrite(DB0, DB0data);

  //delayMicroseconds(1);
  digitalWrite(E, LOW);
}


void transmit(byte high_data,byte low_data) {

  //delayMicroseconds(1);
  //digitalWrite(E, LOW);

  digitalWrite(RS, bitRead(high_data,3));
  //digitalWrite(RW, bitRead(high_data,2));
  digitalWrite(CS1, bitRead(high_data,1));
  digitalWrite(CS2, bitRead(high_data,0));

  //delayMicroseconds(1);
  digitalWrite(E, HIGH);

  digitalWrite(DB7, bitRead(low_data,7));
  digitalWrite(DB6, bitRead(low_data,6));
  digitalWrite(DB5, bitRead(low_data,5));
  digitalWrite(DB4, bitRead(low_data,4));

  digitalWrite(DB3, bitRead(low_data,3));
  digitalWrite(DB2, bitRead(low_data,2));
  digitalWrite(DB1, bitRead(low_data,1));
  digitalWrite(DB0, bitRead(low_data,0));

  //delayMicroseconds(1);
  digitalWrite(E, LOW);
}





