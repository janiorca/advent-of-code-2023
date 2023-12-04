#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct{
    int winning_nos[ 64 ];
    int drawn_nos[ 64 ];
}Card;

Card cards[1000] = {0};

int main()
{
    FILE *fp = fopen( "input4.txt", "r");
    char line[256];
    
    // Read all the cards
    int total_value = 0;
    int num_cards = 0;
    while ( fgets( line, 256, fp ) != NULL) {
        int card_no;
        // 0 is never a drawn number or a winning number so can use it as a terminator for 0-terminated lists
        Card *card = &cards[ num_cards++ ];
        sscanf(  line, "Card %d", &card_no);
        char* token = strtok( strchr( line, ':')+1, " ");
        int* dest = card->winning_nos;
        while( token ){
            if( token[0] == '|') {
                dest = card->drawn_nos;
            } else {
                int value = atoi( token );
                *dest++ = value;
            }
            token = strtok( NULL, " ");
        }
    }

    // Part 1 & part 2
    // Track the card counts. Initially there is only one card for each 
    int card_counts[ 1000 ] = {0};
    for( int i=0; i<num_cards; i++ ) card_counts[i] = 1;
    for( int card_no = 0; card_no<num_cards; card_no++) {
        Card *card = &cards[ card_no];
        int* next_drawn = card->drawn_nos;
        int draw_no = *next_drawn++;
        int num_matches = 0;
        while( draw_no ) {
            int* next_winning = card->winning_nos;
            int winning_no = *next_winning++;
            while( winning_no) {
                if( winning_no == draw_no) {
                    num_matches++;
                }
                winning_no = *next_winning++;
            }
            draw_no = *next_drawn++;
        }
        total_value += ( 1 << num_matches ) >> 1;
        for( int i=0; i< num_matches; i++ ) {
            card_counts[ i+card_no+1] += card_counts[card_no];
        }        
    }
    int total_scratch_cards = 0;
    for( int card_no =0; card_no<num_cards; card_no ++ )total_scratch_cards += card_counts[card_no]; 

    printf( "Total value %d\n", total_value);
    printf( "Won scratch cards %d\n", total_scratch_cards);
}