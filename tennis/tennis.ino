#include <avr/io.h>
#include <math.h>
#include <stdlib.h> //gives rand() function

#define g 9.8 // gravitational acceleration (should be positive.)

#define ts 0.5 // TimeStep

#define historyLength 0

void drawGroundAndNet()
{
  // Draw Ground and Net
  int k = 0;
  int m = 0;
  // while (k < 20) {
  while (k < 3)
  { // draws the ground and net multiple times per cycle. Increase to slow down, decrease to speed up.
    k++;
    m = 0;

    // Draw the left side of the ground (x=0 ~ x=127)
    while (m < 127)
    {
      writey(0);
      writex(m);
      m++;
    }

    // Draw the Net (x=127, y=0 ~ y=60)
    writex(127);
    m = 0;
    while (m < 61)
    {
      writey(m);
      m += 2;
    }
    while (m > 1)
    {
      writey(m);
      m -= 2;
    }

    writey(0);
    writex(127);
    
    // Draw the right side of the ground (x=127 ~ x=255)
    m = 127;
    while (m < 255)
    {
      writey(0);
      writex(m);
      m++;
    }
  }
}

bool getPlayerInput(uint8_t port)
{ // Read the player input from A0~A5
  String binary = String(PINC, BIN); // analogRead(A0-A5)
  while (binary.length() < 8) // Pad with zeros if necessary
  {
    binary = "0" + binary;
  }
  bool portOut = binary.charAt(7 - port) - '0' == 1;

  return portOut;
}

void writey(uint8_t coord)
{// Set the output to DAC
  PORTD = coord;
}

void writex(uint8_t coord)
{// Set the output to DAC

  // Step 1: Convert to binary
  String binary = String(coord, BIN);

  // Step 2: Pad with zeros if necessary
  int bitAsInt[8];
  while (binary.length() < 8)
  {
    binary = "0" + binary;
  }

  // Step 3: Split into 8 numbers
  for (int i = 0; i < 8; ++i)
  {
    char bit = binary.charAt(i);
    // Convert char to int and do something with it
    bitAsInt[i] = bit - '0';                     // Assign the value to bitAsInt array
    bitAsInt[i] = bitAsInt[i] == 1 ? HIGH : LOW; // Convert 1 to HIGH and 0 to LOW
  }

  // Step 4: Write to corresponding ports
  digitalWrite(8, bitAsInt[0]);
  digitalWrite(9, bitAsInt[1]);
  digitalWrite(10, bitAsInt[2]);
  digitalWrite(11, bitAsInt[3]);
  digitalWrite(12, bitAsInt[4]);
  digitalWrite(13, bitAsInt[5]);
  digitalWrite(A2, bitAsInt[6]);
  digitalWrite(A3, bitAsInt[7]);
}

