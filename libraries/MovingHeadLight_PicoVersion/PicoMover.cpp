/* source file for mover class
 * Created by: Victor Huang and Rohan Katreddy
 * Aug 30, 2023
 * 
 */

#include <PicoMover.h>
#include <AccelStepper.h>

void Mover::add_steppers(AccelStepper &s1, AccelStepper &s2){
    _steppers[0] = &s1;
    _steppers[1] = &s2;
}

void Mover::set_max_speed(int speed1, int speed2){
    _max_speeds[0] = speed1;
    _max_speeds[1] = speed2;    
}

void Mover::set_accel(int accel1, int accel2){
    _steppers[0]->setAcceleration(accel1);
    _steppers[1]->setAcceleration(accel2);
}

void Mover::coordinate(int pos1, int pos2){

    _steppers[0]->moveTo(pos1);
    _steppers[1]->moveTo(pos2);

    // calculate for the longest time required
    double t_max = 0.0;
    double t_accel = 1.0 / AccelFactor; // time it takes to accel to the 
    for (int i=0;i<2;i++){
      double t_total = ((double) _steppers[i]->distanceToGo() / (double) _max_speeds[i]) + t_accel;
      t_max = max(t_max, t_total);
    }

    // set new speed for each stepper
    for (int i=0;i<2;i++){
      _steppers[i]->setMaxSpeed((double) _steppers[i]->distanceToGo() / (t_max - t_accel));
    }
}

bool Mover::run_towards(){
    for (int i=0;i<2;i++) _steppers[i]->run();
    return true;
}



