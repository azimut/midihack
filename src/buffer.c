#include "buffer.h"
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <time.h>

static unsigned char min(const unsigned char a, const unsigned char b) {
  return (a < b) ? a : b;
}

static unsigned char max(const unsigned char a, const unsigned char b) {
  return (a > b) ? a : b;
}

static unsigned char clamp(const unsigned char velocity) {
  const int midi_max = 127;
  const int midi_min = 0;
  return max(min(velocity, midi_max), midi_min);
}

static unsigned char randomize(const char velocity) {
  return clamp(velocity + (random() % 10) - 5);
}

static unsigned char avg(Buffer buffer) {
  assert(buffer.size > 0);
  uint result = 0;
  for (size_t i = 0; i < buffer.size; ++i)
    result += buffer.queue[i];
  return clamp((unsigned char)round(((result * 1.0) / buffer.size)));
}

Buffer buffer_create(void) {
  srandom(time(0));
  return (Buffer){.size = 0, .last_seen = UINT_MAX};
}

unsigned char buffer_get_velocity(Buffer buffer) {
  return (buffer.size == 0) ? randomize(MIDI_DEFAULT) : randomize(avg(buffer));
}

void buffer_expire(Buffer *buffer, const uint now) {
  if ((now - buffer->last_seen) > BUFFER_LIFE) {
    buffer->size = 0;
    buffer->last_seen = UINT_MAX;
  }
}

void buffer_add_velocity(Buffer *buffer, const unsigned char velocity, const uint now) {
  buffer->last_seen = now;
  if (buffer->size == BUFFER_SIZE) {
    for (size_t i = 0; i < BUFFER_SIZE - 1; ++i) {
      buffer->queue[i] = buffer->queue[i + 1];
    }
    buffer->queue[BUFFER_SIZE - 1] = velocity;
  } else {
    buffer->queue[buffer->size] = velocity;
    buffer->size++;
  }
}
