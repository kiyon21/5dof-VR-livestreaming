// CONSTANTS FOR MOTOR CONTROL
// Time between steps for all motors
const int MAX_DELAY = 1500; // Starting delay (decreased on change of directions)
const int MIN_DELAY = 800; // Fastest that gantry can travel

// Motor Control pins
// M1 and M2 Connect to the crossbar and will move in the same direction (fwd/back)
// M3 Controls left/right of camera mount
const int M1_DIR_PIN = 3; // Sets direction of motor
const int M1_STEP_PIN = 2; // Controls motor step
const int M2_DIR_PIN = 5;
const int M2_STEP_PIN = 4;
const int M3_DIR_PIN = 7;
const int M3_STEP_PIN = 6;


// Motor directions
const int M1_FWD = LOW;
const int M1_BCKWD = HIGH;
const int M2_FWD = HIGH;
const int M2_BCKWD = LOW;
const int M3_LEFT = LOW;
const int M3_RIGHT = HIGH;

// Calibration button pins
// RESET_STATE is the state the buttons go to when pressed
const int BTN_1 = 10;
const int BTN_2 = 11;
const int B_RESET_STATE = LOW;

static int m1_m2_pos = 0;
static int m3_pos = 0;

// Constants that split area into grid shape
const int MAX_X = 9;
const int MAX_Y = 25;
const int STEPS_PER_INC = 100;

// Variables to hold latest coordinates from headset in values between 0-MAX_X/Y
static int curr_x_coord;
static int curr_y_coord;

// Values hold current grid position
static int curr_x_pos;
static int curr_y_pos;

// Delay variable to store speed of motors
static int x_del; 
static int y_del;

// These values hold the last direction of motion, forward, backward or stationary
static int x_dir; 
static int y_dir;

// Parses integer points representing a grid position"
void get_coordinates_from_serial(int *x, int *y) {
  if (Serial.available() > 0) {
    // Read the incoming serial data until newline
    String input = Serial.readStringUntil('\n');
    input.trim();  // Remove any leading/trailing spaces

    // Find the comma separating the two floats
    int commaIndex = input.indexOf(',');

    if (commaIndex != -1) {  // Ensure a comma exists
      // Extract the first and second float values
      String xText = input.substring(0, commaIndex);
      String yText = input.substring(commaIndex + 1);

      // Convert to floats and store in pointers
      *x = xText.toInt();
      *y = yText.toInt();
    }
  }
}

void step() {
  // Set motor directions
  if (x_dir > 0) {
    digitalWrite(M3_DIR_PIN, M3_RIGHT);
  } else if (x_dir < 0) {
    digitalWrite(M3_DIR_PIN, M3_LEFT);
  }
  if (y_dir > 0) {
    digitalWrite(M1_DIR_PIN, M1_FWD);
    digitalWrite(M2_DIR_PIN, M2_FWD); 
  } else if (y_dir < 0) {
    digitalWrite(M1_DIR_PIN, M1_BCKWD);
    digitalWrite(M2_DIR_PIN, M2_BCKWD); 
  }
  // Step any motors that need stepping

  // This complicated ass logic basically says, we need to step motors, and the delay between steps for each might be different
  // Run the while loop with a 1us delay, and if the period for the direction has passed, step those motors
  int x_steps = 0;
  int x_last_step = 0;
  int y_steps = 0;
  int y_last_step = 0;
  int us_delayed = 0;
  while ((x_dir != 0 && x_steps < STEPS_PER_INC) || (y_dir != 0 && y_steps < STEPS_PER_INC)) {
    if (x_dir != 0) {
      if (us_delayed - x_last_step == x_del) {
        x_last_step = us_delayed;
        digitalWrite(M3_STEP_PIN, LOW);
        digitalWrite(M3_STEP_PIN, HIGH);
        if (x_del > MIN_DELAY) {
          x_del = x_del-3;
        }
        x_steps++;
      }
    }
    if (y_dir != 0) {
      if (us_delayed - y_last_step == y_del) {
        y_last_step = us_delayed;
        digitalWrite(M1_STEP_PIN, LOW);
        digitalWrite(M2_STEP_PIN, LOW);
        digitalWrite(M1_STEP_PIN, HIGH);
        digitalWrite(M2_STEP_PIN, HIGH);
        if (y_del > MIN_DELAY) {
          y_del = y_del-3;
        }
        y_steps++;
      }
    }
    delayMicroseconds(1);
    us_delayed++;
  }
  // Update current positions
  if (x_dir > 0) {
    curr_x_pos++;
  } else if (x_dir < 0) {
    curr_x_pos--;
  }
  if (y_dir > 0) {
    curr_y_pos++;
  } else if (y_dir < 0){
    curr_y_pos--;
  }
}

void update_pos(int x_coord, int y_coord) {
  // Set direction for each motor based on the received coordinates and current pos
  // grid positions are in 100 step increments
  if (curr_x_pos == x_coord) {
    x_dir = 0;
  } else if (curr_x_pos < x_coord) {
    // If we're changing directions, start at max delay
    if (x_dir == -1 || x_dir == 0) {
      x_del = MAX_DELAY;
    }
    x_dir = 1;
  } else {
    if (x_dir == 1 || x_dir == 0) {
      x_del = MAX_DELAY;
    }
    x_dir = -1;
  }
  if (curr_y_pos == y_coord) {
    y_dir = 0;
  } else if (curr_y_pos < y_coord) {
    // If we're changing directions, start at max delay
    if (y_dir == -1 || y_dir == 0) {
      y_del = MAX_DELAY;
    }
    y_dir = 1;
  } else {
    if (y_dir == 1 || y_dir == 0) {
      y_del = MAX_DELAY;
    }
    y_dir = -1;
  }
  // Move motors based on calculated directions
  step();
}

// void calibrate() {
//   bool m1_m2_reset = false;
//   bool m3_reset = false;0,0
//   int steps_per_cycle = -1;

//   // Step motors towards start pos until buttons are hit
//   while (!m1_m2_reset || !m3_reset) {
//     // Handle forward direction for motors m1 and m2
//     if (!m1_m2_reset) {
//       if (digitalRead(B1) == B_RESET_STATE) {
//         m1_m2_reset = true;
//       } else  {
//         move_forward(steps_per_cycle);
//       }
//     }
//     if (!m3_reset) {
//       if (digitalRead(B2) == B_RESET_STATE) {
//         m3_reset = true;
//       } else  {
//         move_left(steps_per_cycle);
//       }
//     }
//   }
// }`

void setup() {
  // Set up serial port and motor control pins
  Serial.begin(9600);
  pinMode(M1_DIR_PIN, OUTPUT);
  pinMode(M1_STEP_PIN, OUTPUT);
  pinMode(M2_DIR_PIN, OUTPUT);
  pinMode(M2_STEP_PIN, OUTPUT);
  pinMode(M3_DIR_PIN, OUTPUT);
  pinMode(M3_STEP_PIN, OUTPUT);
  pinMode(BTN_1, INPUT);
  pinMode(BTN_2, INPUT);
  
  // Calibrate gantry
  //calibrate();
}

void print_coords() {
    Serial.print(curr_x_coord);
    Serial.print(",");
    Serial.print(curr_y_coord);
    Serial.print("    ");
    Serial.print(curr_x_pos);
    Serial.print(",");
    Serial.print(curr_y_pos);
    Serial.print("\n");
}

void loop() {
  // Get coordinates from serial port
  get_coordinates_from_serial(&curr_x_coord, &curr_y_coord);
  update_pos(curr_x_coord,curr_y_coord);
  print_coords();
}
