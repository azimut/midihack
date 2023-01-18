#include "buffer.h"
#include <assert.h>
#include <math.h>
#include <time.h>

static unsigned char min(const unsigned char a, const unsigned char b) {
  return (a < b) ? a : b;
}

static unsigned char max(const unsigned char a, const unsigned char b) {
  return (a > b) ? a : b;
}

static unsigned char clamp(const unsigned char velocity) {
  return max(min(velocity, MIDI_MAX), MIDI_MIN);
}

static unsigned char randomize(const char velocity) {
  return clamp(velocity + (random() % 10) - 5);
}

static unsigned char avg(buffer buffer) {
  assert(buffer.size > 0);
  uint result = 0;
  for (int i = 0; i < buffer.size; ++i)
    result += buffer.queue[i];
  return clamp((unsigned char)round(((result * 1.0) / buffer.size)));
}

buffer buffer_new(void) {
  srandom(time(0));
  return (buffer){};
}

unsigned char buffer_get(buffer buffer) {
  return (buffer.size == 0) ? randomize(MIDI_DEFAULT) : randomize(avg(buffer));
}

void buffer_expire(buffer *buffer, const uint now) {
  if ((now - buffer->last_seen) > BUFFER_LIFE)
    buffer->size = 0;
}

void buffer_add(buffer *buffer, const unsigned char velocity, const uint now) {
  buffer->last_seen = now;
  if (buffer->size == BUFFER_SIZE) {
    for (int i = 0; i < BUFFER_SIZE - 1; ++i) {
      buffer->queue[i] = buffer->queue[i + 1];
    }
    buffer->queue[BUFFER_SIZE - 1] = velocity;
  } else {
    buffer->queue[buffer->size] = velocity;
    buffer->size++;
  }
}
