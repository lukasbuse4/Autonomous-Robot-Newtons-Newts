// This file works, found it on the web.
// Connect the board to both your computer and the circuit
// Open in Arduino IDE as it is easiest to upload onto the board from there
// Press save then Upload to board, should be second button in top left.
// After uploading to board please reset board
// Turn power on in the circuit and watch the magic happen

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

void setup() 
{
  pinMode(10, OUTPUT); // Sets the trigPin(11) as an OUTPUT
  pinMode(11, INPUT); // Sets the echoPin(10) as an INPUT

  //need to add back sensor with different pins (basically uncomment below and change 10/11)
  pinMode(31, OUTPUT); // Sets the trigPin(31) as an OUTPUT
  pinMode(33, INPUT); // Sets the echoPin(33) as an INPUT

  Serial.begin(9600);

}
void loop()
{
  //Back Ping
  Serial.println("Back Ping");
  distance = Get_Distance(31, 33);
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(1000);

  //Front Ping
  Serial.println("Front Ping");
  distance = Get_Distance(10, 11);
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(1000);
}


int Get_Distance(int trigPin, int echoPin)
{
    //pulled this from online. It works
    int distance;
    // Clears the trigPin condition
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
    return distance;
}
