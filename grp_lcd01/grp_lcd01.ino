
# define _ATmega168
// ポート操作を高速化するためのピン配列に従うときの定義
//

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

# define RS 12
# define RW
# define E  13

# define RST


/* //////////
 *
 * This sketch is for 'TG12864E'
 *
 * RW to GND
 * RST to Vcc
 * Vo is about -2.0V
 *
 * B0000, B0000 0000
 * RW, RS, CS2, CS1 ; DB7-0
 *
*/ //////////


//int aIn = 0;

byte image[2][8][64] = {0};

// 4byte (null,C,B,D)
unsigned long portState = 0;

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
  transmit(B0000, B11000000);

  // ON
  transmit(B0011, B00111111);

  delay(1);

  // Z
  transmit(B0011, B11000000);

  setX(0);
  setY(0);

  transmit(image);
  delay(2000);

  for (byte x = 0; x < 8; x++) {
    for (int y = 0; y < 64; y++) {
      // image[0][x][y] = ((x - 4) * (x - 4) + (y - 64) * (y - 64) / 64 > 10) ? random(256) : 0;
      image[0][x][y] = random(256);
    }
    for (int y = 0; y < 64; y++) {
      // image[1][x][y] = ((x - 4) * (x - 4) + y * y / 64 > 10) ? random(256) : 0;
      image[1][x][y] = random(256);
    }
  }
}

void loop() {
  /*
  for (byte x = 0; x < 8; x++) {
    setX(x);
    for (int y = 0; y < 64; y++) {
      //transmit(B1010, random(255));
      transmit(B1010, millis()%256);
    }
    for (int y = 0; y < 64; y++) {
      transmit(B1001, analogRead(A0)/4);
    }
  }
  */
  for(int i = 0; i < 31; i++){
    randomImage(i*2);
    transmit(image);
  }
  delay(1000);

  for(int j = 0; j < 10; j++){
    for (byte x = 0; x < 8; x++) {
      for (int y = 0; y < 64; y++) {
        image[0][x][y] = random(256);
      }
      for (int y = 0; y < 64; y++) {
        image[1][x][y] = ~image[0][x][y];
      }
    }
    for (int k = 0; k < 100; k++) {
      transmit(image);
      transmit2(image);
    }
    delay(1000);
  }
  //delay(1005 - millis() % 1000);
}

void randomImage(int radius) {
  
  for (byte x = 0; x < 8; x++) {
    for (int y = 0; y < 64; y++) {
      // image[0][x][y] = ((x - 4) * (x - 4) + (y - 64) * (y - 64) / 64 > 10) ? random(256) : 0;
      image[0][x][y] = random(256);
    }
    for (int y = 0; y < 64; y++) {
      // image[1][x][y] = ((x - 4) * (x - 4) + y * y / 64 > 10) ? random(256) : 0;
      image[1][x][y] = ~image[0][x][y];
      // image[1][x][y] = random(256);
    }
  }

  for (byte cs = 0; cs < 2; cs++) {
    for (byte x = 0; x < 8; x++) {
      for (byte y = 0; y < 64; y++) {
        for (byte z = 0; z < 8; z++) {
          ( pow((y + cs * 64 - 64), 2) * 0.67 + pow((x * 8 + z - 32), 2) < pow(radius, 2))  ? bitClear(image[cs][x][y], z) : 0;
          // (32 / 39)^2 = 0.67324
        }
      }
    }
  }
  image[1][7][60] &= B01111111;
  image[1][7][61] &= B01111111;
  image[1][7][62] &= B01111111;
  image[1][7][63] &= B01111111;

}


void setX(byte page) {
  transmit(B0011, (B10111000 | page));
}


void setY(byte address) {
  transmit(B0011, (B01000000 | address));
}


void transmit(byte array[2][8][64]) {
  setY(0);
  for (byte x = 0; x < 8; x++) {
    setX(x);
    for (int y = 0; y < 64; y++) {
      transmit(B0101, array[0][x][y]);
      transmit(B0110, array[1][x][y]);
    }
  }
}

void transmit2(byte array[2][8][64]) {
  setY(0);
  for (byte x = 0; x < 8; x++) {
    setX(x);
    for (int y = 0; y < 64; y++) {
      transmit(B0101, array[1][x][y]);
      transmit(B0110, array[0][x][y]);
    }
  }
}


void transmit(byte high_data, byte low_data) {

# ifdef _ATmega168

  portState = 0 | high_data << 10 | low_data << 2;
  PORTB = highByte(portState) & B0011111;
  PORTD = lowByte(portState);
  digitalWrite(E, HIGH);
  digitalWrite(E, LOW);

# else
  //delayMicroseconds(1);
  //digitalWrite(E, LOW);

  //digitalWrite(RW, bitRead(high_data, 3));
  digitalWrite(RS, bitRead(high_data, 2));
  digitalWrite(CS2, bitRead(high_data, 1));
  digitalWrite(CS1, bitRead(high_data, 0));

  //delayMicroseconds(1);
  digitalWrite(E, HIGH);

  digitalWrite(DB7, bitRead(low_data, 7));
  digitalWrite(DB6, bitRead(low_data, 6));
  digitalWrite(DB5, bitRead(low_data, 5));
  digitalWrite(DB4, bitRead(low_data, 4));

  digitalWrite(DB3, bitRead(low_data, 3));
  digitalWrite(DB2, bitRead(low_data, 2));
  digitalWrite(DB1, bitRead(low_data, 1));
  digitalWrite(DB0, bitRead(low_data, 0));

  //delayMicroseconds(1);
  digitalWrite(E, LOW);

# endif
}


void transmit(byte RWdata, byte RSdata, byte CS2data, byte CS1data, byte DB7data, byte DB6data, byte DB5data, byte DB4data, byte DB3data, byte DB2data, byte DB1data, byte DB0data) {

  //delayMicroseconds(1);
  //digitalWrite(E, LOW);

  //digitalWrite(RW, RWdata);
  digitalWrite(RS, RSdata);
  digitalWrite(CS2, CS2data);
  digitalWrite(CS1, CS1data);

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
