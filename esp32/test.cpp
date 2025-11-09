/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-piezo-buzzer
 */

#include "pitchs.h"
#include <Arduino.h>

// ESP32 has different tone implementation
#define BUZZER_PIN 18  // GPIO18 pin connected to piezo buzzer
#define BUZZER_CHANNEL 0  // PWM channel to use

// Function to play a tone on ESP32
void playTone(uint8_t pin, unsigned int frequency, unsigned long duration) {
  if (frequency == 0) {
    noTone(pin);
    return;
  }
  ledcSetup(BUZZER_CHANNEL, frequency, 8); // 8 bit resolution
  ledcAttachPin(pin, BUZZER_CHANNEL);
  ledcWriteTone(BUZZER_CHANNEL, frequency);
  delay(duration);
  ledcWriteTone(BUZZER_CHANNEL, 0);
}

// notes in the melody:
int melody[] = {
  NOTE_G5, NOTE_G5, NOTE_G5, 0, 
  NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5,
};

// note durations: 4 = quarter note, 8 = eighth note, etc, also called tempo:
int noteDurations[] = {
  16, 16, 16, 12, 
  8, 8, 16, 16,
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting buzzer test...");
  
  // Configure PWM for the buzzer
  ledcSetup(BUZZER_CHANNEL, 5000, 8);
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
  
  // iterate over the notes of the melody:
  int size = sizeof(noteDurations) / sizeof(int);
  Serial.printf("Playing %d notes...\n", size);

  for (int thisNote = 0; thisNote < size; thisNote++) {
    Serial.printf("Playing note %d: frequency=%d\n", thisNote, melody[thisNote]);
    
    // to calculate the note duration, take one second divided by the note type.
    int noteDuration = 1000 / noteDurations[thisNote];
    playTone(BUZZER_PIN, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
  }
}

void loop() {
  // Play the melody again every second
  static unsigned long lastPlay = 0;
  if (millis() - lastPlay > 1000) {
    Serial.println("Playing melody again...");
    setup();  // This will play the melody again
    lastPlay = millis();
  }
}
