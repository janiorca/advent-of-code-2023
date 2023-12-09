#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#define NUM_LETTERS ('Z'-'A'+1)
typedef struct {
    char name[4];
    int left;
    int right;
}Node;
Node nodes[ NUM_LETTERS*NUM_LETTERS*NUM_LETTERS] = {0};

int name_to_index( char* name ) {
    return (name[0]-'A')*NUM_LETTERS*NUM_LETTERS + (name[1]-'A')*NUM_LETTERS + (name[2]-'A');
}

int map_steps( char* start, char* end, char* instructions ) {
    int current_loc = name_to_index(start);
    int destination = name_to_index( end);
    char *current_instruction = instructions;
    int num_steps = 0;
    while( current_loc != destination) {
        current_loc = *current_instruction++ == 'L' ? nodes[ current_loc].left : nodes[ current_loc].right;
        if( *current_instruction != 'L' && *current_instruction != 'R') current_instruction = instructions;
        num_steps ++;
    };
    return num_steps;
}

uint64_t find_lcm( uint64_t a, uint64_t b ) {
    uint64_t current[ 2 ] = {0};
    for( ;; ) {
        if( current[0] < current[1] ) {
            current[0] += a;
        } else {
            current[1] += b;
        }
        if( current[0] == current[1]) return current[0];
    }
}

int find_loops( char* start, char* instructions ) {
    int current_loc = name_to_index(start);
    char *current_instruction = instructions;
    int num_steps = 0;
    for(;;) {
        // The puzzle is initially tried to solve. On reaching the xxZ node the chain loops back to the start. Neither
        // looping back to start or looping back to the start of the chain is given but the puzzle seems to guarantee it
        if( nodes[ current_loc ].name[2] == 'Z' ) {
            return num_steps;
        }
        current_loc = *current_instruction++ == 'L' ? nodes[ current_loc].left : nodes[ current_loc].right;
        if( *current_instruction != 'L' && *current_instruction != 'R') current_instruction = instructions;
        num_steps ++;
    };
}

int main()
{
    FILE *fp = fopen( "input8.txt", "r");
    char instructions[512];
    char line[512];
    char node_start_names[ 100 ][4];
    int num_start_nodes = 0;
    // Read the instructions
    fgets( instructions, 512, fp);

    // skip line
    fgets( line, 512, fp);
    // read the map
    while ( fgets( line, 256, fp ) != NULL) {
        Node node = {" ", name_to_index(&line[7]), name_to_index( &line[12])};
        strncpy( &node.name, line, 3);
        node.name[3] = 0;
        nodes[name_to_index(&line[0])] = node;
        if( line[2] == 'A') {
            memcpy ( node_start_names[ num_start_nodes++ ], node.name, 4 );
        }
    }

    // Follow the map
    int num_steps = map_steps( "AAA", "ZZZ", instructions );   //14893
    printf(  "Part1 %d steps \n", num_steps);

    uint64_t  lcm =1 ;
    for( int loop=0; loop< num_start_nodes; loop++ ) {
        uint64_t  loop_size = find_loops( node_start_names[ loop ], instructions );
        lcm = find_lcm( lcm, loop_size);
    }
    printf(  "Part2, loop %"PRIu64" steps \n", lcm);
}