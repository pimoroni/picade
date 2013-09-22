// key codes to emmit when input detected
char keys[]  = {KEY_LEFT_ARROW, KEY_UP_ARROW, KEY_RIGHT_ARROW, KEY_DOWN_ARROW, KEY_LEFT_CTRL, KEY_LEFT_ALT, '1', '5', '6', KEY_ESC, KEY_RETURN};
// 0 = PORTB, 1 = PORTF, 2 = PORTD
char ports[] = {0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2};
// bitmask to test the port against
char masks[] = {B00000001, B00000010, B00000100, B00001000, B00000001, B00000010, B00010000, B00100000, B01000000, B00000100, B00001000};
// current state (0 = unpressed, 1 = pressed)
boolean pressed[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

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
  for(int i = 0; i < sizeof(keys) / sizeof(char); i++)
  {
    // test for current state of this input
    boolean test = ~pinStates[ports[i]] & masks[i];
    
    if(test != pressed[i]) // has this input changed state since the last time we checked?
    {
      changed = true; // something changed, we should remember that
      pressed[i] = test; // update our state map so we know what's happening with this key in future
      
      if(test) // send the key press or release event
        Keyboard.press(keys[i]);
      else
        Keyboard.release(keys[i]);
    }
  }
  
  if(changed) // if an input changed we'll add a small delay to avoid "bouncing" of the input
    delay(50);
}