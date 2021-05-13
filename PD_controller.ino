// DECLARE motor pins
const int left_slp_pin = 31;
const int left_dir_pin = 29;
const int left_pwm_pin = 40;
const int right_slp_pin = 11;
const int right_dir_pin = 30;
const int right_pwm_pin = 39;



// DECLARE reflectance pins
int pin_ref[8] = {65, 48, 64, 47, 52, 68, 53, 69};

// DECLARE LED pins
const int even_led = 45;
const int odd_led = 61;

// DECLARE led weights
const float sensor_weights[] = {-1.75, -1.25, -.75, -0.45, 0.45, 0.75, 1.25, 1.75};

// DECLARE states
bool reached_black_line = false; // indicates when a horizontal black line has been reached
bool reached_first_line = false; // indicates when one end of the track has been reached

// DECLARE speed values
const float right_motor = 105; //105
const float left_motor = 105; //105

const float Kp = 28; //30
const float Kd = 50; //200
const float turn_speed = 85;//75

float prev_error;

// declare LED values
int ref_LED[8];


void setup() {

  
  pinMode(left_slp_pin, OUTPUT);
  pinMode(left_dir_pin, OUTPUT);
  pinMode(left_pwm_pin, OUTPUT);
  pinMode(right_slp_pin, OUTPUT);
  pinMode(right_dir_pin, OUTPUT);
  pinMode(right_pwm_pin, OUTPUT);


  pinMode(odd_led, OUTPUT);
  pinMode(even_led, OUTPUT);
  digitalWrite(odd_led, HIGH);
  digitalWrite(even_led, HIGH);
  
  digitalWrite(left_dir_pin, LOW);
  digitalWrite(right_dir_pin, LOW);
  digitalWrite(left_slp_pin, HIGH);
  digitalWrite(right_slp_pin, HIGH);
  

  prev_error = 0;
}

void loop() {
  read_sensor_values();
  
  float error = 0;
  for (int i = 0; i < 8; i++) {
    error += (ref_LED[i] * sensor_weights[i]); 
  }

  
  reached_black_line = all_black(ref_LED);

  if (reached_black_line == true) {
    if (reached_first_line == true) {
      analogWrite(left_pwm_pin, 0);
      analogWrite(right_pwm_pin, 0);
      while(true) {}
    }
    else {
      reached_black_line = false;
      reached_first_line = true;
      turn_around(); 
    }
  }

   float motorspeed = Kp * error + Kd*(error-prev_error);

   make_turn(motorspeed);
   prev_error = error;
}

void read_sensor_values() {
  for (int i = 0; i < 8; i++) {
    ref_LED[i] = get_sensor_value(pin_ref[i], 700);
  }
}


int get_sensor_value(int pin, int delay_length) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  
  delayMicroseconds(10);
  pinMode(pin, INPUT);
  delayMicroseconds(delay_length);
  int result = digitalRead(pin);
  return result;
}




int all_black (int arr[]) {
  int result = true;
  for (int i = 0; i < 8; i++) {
    if (arr[i] == 0) {
      result = false;
    }
  }
  return result;
}

void make_turn (int motorspeed) {
      analogWrite(left_pwm_pin, turn_speed-motorspeed);
      analogWrite(right_pwm_pin,turn_speed+motorspeed);
}

void turn_around() {
  
    digitalWrite(right_dir_pin, LOW);


    digitalWrite(left_dir_pin, HIGH);
    
    analogWrite(left_pwm_pin, 175);

    
    delay(520);
    
    digitalWrite(left_dir_pin, LOW);
    digitalWrite(right_dir_pin, LOW);
    
    analogWrite(left_pwm_pin, right_motor);
    analogWrite(right_pwm_pin, left_motor);
    
    return;
}
