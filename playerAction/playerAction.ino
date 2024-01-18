void setup()
{
  DDRB = 255;
  DDRD = 255;
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, INPUT);
  // Serial.begin(115200);

  // Inputs:
  DDRC = 0;
  PORTC = 0; // Pull-ups off.  We have our own external pull-ups anyway.

  // ADC Setup

  PRR &= ~(_BV(ICF1)); // Allow ADC to be powered up

  // ADC 3-5
  ADMUX = 0 * 4;
  ADCSRA = 197; // Enable ADC & start, prescale at 32

  Serial.begin(115200);
}

void writey(uint8_t coord)
{
  PORTD = coord;
}

void writex(uint8_t num)
{
  if (num > 255)
  {
    num = 255;
  }

  // Step 1: Convert to binary
  String binary = String(num, BIN);

  // Step 2: Pad with zeros if necessary
  int bitAsInt[8]; // Declare the variable bitAsInt

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
    // For example, print it
    // Serial.print(bitAsInt[i]);
  }
  // Serial.println();
  digitalWrite(8, bitAsInt[0]);
  digitalWrite(9, bitAsInt[1]);
  digitalWrite(10, bitAsInt[2]);
  digitalWrite(11, bitAsInt[3]);
  digitalWrite(12, bitAsInt[4]);
  digitalWrite(13, bitAsInt[5]);
  digitalWrite(A2, bitAsInt[6]);
  digitalWrite(A3, bitAsInt[7]);
}

bool BPINC(uint8_t port)
{
  String binary = String(PINC, BIN);
  while (binary.length() < 8)
  {
    binary = "0" + binary;
  }
  bool portOut = binary.charAt(7 - port) - '0' == 1;

  return portOut;
}

void grid()
{

  writex(0);
  writey(0);
  writex(127);
  writey(0);
  writex(255);
  writey(0);
  writex(0);
  writey(127);
  writex(127);
  writey(127);
  writex(255);
  writey(127);
  writex(0);
  writey(255);
  writex(127);
  writey(255);
  writex(255);
  writey(255);
}

void loop()
{

  Serial.print(analogRead(A0) >> 4);
  Serial.print(" ");
  Serial.print(analogRead(A4)>>4);
  Serial.print(" ");
  Serial.print(BPINC(1));
  Serial.print(" ");
  Serial.println(BPINC(5));

}
