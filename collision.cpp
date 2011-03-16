/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "collision.h"

using namespace std;

bool collision_check(double a_x,double a_y,double a_w,double a_h,double b_x,double b_y,double b_w,double b_h){
    //The sides of the rectangles:
    double left_a,left_b;
    double right_a,right_b;
    double top_a,top_b;
    double bottom_a,bottom_b;

    //Calculate the sides of rectangle A:
    left_a=a_x;
    right_a=a_x + a_w;
    top_a=a_y;
    bottom_a=a_y + a_h;

    //Calculate the sides of rectangle B:
    left_b=b_x;
    right_b=b_x + b_w;
    top_b=b_y;
    bottom_b=b_y + b_h;

    //Check each side of A.  If it is found to be outside of B, these two rectangles are not colliding, so return false.
    if(bottom_a<=top_b){
        return false;
    }
    if(top_a>=bottom_b){
        return false;
    }
    if(right_a<=left_b){
        return false;
    }
    if(left_a>=right_b){
        return false;
    }

    //If none of the sides from a are outside b, a collision has occured.
    return true;
}
