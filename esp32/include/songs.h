/*************************************************
 * Songs and Note Durations
 *************************************************/
#ifndef SONGS_H
#define SONGS_H

#include "pitchs.h"

// Number of songs available
#define SONGS_COUNT 2

// Song 0 - iPhone FindMy Beep Pattern
const int SONG0_NOTES[] = {
    NOTE_G5, NOTE_G5, NOTE_G5, 0, 
    NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5
};
const int SONG0_DURATIONS[] = {
    16, 16, 16, 12, 
    8, 8, 16, 16
};
const int SONG0_LENGTH = sizeof(SONG0_NOTES) / sizeof(int);

// Song 1 - Jingle Bells
const int SONG1_NOTES[] = {
    NOTE_E5, NOTE_E5, NOTE_E5,
    NOTE_E5, NOTE_E5, NOTE_E5,
    NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
    NOTE_E5,
    NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
    NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
    NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
    NOTE_D5, NOTE_G5
};
const int SONG1_DURATIONS[] = {
    8, 8, 4,
    8, 8, 4,
    8, 8, 8, 8,
    2,
    8, 8, 8, 8,
    8, 8, 8, 16, 16,
    8, 8, 8, 8,
    4, 4 
};
const int SONG1_LENGTH = sizeof(SONG1_NOTES) / sizeof(int);

// Arrays to make songs accessible by index
const int* const SONGS[] = { SONG0_NOTES, SONG1_NOTES };
const int* const DURATIONS[] = { SONG0_DURATIONS, SONG1_DURATIONS };
const int LENGTHS[] = { SONG0_LENGTH, SONG1_LENGTH };

// Simple functions to get song data
inline const int* get_song_notes(int index) {
    return SONGS[index % SONGS_COUNT];
}

inline const int* get_song_durations(int index) {
    return DURATIONS[index % SONGS_COUNT];
}

inline int get_song_length(int index) {
    return LENGTHS[index % SONGS_COUNT];
}

#endif
