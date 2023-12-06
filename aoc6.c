#include <math.h>
#include <stdio.h>

typedef struct {
    double time;
    double dist;
}Race;
Race races[ 4 ] = { { 40, 219}, { 81, 1012}, { 77, 1365}, { 72, 1089 }}; 

void get_solution( double race_time, double previous_record, double* min_hold, double* max_hold ) {
    // The the dist is 
    // dist = -x*x+ race_time*x - previous_record 
    // where x is hold_time, Use quadratic root finder to find both solutions. 
    double a = -1.0;
    double b = race_time;
    double c = -previous_record;
    double min_h = (-b + sqrt( b*b - 4*a*c))/(2.0*a);
    double max_h = (-b - sqrt( b*b - 4*a*c))/(2.0*a);
    // All integer numbers between the roots is the solution
    *min_hold = floor( min_h ) + 1.0;
    *max_hold = ceil( max_h ) - 1.0;
}

int main()
{
    double winning_ways = 1.0;
    double min_hold, max_hold;

    for( int race_no=0; race_no<4; race_no ++) {
        get_solution( races[ race_no].time, races[race_no].dist, &min_hold, &max_hold);
        winning_ways *= ( max_hold - min_hold) + 1.0;
    }
    printf( "Part1 %.0f  \n", winning_ways);

    get_solution( 40817772.0, 219101213651089.0, &min_hold, &max_hold);
    winning_ways = ( max_hold - min_hold) + 1.0;
    printf( "Part2 %.0f  \n", winning_ways);
}