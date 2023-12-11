#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAP_SIZE 200
char map[MAP_SIZE][MAP_SIZE] = {0};
char outline[MAP_SIZE][MAP_SIZE] = {0};

typedef struct{
    int x,y;
}Point;

typedef struct{
    Point prev;
    Point curr;
}PipeTracker;

typedef struct{
    char* dir;
    int dx,dy;
} Direction;
Direction directions[] = { {"|7F", 0,-1}, {"|JL", 0,1}, {"-J7", 1,0},  {"-LF", -1,0}};

void follow_pipe( PipeTracker* tracker, bool push) {
    for( int dir=0; dir<4; dir++ ) {
        Point potential = {tracker->curr.x + directions[dir].dx, tracker->curr.y + directions[dir].dy};
        Point test_pos = potential;
        if( push ) {
            test_pos = tracker->curr;
            potential.x = tracker->curr.x - directions[dir].dx;
            potential.y = tracker->curr.y - directions[dir].dy;
        }
        if (strchr(directions[dir].dir, map[test_pos.y][test_pos.x])) {
            if( potential.x != tracker->prev.x || potential.y != tracker->prev.y ) {
                tracker->prev = tracker->curr;
                tracker->curr = potential;
                outline[ tracker->prev.y ][ tracker->prev.x ] = 'S';
                return;
            }
        }
    }
}

int main(void) {
    FILE *fp = fopen("input10.txt", "r");

    int size = -1;
    while (fgets(&map[++size], 512, fp) != NULL);
    fclose(fp);
    int x,y;
    for( y=0; y<size; y++ ) {
        for( x=0; x<size; x++ ) {
            if( map[y][x] == 'S' ) goto out;
        }
    } out:
    PipeTracker trackers[2] = { { x,y,x,y}, {x,y,x,y}};
    outline[ trackers[0].curr.y ][ trackers[0].curr.x  ] = 'S';
    follow_pipe( &trackers[0], false );
    trackers[1].prev = trackers[0].curr;
    follow_pipe( &trackers[1], false );

    int num_steps =1;
    while( trackers[0].curr.x != trackers[1].curr.x || trackers[0].curr.y != trackers[1].curr.y) {
        follow_pipe( &trackers[0], true);
        follow_pipe( &trackers[1], true);
        num_steps++;
    }
    //6828
    printf( "Part 1, Steps %d\n", num_steps);

    outline[ trackers[0].curr.y ][ trackers[0].curr.x  ] = 'S';
    int fill_count = 0;
    for( y=0; y<size; y++ ) {
        bool fill = false;
        char split_start;
        bool half_split = false;
        for( x=0; x<size; x++ ) {
            if (outline[y][x] == 'S' ) {
                if (strchr("|", map[y][x])) {
                    fill = !fill;
                } else if (strchr("J7LF", map[y][x])) {
                    if( half_split ){
                        half_split = false;
                        if( (split_start == 'F' && map[y][x] == 'J' ) ||
                            (split_start == 'L' && map[y][x] == '7' ) ) {
                            fill = !fill;
                        }
                    } else {
                        split_start = map[y][x];
                        half_split = true;
                    }
                }
            } else {
                if( fill && half_split == false )fill_count ++;
            }
        }
    }
    printf( "Part 2, Cells %d\n", fill_count);
}