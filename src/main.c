#include "buffer.h"
#include <alsa/asoundlib.h>
#include <alsa/seq.h>
#include <alsa/seq_event.h>
#include <stdio.h>
#include <stdlib.h>

#define MIDI_NAME "midihack"
#define MIDI_NOTE_TO_REPLACE 127

static snd_seq_t *seq_handle;
static int in_port;
static int out_port;

void midi_open(void) {
  printf("starting...");
  if ((snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_DUPLEX, 0)) < 0) {
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
  printf("done!\n");
}

int main(void) {
  midi_open();
  printf("listening...\n");
  snd_seq_event_t *ev = NULL;
  buffer buf = buffer_new();
  int err;
  while (snd_seq_event_input(seq_handle, &ev) >= 0) {

    buffer_expire(&buf, ev->time.tick);

    if (ev->type == SND_SEQ_EVENT_NOTEON) {
      if (ev->data.note.velocity == MIDI_NOTE_TO_REPLACE) {
        ev->data.note.velocity = buffer_get(buf);
        printf("replaced note `%d` velocity with `%d`\n", ev->data.note.note,
               ev->data.note.velocity);
      } else {
        buffer_add(&buf, ev->data.note.velocity, ev->time.tick);
      }
    }

    snd_seq_ev_set_source(ev, out_port);
    snd_seq_ev_set_subs(ev);
    snd_seq_ev_set_direct(ev);

    if ((err = snd_seq_event_output(seq_handle, ev)) < 0) {
      printf("written = %i (%s)\n", err, snd_strerror(err));
    }
    if ((err = snd_seq_drain_output(seq_handle)) < 0) {
      printf("drain = %i (%s)\n", err, snd_strerror(err));
    }
  }
  return -1;
}
