/* configuration file for MovingHeadLight library (pico version)
 * Created by: Victor Huang and Rohan Katreddy
 * August 22, 2023
 */

#ifndef config_h
#define config_h

/**************
 * PAN TILT CONFIG *
 **************/


// pan config
const int PanMicroSteps  = 8;
const int PanSteps      = 200 * PanMicroSteps;
const int PanStepPin    = 2;
const int PanDirPin     = 3;
const int PanHallPin    = 0; 
const double PanGR      = 144.0 / 17.0;
const double PanUpper   = 359.0;
const double PanLower   = 0.0;

const int PanMaxSpeed   = 200 * PanMicroSteps * PanGR; // in steps per second

// tilt config
const int TiltMicroSteps = 8;
const int TiltSteps     = 200 * TiltMicroSteps;
const int TiltStepPin   = 6;
const int TiltDirPin    = 7;
const int TiltHallPin   = 2; // NOTE (2023-06-25): the only available interrupt pins on arduino uno are pins 2 and 3
const double TiltGR     = 64.0 / 21.0;
const double TiltUpper  = 180;
const double TiltLower  = 0.0;

const int TiltMaxSpeed      = 400 * TiltMicroSteps * TiltGR; // in steps per second

// general config
const int StepperEnablePin  = 14; // enable pins for motor (cnc shield); must be at low
const int MinStepperDelay   = 40; // minimum on 1/8 microsteping TMC2209
const int FixedStepperDelay = 500;
const double OneRevolution = 360.0; // set to 2 for radian-based system
const double BoundaryUC = 2.0; // boundary uncertainty of 2 degrees

/**************
 * DMX CONFIG *
 **************/

// receiver config
const int DMXReceiverPin        = 1;
const int DMXControlPin         = 15;

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
const double TiltInputUpper = 180.0;
const double TiltInputLower = 0.0;

#endif