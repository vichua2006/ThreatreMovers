/* header file for pico mover class
 * Created by: Victor Huang and Rohan Katreddy
 * Aug 30, 2023
 * 
 */

#ifndef MOVER_h
#define MOVER_h

#include <AccelStepper.h>
#include "PicoConfig.h"

class Mover{
    private:
        // accelstepper address array
        AccelStepper* _steppers[2];
        // max speed of each stepper
        int _max_speeds[2]; 
    
    public: 

        // constructor and destructor
        Mover() = default;

        /// @brief add pan and tilt steppers to mover object
        void add_steppers(AccelStepper &s1, AccelStepper &s2);

        /// @brief Set max speeds in steps per second for both steppers
        /// @param speed1 speed for stepper 1
        /// @param speed2 speed for stepper 2
        void set_max_speed(int speed1, int speed2);

        /// @brief Set acceleration in steps per second per second for both steppers. Computes a squre root, do not call more than nessecary.
        /// @param accel1 acceleration of stepper 1
        /// @param accel2 acceleration of stepper 2
        void set_accel(int accel1, int accel2);

        /// @brief Adjust the speed of steppers to ensure that both arrive at give positions near the same time. 
        /// @param pos1 target position of stepper 1
        /// @param pos2 target position of stepper 2
        void coordinate(int pos1, int pos2);

        /// @brief Move steppers towards their target position set by `coordinate()` by one step
        /// @return true if there's still at least one stepper running to its position
        bool run_towards();

};


#endif