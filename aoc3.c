#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAP_SIZE 140

typedef struct{
    int x, y;
    int width;
    int value;
}NumberBox;

char map[MAP_SIZE*MAP_SIZE] = {0};
bool touching_symbol[MAP_SIZE*MAP_SIZE] = {false};
NumberBox number_boxes[10000] = {0};

bool is_touching_symbol( int x, int y) {
    for( int test_y=y-1; test_y<=y+1; test_y ++ ) {
        for( int test_x=x-1; test_x<=x+1; test_x ++ ) {
            if( test_y < 0 || test_x < 0 || test_y >= MAP_SIZE || test_x >= MAP_SIZE) continue;
            char c = map[ test_y*MAP_SIZE+test_x];
            if( !isdigit(c) && c != '.') {
                return true;
            }
        }
    }
    return false;
}

int main()
{
//    FILE *fp = fopen( "test_input.txt", "r");
    FILE *fp = fopen( "input3.txt", "r");
    char line[256];
    
    int y = 0;
    while ( fgets( line, 256, fp ) != NULL) {
        memcpy( &map[MAP_SIZE*y++], line, MAP_SIZE );
    }
    
    int all_sum = 0;
    int schematic_sum = 0;
    int num_number_boxes = 0;
    for( int y=0; y<MAP_SIZE; y++ ) {
        char* read_pos = &map[y*MAP_SIZE];
        char* next_line = &map[(y+1)*MAP_SIZE];

        for( ;; ) {
            char* num_start = strpbrk( read_pos, "0123456789");
            if( num_start >= next_line || num_start == NULL ) break;
            char* num_end = num_start + strspn( num_start, "0123456789");
            if( num_end > next_line) num_end = next_line;

            // Extract the number
            char number_string[ 64] = {0};
            strncpy( number_string, num_start, num_end-num_start);
            int value = atoi( number_string );
            int x = num_start-&map[y*MAP_SIZE];
            int width = num_end - num_start;
            
            // Store the number box for the next part of the problem
            NumberBox nb = {x,y, width, value};
            number_boxes[ num_number_boxes++ ] = nb;

            // Are any of the digits touching a symbol
            bool is_touching = false;
            for( int x_pos=x; x_pos<x+width; x_pos++ ) {
                is_touching |= is_touching_symbol( x_pos, y);
            }
            if( is_touching ) schematic_sum += value;
            all_sum += value;
            read_pos = num_end;
        }
    }
    printf( "\nSum of schematic numbers: %d", schematic_sum);

    // Part 2
    int sum_of_gear_ratios = 0;
    for( int y=0; y<MAP_SIZE; y++ ) {
        for( int x=0; x<MAP_SIZE; x++ ) {
            if( map[y*MAP_SIZE+x] == '*') {
                // Found gear, find any touching numbers
                int num_touching = 0;
                int ratio = 1;
                if( x == 23 && y == 5 ) {
                    int t =0;
                }
                for( int n_box=0; n_box<num_number_boxes; n_box++ ) {
                    if( number_boxes[ n_box].x -1 <= x && number_boxes[ n_box].y -1 <=y &&
                        (number_boxes[ n_box].x + number_boxes[ n_box].width)>= x && number_boxes[ n_box].y >=y ) {
                        ratio *= number_boxes[n_box].value;
                        num_touching ++;
                    }
                }
                if( num_touching >= 2) sum_of_gear_ratios += ratio;
            }
        }
    }

    printf( "\nSum of gear ratios: %d", sum_of_gear_ratios);
    int t = 0;
}