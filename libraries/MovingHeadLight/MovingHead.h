/* header file for moving head light object
 * Created by: Victor Huang and Rohan Katreddy
 * June 22, 2023
 *
 */

#ifndef MovingHead_h
#define MovingHead_h

#include <Arduino.h>
#include "DualStepper.h"
#include "HallSensor.h"

class MovingHead{
    private:
        DualStepper &PanTilt;
        DualHallSensors &PanTiltHallSensor;

    public:
        MovingHead(DualStepper &steppers, DualHallSensors &sensors);
        MovingHead() = default;

        void init_pin_mode();
        void move_to(double pan_pos, double tilt_pos, int delay);
        void find_home();

};

#endif