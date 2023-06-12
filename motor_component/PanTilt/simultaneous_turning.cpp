const int stepPin = 3; 
const int dirPin = 6; 
const int stepPin2 = 2; 
const int dirPin2 = 5; 
int time = 650;
String line = "";  // Stores the received line
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(stepPin2,OUTPUT); 
  pinMode(dirPin2,OUTPUT);
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
}
void loop() {
  if (Serial.available()) {
    while (Serial.available()) {
      char c = Serial.read();  // Read the next character
      if (c == '\n' || c == '\r') {
        Serial.println(line);
        time = line.toInt();
        line = "";
        Serial.println(line);
        break;
      }
      line += c;
    }
  }
  Serial.println(time);
  int deg = 60;
  int speed = 50;
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  digitalWrite(dirPin2,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < ((80/17)*deg); x++) {
    digitalWrite(stepPin,HIGH); 
    digitalWrite(stepPin2,HIGH); 
    delayMicroseconds(time); 
    digitalWrite(stepPin,LOW); 
    digitalWrite(stepPin2,LOW); 
    delayMicroseconds(time); 
  }
  delay(1000); // One second delay
  
  digitalWrite(dirPin,LOW); //Changes the rotations direction
  digitalWrite(dirPin2,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < ((80/17)*deg); x++) {
    digitalWrite(stepPin,HIGH);
    digitalWrite(stepPin2,HIGH);
    delayMicroseconds(time);
    digitalWrite(stepPin,LOW);
    digitalWrite(stepPin2,LOW);
    delayMicroseconds(time);
  }
  delay(1000);
}