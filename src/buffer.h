#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>

#define BUFFER_SIZE 10
#define BUFFER_LIFE 2
#define MIDI_DEFAULT 40
#define MIDI_MAX 127
#define MIDI_MIN 0

typedef struct {
  unsigned char queue[BUFFER_SIZE];
  int size;
  uint last_seen;
} Buffer;

Buffer buffer_new(void);
unsigned char buffer_get(Buffer buffer);
void buffer_add(Buffer *buffer, const unsigned char velocity, const uint now);
void buffer_expire(Buffer *buffer, const uint now);

#endif /* BUFFER_H */
