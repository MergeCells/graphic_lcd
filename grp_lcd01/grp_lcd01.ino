
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

  for (byte cs = 0; cs < 2; cs++) {
    for (byte x = 0; x < 8; x++) {
      for (byte y = 0; y < 64; y++) {
        for (byte z = 0; z < 8; z++) {
          ( pow((y + cs*64 - 64), 2)+pow((x*8 + z - 32), 2) < 500)  ? bitClear(image[cs][x][y],z) : 0;
        }
      }
    }
  }
  image[1][7][60] &= B01111111;
  image[1][7][61] &= B01111111;
  image[1][7][62] &= B01111111;
  image[1][7][63] &= B01111111;
  transmit(image);

  delay(1005 - millis() % 1000);
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
      transmit(B1010, array[0][x][y]);
      transmit(B1001, array[1][x][y]);
    }
  }
}


void transmit(byte high_data, byte low_data) {

  //delayMicroseconds(1);
  //digitalWrite(E, LOW);

  digitalWrite(RS, bitRead(high_data, 3));
  //digitalWrite(RW, bitRead(high_data,2));
  digitalWrite(CS1, bitRead(high_data, 1));
  digitalWrite(CS2, bitRead(high_data, 0));

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
