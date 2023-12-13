#include <stdio.h>
#include <string.h>
#include <stdbool.h>

char pattern[64][64];

int find_symmetry( int width, int height, bool smudge ) {
    for( int x=1; x<width; x++ ) {
        bool is_symmetrical = true;
        int faults = 0;
        for( int y=0; y<height; y++ ) {
            for( int sx=0; sx<x && (x+sx) < width; sx++ ) {
                if( pattern[y][x-1-sx] != pattern[y][x+sx]) {
                    is_symmetrical = false;
                    faults +=1;
                }
            }
        }
        if( smudge ) {
            if (!is_symmetrical && faults == 1) {
                return x;
            }
        } else {
            if( is_symmetrical ) {
                return x;
            }
        }
    }
    for( int y=1; y<height; y++ ) {
        bool is_symmetrical = true;
        int faults = 0;
        for( int x=0; x<width; x++ ) {
            for( int sy=0; sy<y && (y+sy) < height; sy++ ) {
                if( pattern[y-1-sy][x] != pattern[y+sy][x]){
                    is_symmetrical = false;
                    faults += 1;
                };
            }
        }
        if( smudge ) {
            if (!is_symmetrical && faults == 1) {
                return y*100;
            }
        } else {
            if( is_symmetrical ) {
                return y*100;
            }
        }
    }
    return 0;
}

int main(void) {
    FILE *fp = fopen("input13.txt", "r");
    int width;
    int height = -1;
    int pattern_no = 0;
    int symmetry_sum = 0;
    int smudge_sum = 0;
    for( ;; ) {
        fgets(pattern[++height], 256, fp);
        int line_with = strlen( pattern[height] );
        if( line_with == 1 || feof( fp )) {
            // end of pattern
            printf( "%d X %d   %d\n\r", width, height, pattern_no++ );
            symmetry_sum += find_symmetry( width, height, false );
            smudge_sum += find_symmetry( width, height, true );
            height = -1;
            if( feof(fp) ) break;
        }
        width = line_with-1;
    }
    printf( "Symmetry sum %d (part1) \n", symmetry_sum);
    printf( "Smudge sum %d (part2) \n", smudge_sum);
}
