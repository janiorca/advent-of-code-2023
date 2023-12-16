#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
    int value;
    char key[64];
    struct HashEntry* next;
}HashEntry;

HashEntry  entries[ 10000 ];

typedef struct {
    HashEntry* slots[256];
}HashMap;

int get_hash( char* word, int word_len ) {
    int value = 0;
    for( int idx=0; idx<word_len; idx++) {
        value += word[ idx ];
        value *= 17;
        value = value &0x00ff;
    }
    return value;
}

void hashmap_init(HashMap *hash_map ) {
    for( int idx =0; idx<256;idx ++ ) hash_map->slots[idx ] = NULL;
}

void hashmap_set(HashMap *hash_map, HashEntry *entry ) {
    int index = get_hash(entry->key, strlen(entry->key));
    HashEntry **next_ptr = &hash_map->slots[index];
    for (;;) {
        if (*next_ptr == NULL) {
            *next_ptr = entry;
            entry->next = NULL;
            return;
        } else if (strcmp((*next_ptr)->key, entry->key) == 0) {
            entry->next = (*next_ptr)->next;
            *next_ptr = entry;
            return;
        }
        next_ptr = &(*next_ptr)->next;
    }
}

void hashmap_delete( HashMap *hash_map, char* key  ) {
    int index = get_hash( key, strlen( key ) );
    HashEntry **next_ptr = &hash_map->slots[ index ];
    for( ;; ) {
        if( *next_ptr == NULL ) {
            return;
        } else if( strcmp( (*next_ptr)->key, key ) == 0 ) {
            (*next_ptr) = (*next_ptr)->next;
            return;
        }
        next_ptr = &(*next_ptr)->next;
    }
}

int main(void) {
    FILE *fp = fopen("input15.txt", "r");
    char word[256];
    int pos = 0;
    int hash_sum = 0;
    for (;;) {
        int code = fgetc(fp);
        if (code == EOF || code == ',') {
            hash_sum += get_hash( word, pos);
            pos = 0;
            if( code == EOF)  {
                break;
            }
        } else {
            word[pos++] = code;
        }
    }
    fclose( fp );
    printf( "(Part 1 ) hash sum = %d", hash_sum);   //

    HashMap hash_map;
    hashmap_init( &hash_map);
    int used_entries = 0;
    fp = fopen("input15.txt", "r");
    for (;;) {
        int code = fgetc(fp);
        if (code == EOF || code == ',') {
            word[ pos ] = 0;
            char *instruction = strchr( word, '=');
            if( instruction ) {
                *instruction = 0;
                strcpy( entries[ used_entries ].key, word );
                entries[ used_entries ].value = strtol( instruction+1, NULL, 10);
                hashmap_set( &hash_map, &entries[ used_entries++ ]);
            } else {
                char *instruction = strchr( word, '-');
                *instruction = 0;
                hashmap_delete( &hash_map, word);
            }
            pos = 0;
            if( code == EOF)  {
                break;
            }
        } else {
            word[pos++] = code;
        }
    }

    int total = 0;
    for( int box=0; box<256; box ++ ) {
        int slot_no = 1;
        HashEntry* curr = hash_map.slots[ box ];
        while( curr != NULL ) {
            total += (box+1)*slot_no*curr->value;
            curr = curr->next;
            slot_no +=1;
        }
    }
    printf( "(Part 2) Focusing power = %d", total);   //
}