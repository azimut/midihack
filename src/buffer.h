#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>

#define BUFFER_SIZE 10
#define BUFFER_LIFE 2
#define MIDI_DEFAULT 40

typedef struct {
  unsigned char queue[BUFFER_SIZE];
  size_t size;
  uint last_seen;
} Buffer;

Buffer buffer_create(void);
unsigned char buffer_get_velocity(Buffer buffer);
void buffer_add_velocity(Buffer *buffer, unsigned char velocity, uint now);
void buffer_expire(Buffer *buffer, uint now);

#endif /* BUFFER_H */
