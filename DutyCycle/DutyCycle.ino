/* PP0
 First version of code for LED lighting on the pyramid of possibilities.
 
 Using MMA8452Q accelerometer and neopixel lighting.
 
 Based on MMA8452Q Example Code by: Jim Lindblom
 
 Note: The MMA8452 is an I2C sensor, however this code does
 not make use of the Arduino Wire library. Because the sensor
 is not 5V tolerant, we can't use the internal pull-ups used
 by the Wire library. Instead use the included i2c.h, defs.h and types.h files.
 */

#include <Adafruit_NeoPixel.h>

#define MARK2 1

// Pin definitions
#if MARK2
#define PIN 22
int int1Pin = 17;  // These can be changed, 2 and 3 are the Arduinos ext int pins
int int2Pin = 16;
#else
#define PIN 2
int int1Pin = 20;  // These can be changed, 2 and 3 are the Arduinos ext int pins
int int2Pin = 21;
#endif

#define LEDs 240
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDs, PIN, NEO_GRB + NEO_KHZ800);

#define brightness 80
// Set the scale below either 2, 4 or 8


const int DELAY = 0;

uint32_t RED = strip.Color(brightness, 0, 0);
uint32_t GREEN = strip.Color(0, brightness, 0);
uint32_t BLUE = strip.Color(0, 0, brightness);
uint32_t WHITE = strip.Color(brightness/3, brightness/3, brightness/3);
uint32_t YELLOW = strip.Color(brightness/2, brightness/2, 0);
uint32_t ORANGE = strip.Color(2*brightness/3, brightness/3, 0);
uint32_t PURPLE = strip.Color(brightness/2, 0, brightness/2);
uint32_t BLACK = strip.Color(0, 0, 0);
uint32_t GREY = strip.Color(brightness/2/3, brightness/2/3, brightness/2/3);
uint32_t ALL = strip.Color(brightness, brightness, brightness);

unsigned long nextUpdate;

void p(char *fmt, ... ){
  char tmp[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(tmp, 128, fmt, args);
  va_end (args);
  Serial.print(tmp);
}
long refreshes = 0;
long start;



void setup()
{

  Serial.begin(115200);
  Serial.println("Hello");
  strip.begin();
  strip.show();
  pinMode(13, OUTPUT); 
  digitalWrite(13, HIGH); 

  delay(5000);

  digitalWrite(13, LOW); 
  start = millis();
  nextUpdate = start + 1000;


  p("Starting duty cycle test %d ms delay\n", DELAY);
}


int wrap(int led) {
  return led % LEDs;
}


void comet(int pos) {
  strip.setPixelColor(wrap(pos), RED);
  //  strip.setPixelColor(wrap(pos-1), ORANGE);
  //  strip.setPixelColor(wrap(pos-2), YELLOW);
}

void black(int pos) {
  strip.setPixelColor(wrap(pos), BLACK);
  strip.setPixelColor(wrap(pos-1), BLACK);
  strip.setPixelColor(wrap(pos-2), BLACK);
}

void loop()
{  
  int c = refreshes;

  for(int i = 0; i < 12; i++) {
    if (c&1)
      strip.setPixelColor(i, ALL);
    else 
     strip.setPixelColor(i, BLACK);
     c>>=1;
  }

  strip.show();
  refreshes++;
  long now = millis();
  if (now > nextUpdate) {
    p("%6dms, %3d Hz\n", now-start, refreshes * 1000 / (now-start));
    nextUpdate = nextUpdate + 10000;
  }
  delay(DELAY);
  

}

uint32_t color(float r, float g, float b) {
  return strip.Color((int)(255*r), (int) (255*g), (int)(255*b));
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t HSVtoRGB(  float h, float s, float v )
{
  int i;
  float r,g,b;
  float f, p, q, t;
  if( s == 0 ) {
    // achromatic (grey)
    return color(v,v,v);
  }
  h /= 60;			// sector 0 to 5
  i = floor( h );
  f = h - i;			// factorial part of h
  p = v * ( 1 - s );
  q = v * ( 1 - s * f );
  t = v * ( 1 - s * ( 1 - f ) );
  switch( i ) {
  case 0:
    return color(v,t,p);

  case 1:
    return color(q,v,p);

  case 2:
    return color(p,v,t);

  case 3:
    return color(p,q,v);

  case 4:
    return color(t,p,v);

  case 5:
  default:
    return color(v,p,q);
  }
}

















