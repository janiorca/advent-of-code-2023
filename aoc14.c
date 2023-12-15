#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

char map[128][128] = {0};

void tilt_north( int width, int height) {
    for( int y=0; y<height; y++ ) {
        for( int x=0; x<width; x++ ) {
            if( map[y][x] == 'O') {
                int dest = y;
                for( int yd=y-1; yd>=0; yd-- ) {
                    if( map[yd][x] !='.' )break;
                    dest = yd;
                }
                if( dest!= y) {
                    map[dest][x] = 'O';
                    map[y][x] = '.';
                }
            }
        }
    }
}

void tilt_south( int width, int height) {
    for( int y=height-1; y>=0; y-- ) {
        for( int x=0; x<width; x++ ) {
            if( map[y][x] == 'O') {
                int dest = y;
                for( int yd=y+1; yd<height; yd++ ) {
                    if( map[yd][x] !='.' )break;
                    dest = yd;
                }
                if( dest!= y) {
                    map[dest][x] = 'O';
                    map[y][x] = '.';
                }
            }
        }
    }
}

void tilt_west( int width, int height) {
    for( int x=0; x<width; x++ ) {
        for( int y=0; y<height; y++ ) {
            if( map[y][x] == 'O') {
                int dest = x;
                for( int xd=x-1; xd>=0; xd-- ) {
                    if( map[y][xd] !='.' )break;
                    dest = xd;
                }
                if( dest!= x) {
                    map[y][dest] = 'O';
                    map[y][x] = '.';
                }
            }
        }
    }
}

void tilt_east( int width, int height) {
    for( int x=width-1; x>=0; x-- ) {
        for( int y=0; y<height; y++ ) {
            if( map[y][x] == 'O') {
                int dest = x;
                for( int xd=x+1; xd<width; xd++ ) {
                    if( map[y][xd] !='.' )break;
                    dest = xd;
                }
                if( dest!= x) {
                    map[y][dest] = 'O';
                    map[y][x] = '.';
                }
            }
        }
    }
}

uint64_t calc_weight( int width, int height ) {
    uint64_t total_weight = 0;
    for( int y=0; y<height; y++ ) {
        for( int x=0; x<width; x++ ) {
            if( map[y][x] == 'O') total_weight += height-y;
        }
    }
    return total_weight;
}

int detect_loop( uint64_t* weights, int num_weights, uint64_t target_cycles) {
    // The loop detector just uses the weights. This could in theory find false loops where the weights just happen
    // to match the chances of an entire loop of false matches seems extremely small
    for( int loop_size=2; loop_size<10000; loop_size++ ) {
        if( num_weights > loop_size*2 ) {
            bool is_looping = true;
            for (int loop = 0; loop < loop_size; loop++) {
                int idx = num_weights - 1 - loop;
                int idx2 = idx-loop_size;
                if (weights[idx] != weights[idx2]) {
                    is_looping = false;
                    break;
                }
            }
            if (is_looping) {
                uint64_t remaining_loops = (target_cycles-num_weights) / loop_size;
                uint64_t skip = remaining_loops * loop_size;
                uint64_t skipped_pos = num_weights + skip;
                int idx = num_weights - loop_size + (target_cycles - skipped_pos)-1;
                return weights[ idx ];
            }
        }
    }
    return 0;
}

void read_map(int* pwidth, int* pheight ) {
    FILE *fp = fopen("input14.txt", "r");
    int width;
    int height = -1;
    while( fgets(map[++height], 128, fp));
    fclose( fp );
    *pwidth = strlen(map[height-1]);
    *pheight = height;
}

int main(void) {
    FILE *fp = fopen("input14.txt", "r");
    int width;
    int height;
    read_map( &width, &height);

    // Part1
    tilt_north( width, height);
    uint64_t weight = calc_weight( width, height );
    printf(  "Part1, weight %"PRIu64" \n", weight);

    // Part 2
    uint64_t weights[ 10000 ] ={0};
    read_map( &width, &height);
    int num_weights = 0;
    for( ;; ) {
        tilt_north(width,height);
        tilt_west(width,height);
        tilt_south(width,height);
        tilt_east(width,height);
        uint64_t weight = calc_weight( width, height );
        weights [ num_weights++] = weight;
        int solution = detect_loop( weights, num_weights, 1000000000 );
        if( solution > 0 ) {
            printf(  "Part2, solution  %"PRIu64" \n", solution);
            break;
        }
    }

}