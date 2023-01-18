#ifndef BUFFER_H
#define BUFFER_H

#define BUFFER_SIZE 10
#define BUFFER_LIFE 2
#define MIDI_DEFAULT 40
#define MIDI_MAX 127
#define MIDI_MIN 0

typedef struct {
  unsigned char queue[BUFFER_SIZE];
  int size;
  unsigned int last_seen;
} buffer;

buffer *buffer_new(void);
unsigned char buffer_get(buffer buffer);
void buffer_add(buffer *buffer, unsigned char velocity, unsigned int now);
void buffer_expire(buffer *buffer, unsigned int now);

#endif /* BUFFER_H */
