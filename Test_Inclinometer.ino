
double getInclination();

//yellow goes in analog 1 (A1)
//Red and green need to be connected to +5V and GND


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
  double inclination = getInclination();
  Serial.println("Inclination Reading:");
  Serial.println(inclination);
}

double getInclination()
{
    const int conductivityAnalogPin1 = 1;

    int reading;
    
    reading = analogRead(conductivityAnalogPin1);
    return reading;
//    double slope = 0.2390276187;
//    double inter = -108.2381394;
//    double angle = (slope * reading) - inter;
//
//    return angle;
}
