#define stepPin 2
#define dirPin 5

//
const int STEPS = 200;
int pan_position, tilt_position, home_pos;
 
void setup() {
  Serial.begin(9600);
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);


  pan_position = 0;
  tilt_position = 0;
}
void loop() {
  delay(2000);
  pan_to(90, 0.1);
  delay(500);
  pan_to(180, 0.1);
  delay(500);
  pan_to(270, 0.1);
  delay(500);
  pan_to(360, 0.1);
  delay(500);
  pan_to(270, 0.1);
  delay(500);
  pan_to(180, 0.1);
  delay(500);
  pan_to(90, 0.1);
  delay(500);
  pan_to(0, 0.1);
}

void pan_turn(int inc, bool dir, double time){
  if (inc == 0) return ; // do nothing if it doesn't need to turn
  double delay_per_step = max(time / (double) inc * 5e5, (double) 500);
  // delay per step in microseconds; 1 second = 1e6 microseconds, then divide by 2 since there are two delays to get 5e5
  // 500 microseconds is pretty much the limit; anything faster the motor doesn't pick up
  if (dir == 1) digitalWrite(dirPin, HIGH); // set rotation direction
  else digitalWrite(dirPin, LOW);
  for (int i=0;i<inc;i++){
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delay_per_step);    // by changing this time delay between the steps we can change the rotation speed
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delay_per_step); 
  }
}

int deg_to_step(double deg){
  return deg / 360 * STEPS;
}

void pan_to(double angular_position, double time){
  // turns the pan motor to position within some time
  int step_position = deg_to_step(angular_position);
  int new_position = (step_position + (STEPS - pan_position)) % STEPS; // adjusts the position relative to the current pan_position; pan_position is at 0
  bool dir = (new_position <= STEPS / 2 ? 1 : 0); // 1 is cw, 0 ccw
  int inc = min(abs(STEPS - new_position), new_position); // minimum number of steps to get to position
  
  pan_turn(inc, dir, time);
  pan_position = step_position;
  Serial.println(step_position);  
}

