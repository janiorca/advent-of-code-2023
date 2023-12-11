#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_MAPS_SIZE 200
char map[MAX_MAPS_SIZE][MAX_MAPS_SIZE];

typedef struct {
    int64_t x, y;
}Pos;

int64_t calc_distance( int size, int64_t expansion ) {
    bool occupied_x[MAX_MAPS_SIZE] = {0};
    bool occupied_y[MAX_MAPS_SIZE] = {0};
    for (int y = 0; y<size; y++) {
        for (int x = 0; x<size; x++) {
            bool is_occupied = map[y][x] != '.';
            occupied_x[x] |= is_occupied;
            occupied_y[y] |= is_occupied;
        }
    }
    int64_t remapped_x[ MAX_MAPS_SIZE] = {0};
    int64_t remapped_y[ MAX_MAPS_SIZE] = {0};
    int64_t offset = 0;
    for( int x=0; x<size; x++ ) {
        if( !occupied_x[x]) offset+= expansion;
        remapped_x[ x ] = x+offset;
    }
    offset = 0;
    for( int y=0; y<size; y++ ) {
        if( !occupied_y[y]) offset+= expansion;
        remapped_y[ y ] = y+offset;
    }

    int num_galaxies = 0;
    uint64_t total_dist = 0;
    Pos positions[2000] = {0};
    for (int y = 0; y<size; y++) {
        for (int x = 0; x<size; x++) {
            if( map[y][x] == '#') {
                Pos new_pos = { x,y};
                for( int idx=0; idx<num_galaxies; idx++ ) {
                    int dist = abs( remapped_x[new_pos.x] - remapped_x[ positions[idx].x] ) +
                               abs( remapped_y[new_pos.y] - remapped_y[ positions[idx].y] );
                    total_dist += dist;
                }
                positions[ num_galaxies++ ] = new_pos;
            }
        }
    }
    return total_dist;
}

int main(void) {
    FILE *fp = fopen("input11.txt", "r");
    int size = -1;
    while (fgets(&map[++size], 512, fp) != NULL);
    fclose(fp);

    uint64_t  total_dist = calc_distance( size, 1 );
    printf( "Total dist = %u", total_dist);

    total_dist = calc_distance( size, 1000000-1 );
    printf( "Total dist = %"PRIu64, total_dist);

}