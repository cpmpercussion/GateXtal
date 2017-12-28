#include "MIDIUSB.h" // MIDI USB required.

#define NOTE_ON 0x09
#define NOTE_OFF 0x08

void HandleNoteOn(byte channel, byte note, byte velocity) {
  notecounter++;
  noteFreq = mtof(float(note));
  aSinFreq = noteFreq;// +lfoOutput;
  aSin.setFreq(aSinFreq);
  envelope.noteOn();
  //MODenvelope.noteOn();
  digitalWrite(LED, HIGH);
  lastNote = note;
}

void HandleNoteOff(byte channel, byte note, byte velocity) {
  notecounter--;
  if (note == lastNote) { //only turn voice off if it was the last note to be pushed that was released
    envelope.noteOff();
    //MODenvelope.noteOff();
    digitalWrite(LED, LOW);
  }
}

void usbmidiprocessing()
{
  midiEventPacket_t rx;
  do {
    rx = MidiUSB.read();
    if (rx.header != 0) {
      // IF NOTE ON WITH VELOCITY GREATER THAN ZERO
      if ((rx.header == NOTE_ON) && (rx.byte3 > 0)) {
        jitterfreq = 0;
        digitalWrite(LED, HIGH);
        HandleNoteOn(rx.byte1, rx.byte2, rx.byte3);
        //Serial.println(notecounter);
      }
      // IF NOTE ON W/ ZERO VELOCITY
      if ((rx.header == NOTE_ON) && (rx.byte3 == 0)) {
        HandleNoteOff(rx.byte1, rx.byte2, rx.byte3);
        //Serial.println(notecounter);
      }
      // IF USB NOTE OFF
      if (rx.header == NOTE_OFF) {
        HandleNoteOff(rx.byte1, rx.byte2, rx.byte3);
        //Serial.println(notecounter);
      } 
    }
  } while (rx.header != 0);
}
