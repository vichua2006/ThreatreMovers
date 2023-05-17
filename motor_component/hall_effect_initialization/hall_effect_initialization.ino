#define stepPin 2
#define dirPin 5
#define hallSensorPin 3

// pan motor model: kysan_1124090_7961
// data sheet in main folder
const int STEPS = 200;
int pan_position, tilt_position, home_pos;
 
void setup() {
  // output for debugging
  Serial.begin(9600);

  // Sets the motor pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);

  // enable pins for motor; must be at low
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  
  tilt_position = 0;

}

void loop() {

  pan_init();
  delay(1000);

}

// calibrates pan motor to 0 degrees 
void pan_init(){
  // set arbitrary rotation direction
  digitalWrite(dirPin, HIGH);

  // spins motor around until hall sensor detects magnet
  while (digitalRead(hallSensorPin)){
    digitalWrite(stepPin, HIGH); 
    delayMicroseconds(1000); 
    digitalWrite(stepPin, LOW); 
    delayMicroseconds(1000);
  }

  // set current position to 0
  pan_position = 0;
}

// turns the pan motor inc steps in direction dir and in time (rather inaccurate timing)
void pan_turn(int inc, bool dir, double time){
  // exit function if it doesn't need to turn
  if (inc == 0) return ; 

  // calculate delay
  double delay_per_step = max(time / (double) inc * 5e5, (double) 500);
  // delay per step in microseconds; 1 second = 1e6 microseconds, then divide by 2 since there are two delays to get 5e5
  // 500 microseconds is pretty much the limit; anything faster the motor doesn't pick up

  if (dir == 1) digitalWrite(dirPin, HIGH); // set rotation direction
  else digitalWrite(dirPin, LOW);

  for (int i=0;i<inc;i++){
    digitalWrite(stepPin, HIGH); 
    delayMicroseconds(delay_per_step);     // by changing this time delay between the steps we can change the rotation speed
    digitalWrite(stepPin, LOW); 
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
  
  pan_turn(inc, dir, time); // turn motor
  pan_position = step_position; // update global position variable
  Serial.println(step_position);  // debugging
}

