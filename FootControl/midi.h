#ifndef __MIDI_H__
#define __MIDI_H__

#define MIDI_CHAN_1         0x00
#define MIDI_CHAN_2         0x01
#define MIDI_CHAN_3         0x02
#define MIDI_CHAN_4         0x03
#define MIDI_CHAN_5         0x04
#define MIDI_CHAN_6         0x05
#define MIDI_CHAN_7         0x06
#define MIDI_CHAN_8         0x07
#define MIDI_CHAN_9         0x08
#define MIDI_CHAN_10        0x09
#define MIDI_CHAN_11        0x0A
#define MIDI_CHAN_12        0x0B
#define MIDI_CHAN_13        0x0C
#define MIDI_CHAN_14        0x0D
#define MIDI_CHAN_15        0x0E
#define MIDI_CHAN_16        0x0F

#define MIDI_NOTE_ON        0x90 
#define MIDI_NOTE_OFF       0x80 

#define MIDI_POLY_TOUCH     0xA0 
#define MIDI_CONTROL_CHANGE 0xB0 
#define MIDI_PROGRAM_CHANGE 0xC0 
#define MIDI_CHANNEL_TOUCH  0xD0 
#define MIDI_PITCH_BEND     0xE0 

#define MIDI_DATA_MASK      0x7F 
#define MIDI_STATUS_MASK    0xF0 
#define MIDI_CHANNEL_MASK   0x0F 

#define MIDI_BAUD_RATE      31250 

#endif // __MIDI_H__
