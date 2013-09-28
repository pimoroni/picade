typedef struct
{
  char key;        // which keycode to emit when input is pressed
  int  port;       // which port is this input on? 0 = PORTB, 1 = PORTF, 2 = PORTD
  char mask;       // bitmask for the input on the port
  boolean pressed; // is this input current pressed?
} input;

input inputs[] = {
  { KEY_LEFT_ARROW,   0, B00000001, 0 },
  { KEY_UP_ARROW,     0, B00000010, 0 },
  { KEY_RIGHT_ARROW,  0, B00000100, 0 },
  { KEY_DOWN_ARROW,   0, B00001000, 0 },
  { KEY_LEFT_CTRL,    1, B00000001, 0 },
  { KEY_LEFT_ALT,     1, B00000010, 0 },
  { '1',              1, B00010000, 0 },
  { '5',              1, B00100000, 0 },
  { '6',              1, B01000000, 0 },
  { KEY_ESC,          2, B00000100, 0 },
  { KEY_RETURN,       2, B00001000, 0 }
};

void setup() {
  /*
    Setup the ports to be inputs will pullup resistors enabled
  */
  DDRB = DDRD = DDRF = B00000000; // set ports B, D, F to be inputs
  PORTB = PORTD = B11111111; // set the pullup resistors on ports B, D
  PORTF = B11110011; // set the pullup resistors on port F
}

void loop() {
  /*
    Check the state of each input, if it has changed then output the corresponding keypress
  */
  boolean changed = false;  // has any input changed?
  char pinStates[3] = {PINB, PINF, PIND}; // read the current port states and store in 0, 1, 2 indexed array to match our port ids above
  
  // loop through each input
  for(int i = 0; i < sizeof(inputs) / sizeof(input); i++)
  {
    // test for current state of this input
    boolean test = ~pinStates[inputs[i].port] & inputs[i].mask;
    
    if(test != inputs[i].pressed) // has this input changed state since the last time we checked?
    {
      changed = true; // something changed, we should remember that
      inputs[i].pressed = test; // update our state map so we know what's happening with this key in future
      
      if(test) // send the key press or release event
        Keyboard.press(inputs[i].key);
      else
        Keyboard.release(inputs[i].key);
    }
  }
  
  if(changed) // if an input changed we'll add a small delay to avoid "bouncing" of the input
    delay(50);
}
