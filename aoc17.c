#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

char map[200][200] = {0};
int visit_map[200][200][10][4] = {0};

typedef struct  {
    int x, y;
    int cost;
    int delta[2];
    int direction_repeats;
}SearchPos;

SearchPos search_positions[25000];

int find_path( bool ultra_crucible, int width, int height ) {
    SearchPos initial_pos = { 0,0, 0, 0,0,0 };

    for( int y=0; y<height; y++) {
        for( int x=0; x<width; x++ ) {
            for( int s=0; s<10; s++ ) {
                for( int d=0; d<4;d++) visit_map[y][x][s][d] = 0;
            }
        }
    }
    int pending_searches = 0;
    search_positions[ pending_searches++ ] = initial_pos;
    while( pending_searches ) {
        // find next move with lowest cost
        int lowest_cost = INT_MAX;
        int lowest_idx = 0;
        for( int i=0; i<pending_searches; i++ ) {
            if( search_positions[i].cost < lowest_cost ) {
                lowest_cost = search_positions[i].cost;
                lowest_idx = i;
            }
        }

        SearchPos pos = search_positions[ lowest_idx ];
        search_positions[ lowest_idx ] = search_positions[ --pending_searches ];
        if( pos.x == width-1 && pos.y == height-1) {
            // found destination
            return pos.cost;
        }

        // find legal move_direction
        int move_direction[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        int max_steps = ultra_crucible ? 10 : 3;
        for( int move=0; move< 4; move++ ) {
            if(pos.delta[0] == move_direction[move][0] && pos.delta[1] == move_direction[move][1] && pos.direction_repeats >= max_steps ) {
                continue;
            }
            if( pos.delta[ 0 ]== -move_direction[move][0] && pos.delta[ 1 ] == -move_direction[move][1] ) {
                continue;
            }
            bool same_direction = (move_direction[move][0] == pos.delta[0] && move_direction[move][1] == pos.delta[1] );
            int new_y = pos.y;
            int new_x = pos.x;
            int entry_cost = 0;
            int steps = (ultra_crucible && !same_direction) ? 4 : 1;
            bool is_legal = true;
            for( int step=0; step<steps; step++ ) {
                new_y += move_direction[move][1];
                new_x += move_direction[move][0];
                if( new_x <0 || new_y < 0 || new_x >= width || new_y >= height ) {
                    is_legal = false;
                    break;
                }
                entry_cost += map[new_y][new_x];
            }
            if( !is_legal  ) continue;

            int total_cost = entry_cost + pos.cost;
            int direction_repeats = same_direction ? pos.direction_repeats + 1 : steps;
            if( visit_map[new_y][new_x][ direction_repeats-1 ][move] != 0 && visit_map[new_y][new_x][ direction_repeats-1 ][move] <= total_cost ) {
                continue;
            }
            visit_map[new_y][new_x][ direction_repeats-1 ][move] = total_cost;

            SearchPos new_search = {new_x, new_y, total_cost, move_direction[move][0], move_direction[move][1], direction_repeats };
            search_positions[ pending_searches++ ] = new_search;
        }
    }
}

int main(void) {
    FILE *fp = fopen("input17.txt", "r");
    int width;
    int height = -1;
    while( fgets( map[++height], 256, fp ) != NULL ){
        width = strlen( map[ height] );
        for( int x=0; x<width; x++ ) map[height][x] = map[height][x]-'0';
        map[height][width] = 0;
    }

    int part1 = find_path( false, width, height );
    printf( "Part 1 cost %d \n", part1 );

    int part2 = find_path( true, width, height );
    printf( "Part 2 cost %d \n", part2 );

}