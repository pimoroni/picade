typedef struct
{
  char key;        // which keycode to emit when input is pressed
  int  port;       // which port is this input on? 0 = PORTB, 1 = PORTF, 2 = PORTD
  char mask;       // bitmask for the input on the port
  boolean pressed; // is this input current pressed?
} input;

// PICADE: Switch to PCB Mappings
// Ignore GPIO for Mini
//-------------------------------------------------------------------
//          /      U                 Button       | GPIO    \
//         /                     1     2     3    |   1      \
// Start  |    L   O   R                          |           |  Esc
//         \                     4     5     6    |   2      /
//          \      D                                        /
//-------------------------------------------------------------------
//           |  Select                              Enter  |


input inputs[] = {
  // 0 is PORTB
  // 1 is PORTF
  // 2 is PORTD
  // 3 is PORTC

  /* Joystick UDLR on Port B-0123 */
  { KEY_UP_ARROW,      0, B00000001, 0 },
  { KEY_DOWN_ARROW,    0, B00000010, 0 },
  { KEY_LEFT_ARROW,    0, B00000100, 0 },
  { KEY_RIGHT_ARROW,   0, B00001000, 0 },

  /* Buttons 123456 on Port F-01-4567 */
  { KEY_LEFT_CTRL,     1, B10000000, 0 }, // Button 1
  { KEY_LEFT_ALT,      1, B01000000, 0 }, // Button 2
  { ' ',               1, B00100000, 0 }, // Button 3
  { KEY_LEFT_SHIFT,    1, B00010000, 0 }, // Button 4
  { 'z',               1, B00000010, 0 }, // Button 5
  { 'x',               1, B00000001, 0 }, // Button 6
  
  /* GPIO 12/Maxi Buttons 78 on Port D-67 */
  { 'c',               2, B01000000, 0 }, // GPIO 1 / Button 7
  { 'v',               2, B10000000, 0 }, // GPIO 2 / Button 8

  /* GPIO 3456 on Port B-456 C-6 */
  { ' ',               0, B00010000, 0 }, // GPIO 3
  { ' ',               0, B00100000, 0 }, // GPIO 4
  { ' ',               0, B01000000, 0 }, // GPIO 5
  { ' ',               3, B01000000, 0 }, // GPIO 6

  /* Front/Side Buttons on Port D-0123 */
  { '1',               2, B00000001, 0 }, // 1UP Start
  { '5',               2, B00000010, 0 }, // 1UP Select/Coin
  { KEY_ESC,           2, B00000100, 0 }, // Enter
  { KEY_RETURN,        2, B00001000, 0 }  // Esc

};

// PICADE: Default Mappings
// Ignore GPIO for Mini
//-------------------------------------------------------------------
//          /      Up                Button       | GPIO    \
//         /                  LCtrl   LAlt   Spa  |   C      \
//   1    |    Le  O   Ri                         |           |  Esc
//         \                  LShft    Z      X   |   V      /
//          \      Do                                       /
//-------------------------------------------------------------------
//           |    5                                 Enter  |



void setup() {
  /*
    Setup the ports to be inputs will pullup resistors enabled
  */
  DDRB = DDRD = DDRF = B00000000; // set ports B, D, F to be inputs
  PORTB = PORTD = B11111111; // set the pullup resistors on ports B, D
  PORTC = B01000000; // set the pullup resistors on port C
  PORTF = B11110011; // set the pullup resistors on port F
}

void loop() {
  /*
    Check the state of each input, if it has changed then output the corresponding keypress
  */
  boolean changed = false;  // has any input changed?
  char pinStates[4] = {PINB, PINF, PIND, PINC}; // read the current port states and store in 0, 1, 2 indexed array to match our port ids above

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
    delay(20);
}
