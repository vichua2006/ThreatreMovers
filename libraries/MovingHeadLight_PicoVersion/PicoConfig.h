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
const int PanStepPin    = 3;
const int PanDirPin     = 2;
const double PanGR      = 144.0 / 17.0;
const double PanUpper   = 359.0;
const double PanLower   = 0.0;

const int PanMaxSpeed   = 100 * PanMicroSteps * PanGR; // in steps per second

// tilt config
const int TiltMicroSteps = 8;
const int TiltSteps     = 200 * TiltMicroSteps;
const int TiltStepPin   = 7;
const int TiltDirPin    = 6;
const double TiltGR     = 64.0 / 21.0;
const double TiltUpper  = 180;
const double TiltLower  = 0.0;
const int TiltDriverPDN = 9;

const int TiltMaxSpeed      = 400 * TiltMicroSteps * TiltGR; // in steps per second

// general config
const int StepperEnablePin  = 14; // enable pins for stepper; must be at low
const double AccelFactor    = 4.0; // acceleration = speed * AccelFactor
const double OneRevolution  = 360.0; // set to 2 for radian-based system

/**************
 * DMX CONFIG *
 **************/

// number of channels the fixture takes up
const int TotalChannels         = 10;

enum FixtureChannels{
    PAN,
    TILT,
    DIMMER,
};

// receiver config
const int DMXReceiverPin        = 1;
// might not be needed
const int DMXControlPin         = 15;

// default starting channel
const int DefaultStartChannel   = 1;

#endif