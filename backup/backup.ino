const int PWM_pin = 9;
const int period = 250;
int softstart = 1;
int speedpot = 1;
int PWM = 0;
int midpoint = 0;
int sensor_temp = 0;
int PWM_temp = 0;
int sensorvalue = 0;

void setup() {
  // Pins D9 and D10 - 2 kHz 10bit
  TCCR1A = 0b00000011; // 10bit
  TCCR1B = 0b00001010; // x8 fast pwm
  Serial.begin(9600);
}
void loop(){
  if(softstart == 1 ){
    PWM = constrain(PWM,0, 600);
    if(PWM <= 300){
      delay(200);
      PWM = PWM + 5;
      analogWrite(PWM_pin,PWM);
      Serial.println(PWM);
    }
    else{
      softstart = 0;
      PWM_temp = PWM;
    }
  }
  else{
    if(speedpot == 1){
      midpoint = analogRead(A0);
      speedpot = 0;
    }
    else{
      Serial.println(PWM);
      if(abs(sensorvalue-sensor_temp) >= 3){
        PWM = PWM_temp + (sensorvalue - midpoint);
      }
      if(PWM <= 20){
        PWM = 0;
      }
      else if(PWM >= 1000){
        PWM = 1000;
      }
      analogWrite(PWM_pin,PWM);
      sensor_temp = sensorvalue;
    }
  }
}
