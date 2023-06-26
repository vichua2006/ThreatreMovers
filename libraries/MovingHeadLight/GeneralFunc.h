/* header file for general functions
 * Created by: Victor Huang and Rohan Katreddy
 * June 23, 2023
 * 
 * 2023-06-25: removed ArduinoSTL and implemented swap()
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
}

#endif