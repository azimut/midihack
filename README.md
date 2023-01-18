# midihack

Catches midi notes with velocity equal to `127`. By replacing them with velocity `60`.

## Usage

```console
$ make
$ ./midihack
```

## References

- C API Documentation https://www.alsa-project.org/alsa-doc/alsa-lib/seq.html
- C API
  - https://www.alsa-project.org/alsa-doc/alsa-lib/group___seq_middle.html
  - https://www.alsa-project.org/alsa-doc/alsa-lib/group___sequencer.html
  - https://www.alsa-project.org/alsa-doc/alsa-lib/group___seq_event.html
- Example: Using alsamidi + gtk https://github.com/alex-milanov/midi-pipes/
- Example: http://fundamental-code.com/midi/
