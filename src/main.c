#include "buffer.h"
#include <alsa/asoundlib.h>
#include <alsa/seq.h>
#include <alsa/seq_event.h>
#include <stdio.h>
#include <stdlib.h>

static snd_seq_t *seq;
static int in_port;
static int out_port;

void midi_start(void) {
  printf("starting...");
  int err;
  if ((err = snd_seq_open(&seq, "default", SND_SEQ_OPEN_DUPLEX, 0)) < 0) {
    fprintf(stderr, "ERROR: open = %i (%s)\n", err, snd_strerror(err));
    exit(1);
  }
  if ((err = snd_seq_set_client_name(seq, "midihack")) < 0) {
    fprintf(stderr, "ERROR: name = %i (%s)\n", err, snd_strerror(err));
    exit(1);
  }
  if ((in_port = snd_seq_create_simple_port(
           seq, "in", SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
           SND_SEQ_PORT_TYPE_APPLICATION)) < 0) {
    fprintf(stderr, "ERROR: in_port = %i (%s)\n", in_port,
            snd_strerror(in_port));
    exit(1);
  }
  if ((out_port = snd_seq_create_simple_port(
           seq, "out", SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
           SND_SEQ_PORT_TYPE_APPLICATION)) < 0) {
    fprintf(stderr, "ERROR: out_port = %i (%s)\n", out_port,
            snd_strerror(out_port));
    exit(1);
  }
  printf("done!\n");
}

void midi_send(snd_seq_event_t *ev) {
  int err;
  snd_seq_ev_set_source(ev, out_port);
  snd_seq_ev_set_subs(ev);
  snd_seq_ev_set_direct(ev);
  if ((err = snd_seq_event_output(seq, ev)) < 0) {
    printf("written = %i (%s)\n", err, snd_strerror(err));
  }
  if ((err = snd_seq_drain_output(seq)) < 0) {
    printf("drain = %i (%s)\n", err, snd_strerror(err));
  }
}

int main(void) {
  midi_start();
  printf("listening...\n");
  snd_seq_event_t *ev = NULL;
  Buffer buf = buffer_new();
  const int midi_max = 127;

  while (snd_seq_event_input(seq, &ev) >= 0) {

    buffer_expire(&buf, ev->time.tick);

    if (ev->type == SND_SEQ_EVENT_NOTEON) {
      if (ev->data.note.velocity == midi_max) {
        ev->data.note.velocity = buffer_get(buf);
        printf("replaced note `%d` velocity with `%d`\n", ev->data.note.note,
               ev->data.note.velocity);
      } else {
        buffer_add(&buf, ev->data.note.velocity, ev->time.tick);
      }
    }

    midi_send(ev);
  }

  return -1;
}
