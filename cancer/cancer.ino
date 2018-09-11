long randNumber; //establishes randNumber
long randNumber1; //establishes randNumber1

void setup ()
{
}

void loop ()
{
  randNumber = random(500, 3000); //randNumber is between .5 and 3 seconds
  tone(9, 10000, 1000); //plays a 18kHz tone on digital I/O pin 13
  delay(1000); //continues to play tone to the time of randNumber
  noTone(9); //stops playing tone on pin 13
}
