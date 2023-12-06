#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

typedef struct {
    uint64_t dest;
    uint64_t source;
    int count;
}MapRange;

MapRange mappers[100][100] = {0};
int num_maps = 0;

uint64_t map_seed( uint64_t seed, uint64_t* dist_to_next ) {
    uint64_t min_distance_to_next = LLONG_MAX;
    for(int map_no=0; map_no<num_maps; map_no++) {
        for( int mapper_no=0;;mapper_no++) {
                MapRange map_range = mappers[map_no][mapper_no];
            if( map_range.source <= seed && seed < map_range.source + map_range.count ) {
                // how far much further to the end of this run
                uint64_t distance_to_next = map_range.count - (seed - map_range.source);
                // keep track of smallest dist (required to solve part2 quickly)
                min_distance_to_next = distance_to_next < min_distance_to_next ? distance_to_next : min_distance_to_next; 
                // do the actual mapping
                seed -= map_range.source;
                seed += map_range.dest;
                break;
            }
            if( map_range.count == 0 )break;
        }            
    }
    if( dist_to_next ) *dist_to_next = min_distance_to_next;
    return seed;
} 

int main()
{
    FILE *fp = fopen( "input5.txt", "r");
    char line[256];
    
    // Read the seeds
    int num_seeds = -1;
    uint64_t seeds[100] = {0};
    fgets( line, 256, fp );
    char* pPos;   
    char* pEnd = &line[7];
    do {
        pPos = pEnd;
        seeds[ ++num_seeds] = strtoll( pPos, &pEnd, 10 );
    } while( pEnd != pPos );

    int num_ranges_in_map = 0;
    while ( fgets( line, 256, fp ) != NULL) {
        if( strchr( line, ':') ){   // Found new mapper
            num_ranges_in_map = 0;
            num_maps ++;
        } else if ( strlen(line)>1) {
            uint64_t source, dest, count;
            sscanf( line, "%"SCNu64" %"SCNu64" %"SCNu64, &source, &dest, &count );
            MapRange map_range = { source, dest,count };
            mappers[ num_maps-1 ][ num_ranges_in_map++] = map_range;
        } 
    }

    // Part 1. Push each seed through
    uint64_t lowest_location = LLONG_MIN;
    for( int seed_no = 0; seed_no<num_seeds; seed_no++) {
        uint64_t seed = map_seed( seeds[ seed_no], NULL );
        lowest_location = seed < lowest_location ? seed : lowest_location;
    }
    printf( "Lowest location (part1) %"PRIu64"\n" , lowest_location );

    // Push ranges through. ( each mapping returns the distance it is good for so we can skip all of them.
    lowest_location = LLONG_MIN;
    for( int seed_no = 0; seed_no<num_seeds; seed_no +=2 ) {
        uint64_t seed_range_pos = seeds[ seed_no  ];
        uint64_t seed_range_end = seed_range_pos + seeds[ seed_no+1];
        while( seed_range_pos < seed_range_end ) {
            uint64_t jump ;
            uint64_t seed = map_seed( seed_range_pos, &jump );
            // Ranges increase so only need to check first in a run
            lowest_location = seed < lowest_location ? seed : lowest_location;
            // .. skip all the rest
            seed_range_pos += jump;
        }
    }
    printf( "Lowest location (part 2) %"PRIu64"\n" , lowest_location );
}