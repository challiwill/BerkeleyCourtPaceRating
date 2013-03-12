// BERKELEY CPR DEVICE
// corbin halliwill

// There is lots of code that is commented out. Either these are notes,
// or this is because we have set up the option to run the code without
// a laptop powering the Arduino (Using a button to control sensor
// calibration and an LCD for display). If you want to do this, un-
// comment the appropriate code. This option has not been fully 
// troubleshooted so please view the commented out code as more of 
// a suggestion.

// digital pin number for button
//const int buttonPin = 10;

// analog ambient light sensor pin numbers
const int lightpinI = 0;
const int lightpinO = 1;

const int lightpinA = 2;
const int lightpinB = 3;
const int lightpinC = 4;
const int lightpinD = 5;

// constants to be set
// CHANGE THESE TO BE APPROPRIATE TO YOUR DEVICE
const float sidistance = 15.0; // cm apart for IN sensors
const float xfdistance = 23.8; // cm apart from each other X OUT sensors
const float sodistance = 24.5; // cm from one slant sensor to another (measured perpendicular to the angle of the lasers)
const float iangle = 16; // initial angle of firing the ball
const float T = 20; // deg C of tennis ball surface 
const double pi = 3.14159265359;

void setup() {
  Serial.begin(9600);
 // pinMode(buttonPin, INPUT);   
}

void loop() {
  // read the state of the push button
 // buttonState = digitalRead(buttonPin);
  // hit go button to start loop
//  if (buttonState == HIGH) {  
  // declare time variables
  unsigned long timeInSlanti,timeOutSlanti,timeInXf,timeInSlantf,timeOutXf,timeOutSlantf;

    // GATHER DATA

      // callibrate out light sensors to local light
      int ilightI = analogRead(lightpinI) - 100;
      int ilightO = analogRead(lightpinO) - 100;
      int ilightA = analogRead(lightpinA) - 100;
      int ilightB = analogRead(lightpinB) - 100;
      int ilightC = analogRead(lightpinC) - 100;
      int ilightD = analogRead(lightpinD) - 100;
      
      // Indicate ready to shoot ball
      Serial.println("Fire when ready!");
      
      // SENSORS
      // each sensor loops until ball passes
      //use 'ilight*' where * is I,O,A,B,C, or D for more accurate reading
      
      // In Sensors 
      while ( analogRead(lightpinI) > ilightI) {
        timeInSlanti = micros();
      }      
      while ( analogRead(lightpinO) > ilightO) {
        timeOutSlanti = micros();
      }
            
      // Out Sensors
      while ( analogRead(lightpinA) > ilightA) { 
        timeInXf = micros();
      }
      while ( analogRead(lightpinB) > ilightB ) {
        timeInSlantf = micros();
      } 
      while ( analogRead(lightpinC) > ilightC ) {
        timeOutXf = micros();
      } 
      while ( analogRead(lightpinD) > ilightD ) {
        timeOutSlantf = micros();
      }
   
  // MAKE CALCULATIONS
      
      // compute velocities in the x and y IN directions      
      // we must convert cm to m and microseconds to seconds
      double vix = 10000*(sidistance * cos(iangle*2*pi/360) / (timeOutSlanti-timeInSlanti)); 
      double viy = 10000*(sidistance * sin(iangle*2*pi/360) / (timeOutSlanti-timeInSlanti));
      
      // compute the velocity in the x OUT direction
      double vfx = 10000*(xfdistance/(timeOutXf-timeInXf));
      
      // compute the y velocity using the dot product
      // ie vfy = vfslant - vfslant proj onto vfx     
      double vfy = 10000 * ((((sodistance*cos(16*2*pi/360))(timeOutSlantf-timeInSlantf)) – cos(16*2*pi/360) * (xfdistance(timeOutXf-timeInXf))) / sin(16*2*pi/360));
      
      // declare constants
      double c = 0.003;
      int a = 150;
      double b = 0.81;
  
      // calculate COR
      double e = vfy / viy;
  
      // calculate COF
      double u = (vix - vfx)/(viy*(1 + e));
  
      // calculate adjusted COR for temperature
      double eT = e + c*(23 - T);
  
      // calculate CPR
      double CPR = 100*(1 - u) + a*(b  - eT);
  
  // PRINT OUT FINAL RESULTS
   Serial.print("vix = "); Serial.print(vix); Serial.println(" m/s");
   Serial.print("viy = "); Serial.print(viy); Serial.println(" m/s");
   Serial.print("vfx = "); Serial.print(vfx); Serial.println(" m/s");
   Serial.print("vfy = "); Serial.print(vfy); Serial.println(" m/s");
   Serial.print("e = "); Serial.print(e); Serial.println(" COR");
   Serial.print("u = "); Serial.print(u); Serial.println(" COF");
   Serial.print("T = "); Serial.print(T); Serial.println(" ºC");
   Serial.print("c = "); Serial.print(0.003); Serial.println(" constant");
   Serial.print("eT = "); Serial.print(eT); Serial.println(" calculated");
   Serial.print("a = "); Serial.print(150); Serial.println(" constant");
   Serial.print("b = "); Serial.print(0.81); Serial.println(" constant");
   Serial.print("CPR = "); Serial.print(CPR); Serial.println(" calculated");
   
/*
  } else {
    delay(1000);
  }
  */
}

