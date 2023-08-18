/* header file for moving head light object
 * Created by: Victor Huang and Rohan Katreddy
 * June 22, 2023
 *
 * 2023-06-25: added ifdef USING_DMX for debugging
 */

#ifndef MovingHead_h
#define MovingHead_h

#include "DualStepper.h"
#include "HallSensor.h"
#include "config.h"

class MovingHead{
    private:
        DualStepper &panTilt;
        DualHallSensors &panTiltHallSensor;

        // dmx channels
        int START_CHANNEL;

        int MASTER_DIMMER;
        int RED_CHAN;
        int GREEN_CHAN;
        int BLUE_CHAN;
        int PAN_CHAN;
        int TILT_CHAN;

    public:
        MovingHead(DualStepper &steppers, DualHallSensors &sensors, int start_channel);
        MovingHead() = default;

        void init_pin_mode();
        void move_towards(double pan_pos, double tilt_pos);
        void find_home();
        void new_start_channel(int new_channel);
        void set_speed(int pan_speed, int tilt_speed);
        void set_accel(int pan_accel, int tilt_accel);

        #ifdef USING_DMX
        void main_cycle();
        int read_dmx_channel(int channel);
        #endif

};

#endif