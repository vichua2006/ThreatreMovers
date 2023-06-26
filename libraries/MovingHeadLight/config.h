/* configuration file for MovingHeadLight library
 * Created by: Victor Huang and Rohan Katreddy
 * June 26, 2023
 */



/**************
 * PAN TILT CONFIG *
 **************/

// pan config
const int PanSteps = 200;
const int PanStepPin = 4;
const int PanDirPin = 7;
const int PanHallPin = 0; 
const double PanGR = 144.0 / 17.0;
const double PanUpper = 359.0;
const double PanLower = -359.0;

// tilt config
const int TiltSteps = 200;
const int TiltStepPin = 3;
const int TiltDirPin = 6;
const int TiltHallPin = 2; // NOTE (2023-06-25): the only available interrupt pins on arduino uno are pins 2 and 3
const double TiltGR = 64.0 / 21.0;
const double TiltUpper = 90.0;
const double TiltLower = 0.0;

// general config
const int StepperEnablePin = 8; // enable pins for motor (cnc shield); must be at low
const int MinStepperDelay = 700;
const int FixedStepperDelay = 700;

/**************
 * DMX CONFIG *
 **************/
const int DMXReceiverPin = 5;


String INPUT_STRING;
double INPUT_POS;
