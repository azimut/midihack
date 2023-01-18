#include <alsa/asoundlib.h>
#include <alsa/seq.h>
#include <alsa/seq_event.h>
#include <stdio.h>
#include <stdlib.h>

#define MIDI_NAME "midihack"
#define MIDI_DEFAULT 60
#define MAX_MIDI_VELOCITY 127

static snd_seq_t *seq_handle;
static int in_port;
static int out_port;

void midi_open(void) {
  if ((snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_INPUT, 0)) < 0) {
    fprintf(stderr, "error: midi_open:snd_seq_open\n");
    exit(1);
  }
  if ((snd_seq_set_client_name(seq_handle, MIDI_NAME)) < 0) {
    fprintf(stderr, "error: midi_open:snd_seq_set_client_name\n");
    exit(1);
  }
  if ((in_port = snd_seq_create_simple_port(
           seq_handle, "in",
           SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
           SND_SEQ_PORT_TYPE_APPLICATION)) < 0) {
    fprintf(stderr, "error: midi_open:snd_seq_create_simple_port in_port\n");
    exit(1);
  }
  if ((out_port = snd_seq_create_simple_port(
           seq_handle, "out",
           SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
           SND_SEQ_PORT_TYPE_APPLICATION)) < 0) {
    fprintf(stderr, "error: midi_open:snd_seq_create_simple_port out_port\n");
    exit(1);
  }
}

int main(void) {
  midi_open();
  snd_seq_event_t *ev = NULL;
  while (snd_seq_event_input(seq_handle, &ev) >= 0) {
    if (ev->type == SND_SEQ_EVENT_NOTEON &&
        ev->data.note.velocity == MAX_MIDI_VELOCITY) {
      printf("converted! note=%d\n", ev->data.note.note);
      ev->data.note.velocity = MIDI_DEFAULT;
    }
  }
  return -1;
}
