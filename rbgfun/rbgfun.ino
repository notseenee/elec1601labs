int del = 10;
bool r_up = true;
bool g_up = true;
bool b_up = true;
int r_val = 128;
int g_val = 0;
int b_val = 0;

void setup() {
  pinMode(11, OUTPUT); //Red
  pinMode(10, OUTPUT); //Green
  pinMode(9, OUTPUT); //Blue
}
void loop() {
  // put your main code here, to run repeatedly:
  r_val = breathe(r_val, r_up, 1);
  r_up = check_flip(r_val, r_up);
  g_val = breathe(g_val, g_up, 0);
  g_up = check_flip(g_val, g_up);
  b_val = breathe(b_val, b_up, 1);
  b_up = check_flip(b_val, b_up);

  rgb(r_val, g_val, b_val);
}

void rgb(int red, int green, int blue) {
  analogWrite(11, red);
  analogWrite(10, green);
  analogWrite(9, blue);
  delay(del);
}

int breathe(int col, bool up, float val) {
  if (up) {
    return col+val;
  } else {
    return col-val;
  }
}

bool check_flip(int col, bool up) {
  if (col >= 255) {
    return false;
  } else if (col <= 0) {
    return true;
  } else {
    return up;
  }
}

