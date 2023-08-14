/* configuration file for MovingHeadLight library
 * Created by: Victor Huang and Rohan Katreddy
 * June 26, 2023
 */

#ifndef config_h
#define config_h

// if not defined, all references to DMXSerial will not exist, allowing for Serial.println() to be used
#define USE_DMX

/**************
 * PAN TILT CONFIG *
 **************/

// pan config
const int PanSteps      = 200;
const int PanStepPin    = 4;
const int PanDirPin     = 7;
const int PanHallPin    = 0; 
const double PanGR      = 144.0 / 17.0 * 16;
const double PanUpper   = 358.0;
const double PanLower   = -358.0;

// tilt config
const int TiltSteps     = 200;
const int TiltStepPin   = 3;
const int TiltDirPin    = 6;
const int TiltHallPin   = 2; // NOTE (2023-06-25): the only available interrupt pins on arduino uno are pins 2 and 3
const double TiltGR     = 64.0 / 21.0 * 16;
const double TiltUpper  = 90.0;
const double TiltLower  = 0.0;

// general config
const int StepperEnablePin  = 8; // enable pins for motor (cnc shield); must be at low
const int MinStepperDelay   = 300;
const int FixedStepperDelay = 700;
const double OneRevolution = 360.0;
const double BoundaryUC = 2.0; // boundary uncertainty of 2 degrees

/**************
 * DMX CONFIG *
 **************/

// receiver config
const int DMXReceiverPin        = 5;

// default starting channel
const int DefaultStartChannel   = 1;

// relative channel position config
const int RelativeMasterDimmer  = 0;
const int RelativeRedChan       = 1;
const int RelativeGreenChan     = 2;
const int RelativeBlueChan      = 3;
const int RelativePanChan       = 4;
const int RelativeTiltChan      = 5;

// input range config (bad design)
const double PanInputUpper = 359.0;
const double PanInputLower = 0.0;
const double TiltInputUpper = 90.0;
const double TiltInputLower = 0.0;

#endif