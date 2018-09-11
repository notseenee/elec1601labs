int lightOutput = 0;
boolean up = true;

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  lightOutput = analogRead(0);  
  if (lightOutput > 800) {
    siren();
  } else {
    noTone(9);
  }
  delay(10);
}

void siren() {
  if (up) {
    for (float i = 0; i < 1500; i += 0.4) {
      tone(9, 500 + i, 1000);
      up = false;
    }
  } else {
    for (float i = 1500; i > 0; i -= 0.4) {
      tone(9, 500 + i, 1000);
      up = true;
    }
  }
}

