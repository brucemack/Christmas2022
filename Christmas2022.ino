// Christmas Light Controller
// 2022 Display
// Bruce MacKinnon 26-Nov-22
//
// Hardware is an Arduio Pro Mini, two 74HCT595 shift registers,
// and 16 solid-state relays. 
//
// GPIO pins for shift register.
// CHANGE THESE ACCORDING TO YOUR HARDWARE SETUP!
#define SRCLK 8
#define RCLK  7
#define SER   9

/* The display this year has red and green strings around 7 windows, a 
 * wreath with white lines, and white strings around both doors.
 */
#define TAL_GR     0x0001
#define TAL_RD     0x0002
#define TAM_GR     0x0004
#define TAM_RD     0x0008
#define TAR_GR     0x0010
#define TAR_RD     0x0020
#define BAL_GR     0x0040
#define BAL_RD     0x0080
#define BAR_GR     0x0100
#define BAR_RD     0x0200
#define TBL_GR     0x0400
#define TBL_RD     0x0800
#define TBR_GR     0x1000
#define TBR_RD     0x2000
#define WREATH     0x4000 // White
#define DOORS      0x8000 // White - Both doors on the same channel

void setup() {
  
  pinMode(13,OUTPUT);
  pinMode(SRCLK,OUTPUT);
  pinMode(RCLK,OUTPUT);
  pinMode(SER,OUTPUT);
  digitalWrite(SRCLK,0);
  digitalWrite(RCLK,0);

  int d0 = 1000;

  // Diagnostics on startup
  write(0);
  // Flash on-board LED
  digitalWrite(13,1);
  delay(d0);
  digitalWrite(13,0);
  delay(d0);
  digitalWrite(13,1);
  delay(d0);
  digitalWrite(13,0);
  delay(d0);
  delay(d0);
  
  // Turn on every string
  write(0xffff);
  delay(d0);
  delay(d0);
  delay(d0);
  // Turn off every string
  write(0);
  delay(d0);
  delay(d0);
  delay(d0);
}

// Single right-shift on shift register
void shift() {
  digitalWrite(SRCLK,1);
  delay(10);
  digitalWrite(SRCLK,0);
}

// Latch output for display
void latch() {
  digitalWrite(RCLK,1);
  digitalWrite(RCLK,0);
}

// Writes a 16-bit value into the register and latches
void write(unsigned int data) {
  for (int i = 0; i < 16; i++) {
    if (data & 1) {
      digitalWrite(SER,1);
    } else {
      digitalWrite(SER,0);
    }
    shift();
    data = data >> 1;
  }
  latch();
}

void showCycle() {

  int df = 75;
  int d0 = 125;
  int d1 = 250;
  int d2 = 500;
  
  write(0);
  delay(d1);

  // Long white
  write(DOORS | WREATH);
  delay(d2);
  delay(d2);
  delay(d2);
  // Long red
  write(TAL_RD | TAM_RD | TAR_RD | TBL_RD | TBR_RD | BAL_RD | BAR_RD);
  delay(d2);
  delay(d2);
  // Long green
  write(TAL_GR | TAM_GR | TAR_GR | TBL_GR | TBR_GR | BAL_GR | BAR_GR);
  delay(d2);
  delay(d2);

  // Red chase
  for (int i = 0; i < 2; i++) {
    write(TAL_RD);
    delay(df);
    write(TAM_RD);
    delay(df);
    write(TAR_RD);
    delay(df);
    write(TBL_RD);
    delay(df);
    write(TBR_RD);
    delay(df);
    write(BAR_RD);
    delay(df);
    write(BAL_RD);
    delay(df);
  }
  
  // Green chase
  for (int i = 0; i < 2; i++) {
    write(TAL_GR);
    delay(df);
    write(TAM_GR);
    delay(df);
    write(TAR_GR);
    delay(df);
    write(TBL_GR);
    delay(df);
    write(TBR_GR);
    delay(df);
    write(BAR_GR);
    delay(df);
    write(BAL_GR);
    delay(df);
  }

  // Red/green alt
  for (int i = 0; i < 4; i++) {
    write(TAL_RD | TAM_RD | TAR_RD | TBL_RD | TBR_RD | BAL_RD | BAR_RD);
    delay(df);
    write(TAL_GR | TAM_GR | TAR_GR | TBL_GR | TBR_GR | BAL_GR | BAR_GR);
    delay(df);
  }
  // Red/green alt
  for (int i = 0; i < 4; i++) {
    write(TAL_RD | TAM_GR | TAR_RD | TBL_GR | TBR_RD | BAL_GR | BAR_RD);
    delay(df);
    write(TAL_GR | TAM_RD | TAR_GR | TBL_RD | TBR_GR | BAL_RD | BAR_GR);
    delay(df);
  }    
  // Color/white alt
  for (int i = 0; i < 8; i++) {
    write(TAL_RD | TAM_RD | TAR_RD | TBL_RD | TBR_RD | BAL_RD | BAR_RD |
          TAL_GR | TAM_GR | TAR_GR | TBL_GR | TBR_GR | BAL_GR | BAR_GR |
          DOORS | WREATH);
    delay(df);
    write(DOORS | WREATH);
    delay(df);
  }    

  // Jumble
  unsigned int random1[] = { 0x1d34, 0x8b36, 0x1111, 0x2222, 0x3e3e, 0x8634,
    0x81cc, 0xa4f3 };
  for (int i = 0; i < 8; i++) {
    write(random1[i]);
    delay(50);
  }
}

void loop() {
  showCycle();
}

