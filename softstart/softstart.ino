float xheadpre = 0;
float sigmapre = 0;
float Q = 0.035;
float R = 0.95;
float Dnow = 0.5;
float Km = 220/1500;
float omegaref = 800;
float errorspeed = 0;
float currentTime;
float elapsedTime;
float cumErrorspeed = 0;
float Kpspeed = 0.015;
float Kispeed = 0.0155;
float Kpcurrent = 8;
float Kicurrent = 0.7;
float currentref;
float cumErrorcurrent = 0;
float errorcurrent = 0;
float Vout;
float previousTime;
const int PWM_pin = 9;
int softstart = 1;
const int Controller_pin = 6;
const int Ref_pin = 4;
int PWM;


void setup() {
  // put your setup code here, to run once:
    // Pins D9 and D10 - 2 kHz 10bit
  TCCR1A = 0b00000011; // 10bit
  TCCR1B = 0b00001010; // x8 fast pwm
  Serial.begin(9600);
  pinMode(Controller_pin, INPUT);
  pinMode(Ref_pin, INPUT);
}
void loop() {
   if(digitalRead(Controller_pin) == LOW){
    PWM = constrain(PWM,0, 600);
    if(PWM <= 512){
      delay(60);
      PWM = PWM + 1;}
      
    analogWrite(PWM_pin,PWM);
    Serial.println(PWM);
  }
  // put your main code here, to run repeatedly:
  else{
    if(digitalRead(Ref_pin) == LOW){
      omegaref = 600;
    }
    else{
      omegaref = 800;
    }
    int adc = analogRead(A1);
    float voltage = adc*5/1023.0;
    float current = (voltage-2.5)/0.066;
    float premeasure = xheadpre;
    float S = sigmapre + R;
    float K = sigmapre*(1/S);
    float xhead = xheadpre + K*(current-premeasure);
    float sigma = sigmapre-K*S*K;
    xheadpre = xhead;
    sigmapre = sigma + Q;
    
    float omegahead = (Dnow*120-xhead*0.8)/Km;
    errorspeed = omegaref - omegahead;
    
    currentTime = millis();                //get current time
    elapsedTime = (double)(currentTime - previousTime);  
    cumErrorspeed += errorspeed*elapsedTime;
    currentref = cumErrorspeed*Kispeed + Kpspeed*errorspeed;
    if(currentref >= 4){
      currentref = 4;
    }
    if(currentref <= 0){
      currentref = 0;
    }
    errorcurrent = currentref - xhead;
    cumErrorcurrent = errorcurrent*elapsedTime;
    Vout = cumErrorcurrent*Kicurrent + errorcurrent * Kpcurrent;
    if(Vout <= 0){
      Vout = 0;
    }
    if(Vout >= 110){
      Vout = 110;
    }
    Dnow = Vout/120;
    previousTime = currentTime;   
    int PWM = 1023/Dnow;
    analogWrite(PWM_pin,PWM);
    delay(50);
    Serial.print("Vout: ");
    Serial.println(Vout);
  }
}