void setup()
{
  Serial.begin(250000);
  float sintable[64];
  float costable[64];

  uint8_t xOldList[historyLength];
  uint8_t yOldList[historyLength];

  float xOld = 0;  // a few x & y position values
  float yOld = 64; // a few x & y position values
  float VxOld = 0; //  x & y velocity values
  float VyOld = 0; //  x & y velocity values

  float Xnew, Ynew, VxNew, VyNew;

  uint8_t deadball = 0;
  uint8_t Langle, Rangle;
  uint8_t xp = 0;
  uint8_t yp = 0;

  unsigned int ADoutTemp;

  uint8_t NewBall = 101;

  unsigned int NewBallDelay = 0;

  // Dummy variables:
  uint8_t k = 0;
  uint8_t m = 0;

  uint8_t Serving = 0;

  uint8_t ballside;
  uint8_t Lused = 0;
  uint8_t Rused = 0;

  bool leftButtonPressed = false;
  bool rightButtonPressed = false;

  // Create trig look-up table to keep things snappy.
  // 64 steps, total range: near pi.  Maybe a little more.

  m = 0;
  while (m < 64)
  {
    // sintable[m] = sin((float)0.0647 * (float)m - (float)2.07);
    // costable[m] = cos((float)0.0647 * (float)m - (float)2.07);
    sintable[m] = sin((float)0.0246 * (float)m);
    costable[m] = cos((float)0.0246 * (float)m);
    m++;
  }

  // OUTPUTS:
  for (byte i = 0; i < 14; i++)
  {
    pinMode(i, OUTPUT); // set digital pins 0-7 as outputs
  }
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);

  // INPUTS:
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  pinMode(A4, INPUT);
  pinMode(A5, INPUT);

  ballside = 0;

  // ---------
  // MAIN LOOP
  // ---------

  for (;;)
  {

    // input detection
    leftButtonPressed = !getPlayerInput(1);
    rightButtonPressed = !getPlayerInput(5);
    Langle = analogRead(A0) >> 4; // rescalled from 0-1023 to 0-63
    Rangle = analogRead(A4) >> 4;

    // Sync ballside variable with xOld
    if (ballside != (xOld >= 127))
    {
      ballside = (xOld >= 127);
      if (ballside) // lock or unlock the left or right button
        Rused = 0;
      else
        Lused = 0;
    }

    drawGroundAndNet();

    // If ball has run out of energy, make a new ball!
    if (NewBall > 10)
    {
      NewBall = 0;
      deadball = 0;
      NewBallDelay = 1;
      Serving = (ballside == 0); // the opposite of ballside

      if (Serving) // serving from the left
      {
        xOld = (float)230;
        VxOld = 0;
        ballside = 1;
        Rused = 0;
        Lused = 1;
      }
      else // serving from the right
      {
        xOld = (float)25;
        VxOld = 0;
        ballside = 0;
        Rused = 1;
        Lused = 0;
      }

      yOld = (float)110;

      m = 0;
      while (m < historyLength)
      {
        xOldList[m] = xOld;
        yOldList[m] = yOld;
        m++;
      }
    }

    if (NewBallDelay)
    {
      if ((getPlayerInput(1) == 0) || (getPlayerInput(5) == 0)) // 2U = 00000010 (2nd C port pin), 32U = 00100000 (5th C port pin)
        NewBallDelay = 10000;

      NewBallDelay++;

      if (NewBallDelay > 5000)
        NewBallDelay = 0;

      VxNew = VxOld;
      VyNew = VyOld;
      Xnew = xOld;
      Ynew = yOld;
    }
    else
    {
      // Move the ball
      Xnew = xOld + VxOld * ts /*15*/;
      Ynew = yOld + VyOld - 0.5 * g * ts * ts /**/;

      // Update velocity by acceleration
      VyNew = VyOld - g * ts;
      VxNew = VxOld;

      // Bounce at walls
      if (Xnew < 0)
      {
        Xnew = 0;
        VxNew = -VxNew;
        // VxNew = 0;
      }
      else if (Xnew > 254)
      {
        Xnew = 254;
        VxNew = -VxNew;
        // VxNew = 0;
      }

      // Bounce at ground
      if (Ynew <= 0)
      {
        Ynew = 0;

        // if (VyNew * VyNew < 10)
        //   NewBall++;
        NewBall++;

        if (VyNew < 0)
          VyNew *= -0.75;
      }

      // Bounce at ceiling
      if (Ynew >= 255)
      {
        Ynew = 255;
        VyNew *= -1;
      }

      // Bounce at net
      if (ballside)
      {
        if (Xnew < 127)
        {
          if (Ynew <= 63)
          {
            VxNew *= -0.5;
            VyNew *= 0.5;
            Xnew = 128.00;
            deadball = 1;
          }
        }
      }
      if (ballside == 0)
      {
        if (Xnew > 127)
        {
          if (Ynew <= 63)
          {
            // Bounce off of net
            VxNew *= -0.5;
            VyNew *= 0.5;
            Xnew = 126.00;
            deadball = 1;
          }
        }
      }

      // Simple routine to detect button presses
      if (/*xOld < 120*/ !ballside)
      {
        if (leftButtonPressed)
        {
          if ((Lused == 0) && (deadball == 0))
          {
            // VxNew = 1.5 * g * costable[Langle];
            // VyNew = g + 1.5 * g * sintable[Langle];
            VxNew = 40 * costable[Langle];
            VyNew = 40 * sintable[Langle];
            Lused = 1;
            NewBall = 0;
          }
        }
      }
      else if (/*xOld > 134*/ ballside)
      {
        if (rightButtonPressed)
        {
          if ((Rused == 0) && (deadball == 0))
          {
            // VxNew = -1.5 * g * costable[Rangle];
            // VyNew = g + -1.5 * g * sintable[Rangle];
            VxNew = -40 * costable[Rangle];
            VyNew = 40 * sintable[Rangle];
            Rused = 1;
            NewBall = 0;
          }
        }
      }
    }
    // Figure out which point we're going to draw.
    xp = (int)floor(Xnew);
    yp = (int)floor(Ynew);

    // Draw the ball
    writex(xp);
    writey(yp);

    // Age variables for the next iteration
    VxOld = VxNew;
    VyOld = VyNew;
    xOld = Xnew;
    yOld = Ynew;
  }
}

void loop() {}