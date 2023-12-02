#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

int main()
{
    FILE *fp = fopen( "input2.txt", "r");
    char line[256];
    
    int total = 0;
    int sum_powers = 0;
    while ( fgets( line, 256, fp ) != NULL) {
        int min_required[ 3 ] = {0};    // the minimum number of cubes of each color required for the game
        int game_id;
        sscanf( line, "Game %d: ", &game_id );
        char* game_str = strchr( line, ':')+1;
        // Use strtok to split it the game string into sets 
        // Cant use it in the internal loop to further tokenize because strtok uses global state
        game_str = strtok( game_str, ";" );
        bool game_possible = true;
        while( game_str  ) {
            char* set_read_pos = game_str;
            for(;;) {
                const char* colors[3] = { "red", "green", "blue"};   // match the color order to the constraints order 
                int constraints[3] = { 12, 13, 14};
                int count;
                char color[64] ={0};
                sscanf( set_read_pos, " %d %s", &count, color );
                for( int color_idx =0; color_idx<3; color_idx ++ ) {
                    if( strncmp( color, colors[color_idx], strlen( colors[color_idx]))==0) {
                        if( count > constraints[ color_idx ] ) {
                            game_possible = false;
                        }
                        // Update the minimum number of cubes required of this color
                        min_required[ color_idx ] = count > min_required[ color_idx ] ? count :  min_required[ color_idx ];
                    }                    
                }
                // Move to the next color-count pair 
                set_read_pos = strchr( set_read_pos, ',');
                if( set_read_pos == NULL )break;
                set_read_pos ++;   // skip the separating ,
            }
            // Move on tp the next set
            game_str = strtok( NULL, ";");
        }
        if( game_possible )  total += game_id;
        sum_powers += min_required[0]*min_required[1]*min_required[2];
    }
    printf( "\nTotal of possible IDs is %d\n", total );
    printf( "\nSum of powers %d\n", sum_powers );
    fclose( fp );
}