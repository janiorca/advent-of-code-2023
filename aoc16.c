#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char map[200][200] = {0};
char visited[200][200] = {0};

typedef struct {
    int x, y;
    int dx, dy;
}RayPos;

int evaluate_ray( RayPos initial_ray, int width, int height  ) {
    RayPos ray_store[ 1000 ];
    for( int y=0; y<height; y++ ) {
        for( int x=0; x<width; x++ ) {
            visited[y][x] = 0;
        }
    }

    int stored_rays = 0;
    ray_store[ stored_rays++ ] =initial_ray;
    while( stored_rays > 0 ) {
        RayPos current = ray_store[ --stored_rays];
        while( current.x >=0 && current.y >=0 && current.x < width && current.y < height ) {
            char piece = map[current.y][current.x];
            switch( piece) {
                case '-':
                case '|':
                    if( visited[current.y][current.x] !=0 ) goto end_ray;
                    if( !( (current.dy == 0 && piece == '-' ) || (current.dx == 0 && piece == '|' ))) {
                        int temp = current.dy;
                        current.dy =current.dx;
                        current.dx =temp;
                        ray_store[ stored_rays ] = current;
                        ray_store[stored_rays].dx = -ray_store[stored_rays].dx;
                        ray_store[stored_rays].dy = -ray_store[stored_rays].dy;
                        ray_store[stored_rays].x += ray_store[stored_rays].dx;
                        ray_store[stored_rays].y += ray_store[stored_rays].dy;
                        stored_rays++;
                    }
                    break;
                case '\\':
                    if( current.dx != 0) {
                        current.dy = current.dx;
                        current.dx = 0;
                    } else {
                        current.dx = current.dy;
                        current.dy = 0;
                    }
                    break;
                case '/':
                    if( current.dx != 0) {
                        current.dy = -current.dx;
                        current.dx = 0;
                    } else {
                        current.dx = -current.dy;
                        current.dy = 0;
                    }
                    break;
            }
            visited[ current.y ][current.x] = 1;
            current.x += current.dx;
            current.y += current.dy;
        }end_ray:
        int t=0;
    }
    int total = 0;
    for( int y=0; y<height; y++ ) {
        for( int x=0;x<width; x++ ) {
            if( visited[y][x] != 0 ) {
                total += 1;
            }
        }
    }
    return total;
}

int main(void) {
    FILE *fp = fopen("input16.txt", "r");
    int width =0;
    int height = -1;
    while( fgets( map[++height], 256, fp ) != NULL );
    width = strlen( map[ height - 1] );

    RayPos initial_ray = {0,0, 1,0};
    int energy = evaluate_ray( initial_ray, width, height );
    printf("(Part1) Visited %d", energy);

    int max_energy = 0;
    for( int pos=0; pos<width; pos++ ) {
        RayPos initial_rays[4] = { {0,pos, 1,0},{width-1,pos, -1,0},
                                   {pos, 0,0,1},{pos, width-1, 0,-1} };
        for( int ray_idx=0; ray_idx<4; ray_idx++ ) {
            int energy = evaluate_ray( initial_rays[0], width, height );
            max_energy = energy > max_energy ? energy : max_energy;
        }
    }
    printf("(Part2) Max energy %d", max_energy);
}
