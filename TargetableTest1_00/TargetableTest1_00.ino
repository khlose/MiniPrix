const int clockPin = 10;
const int dataPin = 11;
const int start = 7;
int c = 0;
const int nextColor = 8;


void setup() {
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(start, INPUT);
  pinMode(nextColor, INPUT);
  digitalWrite(clockPin, LOW);
  digitalWrite(dataPin, LOW);
  Serial.begin(9600);

}

void loop() {
  //if(digitalRead(start)){
    //Serial.println(digitalRead(start));
    blank();
  //}
  //if(digitalRead(nextColor)){
    //Serial.println(digitalRead(nextColor));
    
    h(); // start bit
    h(); // Green 5 bits
    l();
    l();
    l();
    l();
    h(); // red 5 bits
    l();
    l();
    l(); 
    l();
    l(); // blue 5 bits
    l();
    l();
    l();
    l();
    
    h(); // start bit
    h(); // Green 5 bits
    l();
    l();
    l();
    l();
    l(); // red 5 bits
    l();
    l();
    l(); 
    l();
    h(); // blue 5 bits
    l();
    l();
    l();
    l();
    
    h(); // start bit
    l(); // Green 5 bits
    l();
    l();
    h();
    l();
    l(); // red 5 bits
    l();
    l();
    l(); 
    l();
    l(); // blue 5 bits
    l();
    h();
    l();
    l();
    
    h(); // start bit
    l(); // Green 5 bits
    l();
    l();
    l();
    l();
    l(); // red 5 bits
    h();
    l();
    l(); 
    l();
    h(); // blue 5 bits
    l();
    l();
    l();
    l();
    
    h(); // start bit
    l(); // Green 5 bits
    l();
    l();
    l();
    l();
    h(); // red 5 bits
    l();
    l();
    l(); 
    l();
    l(); // blue 5 bits
    l();
    l();
    h();
    l();
    
    h(); // start bit
    l(); // Green 5 bits
    l();
    l();
    h();
    l();
    h(); // red 5 bits
    l();
    l();
    l(); 
    l();
    l(); // blue 5 bits
    l();
    l();
    l();
    l();


  //}
  digitalWrite(dataPin, LOW);
  digitalWrite(clockPin, LOW);
  

}

void h(){
  digitalWrite(dataPin, HIGH);
  delay(1);
  digitalWrite(clockPin, HIGH);
  delay(1);
  digitalWrite(clockPin, LOW);
}

void l(){
  digitalWrite(dataPin, LOW);
  delay(1);
  digitalWrite(clockPin, HIGH);
  delay(1);
  digitalWrite(clockPin, LOW);
}

void blank(){
  h();  //start bit
  
  l();
  l();
  l();
  l();
  l();
  l();
  l();
  l();
  
  l();
  l();
  l();
  l();
  l();
  l();
  l();
  l();
  
//  l();
//  l();
//  l();
//  l();
//  l();
//  l();
//  l();
//  l();
//  
//  l();
//  l();
//  l();
//  l();
//  l();
//  l();
//  l();
//  l();
}


