#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

int extract_number( char* line ) { 
    char* first_digit = line;
    while( !isdigit( *first_digit )) first_digit++;
    char* last_digit = line+strlen( line );
    while( !isdigit( *last_digit )) last_digit--;
    int number = 10*(*first_digit-'0') + *last_digit-'0';
    return number;
}

int main()
{
    FILE *fp = fopen( "input1.txt", "r");
    char line[256];
    
    int total = 0;
    while ( fgets( line, 256, fp ) != NULL) {
        total += extract_number( line );
    }
    printf( "\nSum is %d\n", total );
    fclose( fp );

    fp = fopen( "input1.txt", "r");
    total = 0;
    while ( fgets( line, 256, fp ) != NULL) {
        const char* number_strings[10] = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" }; 
        char target_string[256] = {0};
        char *src = line;
        char* dst = target_string;
        while( *src != 0 ) {
            bool found_digit_string = false;
            for( int num_idx=0; num_idx< 10; num_idx ++ ) {
                if( strncmp( number_strings[ num_idx], src, strlen( number_strings[ num_idx])) == 0 ) {
                    found_digit_string = true;
                    *dst++ = num_idx + '0';
                    src ++;
                    break;
                }
            }
            if( !found_digit_string ) {
                *dst++ = *src++;
            } 
        }
        total += extract_number( target_string );
    }
    printf( "\nSum is %d\n", total );
    fclose( fp );
}