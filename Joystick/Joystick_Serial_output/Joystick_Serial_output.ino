int JoyStick_X = A0; // x
int JoyStick_Y = A1; // y
int JoyStick_Z = 12; // key

void setup()
{
  pinMode (JoyStick_X, INPUT);
  pinMode (JoyStick_Y, INPUT);
  pinMode (JoyStick_Z, OUTPUT);
  Serial.begin (9600);
}

void loop()
{
  int x, y, z;
  x = analogRead (JoyStick_X);
  y = analogRead (JoyStick_Y);
  z = digitalRead (JoyStick_Z);
  Serial.print (x, DEC);
  Serial.print (",");
  Serial.print (y, DEC);
  Serial.print (",");
  Serial.println (z, DEC);
  delay (100);
}

