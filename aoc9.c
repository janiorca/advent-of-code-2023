#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
    FILE *fp = fopen( "input9.txt", "r");
    char line[512];

    int extrapolated_sum = 0;
    int front_extrapolated_sum = 0;
    while ( fgets( line, 512, fp ) != NULL) {
        int numbers[20][128] = {0};
        int num_numbers = 0;
        char* pPos;
        char* pEnd = line;
        do {
            pPos = pEnd;
            numbers[0][num_numbers++] = strtoll( pPos, &pEnd, 10 );
        } while( pEnd != pPos );
        num_numbers--;

        int level = 1;
        for( ; level<20; level++ ) {
            bool all_zeroes = true;
            for( int idx=1; idx<num_numbers-level+1;idx++ ) {
                numbers[ level ][ idx-1 ] = numbers[ level-1][idx] - numbers[ level-1][idx-1];
                if( numbers[ level ][ idx-1 ] != 0 ) all_zeroes = false;
            }
            if( all_zeroes) break;
        }

        // solve part one by adding one number to end of level
        int new_value = 0;
        for( int new_level = level-1; new_level>=0; new_level--) {
            new_value = numbers[ new_level][num_numbers-new_level-1 ] + new_value;
        }
        extrapolated_sum += new_value;

        // solve part two by adding one number to front each level
        new_value = 0;
        for( int new_level = level-1; new_level>=0; new_level--) {
            new_value = numbers[ new_level][0 ] - new_value;
        }
        front_extrapolated_sum  += new_value;

    }
    printf( "Part1 %d\n", extrapolated_sum);
    printf( "Part2 %d\n", front_extrapolated_sum);
}