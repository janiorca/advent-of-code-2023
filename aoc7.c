#include "string.h"
#include <stdio.h>
#include <stdlib.h>

enum HandType{
    HIGH_CARD = 0,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    FIVE_OF_A_KIND,
};

typedef struct {
    enum HandType handType;
    int cards_values[5];
    int bid_amount;
}Hand;
Hand hands[1000] = {0};

int card_to_value(char c) {
    char* card_order = "23456789TJQKA";
    int idx = strchr( card_order, c) - card_order;
    return idx;
}

enum HandType classify_hand( Hand hand ) {
    int counts[13] = {0};
    for( int idx=0; idx<5; idx++) counts[ hand.cards_values[idx] ]++;
    int match_counts[6] = {0};
    for( int match_count=1; match_count<=5; match_count++ ) {
        for (int idx = 0; idx < 13; idx++) {
            if (counts[idx] == match_count) match_counts[match_count] += 1;
        }
    }
    if( match_counts[5] == 1) return FIVE_OF_A_KIND;
    if( match_counts[4] == 1) return FOUR_OF_A_KIND;
    if( match_counts[3] == 1 && match_counts[2] == 1) return FULL_HOUSE;
    if( match_counts[3] == 1) return THREE_OF_A_KIND;
    if( match_counts[2] == 2) return TWO_PAIR;
    if( match_counts[2] == 1) return ONE_PAIR;
    return HIGH_CARD;
}

enum HandType best_joker_hand( Hand hand, int card_idx ) {
    if( card_idx == 5 ) {
        return classify_hand( hand );
    }
    enum HandType best_hand_type = ONE_PAIR;
    if( hand.cards_values[ card_idx ] == 9 ) {      // If its a joker try all the different version
        for (int alt_value = 0; alt_value < 13; alt_value++) {
            Hand alt_hand = hand;
            alt_hand.cards_values[card_idx] = alt_value;
            enum HandType alt_hand_type = best_joker_hand(alt_hand, card_idx + 1);
            best_hand_type = alt_hand_type > best_hand_type ? alt_hand_type : best_hand_type;
        }
    } else {
        best_hand_type = best_joker_hand(hand, card_idx + 1);
    }
    return best_hand_type;
}

enum HandType classify_joker_hand( Hand hand ) {
    int num_jokers = 0;
    for (int idx = 0; idx < 13; idx++) {
        num_jokers += hand.cards_values[idx] == 'J' ? 1 : 0;
    }
    if( num_jokers >= 4 ) {
        return FIVE_OF_A_KIND;
    }
    return best_joker_hand(hand, 0);
}

int hand_sort( void *_left, void *_right ) {
    Hand* left = (Hand*)_left;
    Hand* right = (Hand*)_right;

    if( left->handType != right->handType ) {
        return left->handType - right->handType;
    }
    for( int card=0; card<5; card++ ) {
        if( left->cards_values[card] != right->cards_values[card] ) {
            return left->cards_values[card] - right->cards_values[card];
        }
    }
    return 0;
}

int count_winnings( Hand *hands, int num_hands ) {
    qsort(hands, num_hands, sizeof(Hand), hand_sort);
    int winnings = 0;
    for (int idx = 0; idx < num_hands; idx++) {
        winnings += (idx + 1) * hands[idx].bid_amount;
    }
    return winnings;
}

int main(void) {
    FILE *fp = fopen( "input7.txt", "r");
    char line[256];

    int num_hands = 0;
    while ( fgets( line, 256, fp ) != NULL) {
        for( int card_no=0; card_no<5; card_no++) {
            hands[ num_hands ].cards_values[card_no] = card_to_value(line[card_no]);
        }
        hands[ num_hands ].bid_amount = strtol( &line[6], 0, 10);
        hands[ num_hands++ ].handType = classify_hand( hands[ num_hands]);
    }
    int winnings = count_winnings( hands, num_hands );
    printf( "Part 1 Winnings %d \n", winnings);

    // Adjust the hands to account for new rules
    for( int idx =0; idx<num_hands; idx++ ) {
        hands[ idx ].handType = classify_joker_hand( hands[ idx ] );
        // remap the values to account for Joker's new drop in value
        int new_value[ 13 ] = { 1, 2,3,4,5,6,7,8,9, 0, 10, 11 ,12 };
        for( int card_no=0; card_no<5; card_no++ ) {
            hands[idx].cards_values[card_no] = new_value[ hands[idx].cards_values[card_no] ];
        }
    }
    winnings = count_winnings( hands, num_hands );
    printf( "Part 2 Winnings %d", winnings);
}
