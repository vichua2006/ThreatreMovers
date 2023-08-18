/* header file for general functions
 * Created by: Victor Huang and Rohan Katreddy
 * June 23, 2023
 * 
 * 2023-06-25: removed ArduinoSTL due to serial.print/println usage conflicting with DMXSerial; implemented swap() instead
 */

#ifndef GeneralFunc_h
#define GeneralFunc_h

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)>(b)?(b):(a))

bool isZero(double x);

template<typename T> void swap(T& t1, T& t2) {
    T tmp(t1);
    t1=t2;
    t2=tmp;
} // template classes and functions must be defined in the header file (https://forum.arduino.cc/t/a-small-doubt-regarding-the-use-of-templates-in-libraries/850865)

#endif