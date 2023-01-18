#include "buffer.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

static unsigned char min(unsigned char a, unsigned char b) {
  return (a < b) ? a : b;
}

static unsigned char max(unsigned char a, unsigned char b) {
  return (a > b) ? a : b;
}

static unsigned char clamp(unsigned char velocity) {
  return max(min(velocity, MIDI_MAX), MIDI_MIN);
}

static unsigned char randomize(const unsigned char velocity) {
  return clamp(velocity + (random() % 10) - 5);
}

static unsigned char avg(buffer buffer) {
  assert(buffer.size > 0);
  unsigned char result = 0;
  for (int i = 0; i < buffer.size; ++i) {
    result += buffer.queue[i];
  }
  return (unsigned char)round(((result * 1.0) / buffer.size));
}

buffer *buffer_new(void) {
  srandom(time(0));
  return &(buffer){};
}

unsigned char buffer_get(buffer buffer) {
  return (buffer.size == 0) ? randomize(MIDI_DEFAULT) : randomize(avg(buffer));
}

void buffer_expire(buffer *buffer, unsigned int now) {
  if ((now - buffer->last_seen) > BUFFER_LIFE)
    buffer->size = 0;
}

void buffer_add(buffer *buffer, unsigned char velocity, unsigned int now) {
  buffer->last_seen = now;
  if (buffer->size == BUFFER_SIZE) {
    for (int i = 0; i < BUFFER_SIZE; ++i) {
      buffer->queue[i] = buffer->queue[i + 1];
    }
  } else {
    buffer->queue[buffer->size] = velocity;
    buffer->size++;
  }
}
