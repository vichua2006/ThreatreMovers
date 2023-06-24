/* src for general functions
 * Created by: Victor Huang and Rohan Katreddy
 * June 23, 2023
 */

#include <Arduino.h>

bool isZero(double x){
    return abs(x) < 1e-6;
}