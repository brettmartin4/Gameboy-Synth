import pygame.midi
#import RPi.GPIO as GPIO
from time import sleep


BOTTOM_PIN = 22
TOP_PIN = 27
VERIFY_PIN = 21


def print_devices():
    for n in range(pygame.midi.get_count()):
        print (n,pygame.midi.get_device_info(n))


def number_to_note(number):
    notes = ['c', 'c#', 'd', 'd#', 'e', 'f', 'f#', 'g', 'g#', 'a', 'a#', 'b']
    return notes[number%12]


def send_note(note):
    note_conversion = {
        "RELEASE": '000000',
        "C3": '000001',
        "C#3": '000010',
        "D3": '000011',
        "D#3": '000100',
        "E3": '000101',
        "F3": '000110',
        "F#3": '000111',
        "G3": '001000',
        "G#3": '001001',
        "A3": '001010',
        "A#3": '001011',
        "B3": '001100',
        "C4": '001101',
        "C#4": '001110',
        "D4": '001111',
        "D#4": '010000',
        "E4": '010001',
        "F4": '010010',
        "F#4": '010011',
        "G4": '010100',
        "G#4": '010101',
        "A4": '010110',
        "A#4": '010111',
        "B4": '011000',
        "C5": '011001', # Middle C
        "C#5": '011010',
        "D5": '011011',
        "D#5": '011100',
        "E5": '011101',
        "F5": '011110',
        "F#5": '011111',
        "G5": '100000',
        "G#5": '100001',
        "A5": '100010',
        "A#5": '100011',
        "B5": '100100',
        "C6": '100101',
        "C#6": '100110',
        "D6": '100111',
        "D#6": '101000',
        "E6": '101001',
        "F6": '101010',
        "F#6": '101011',
        "G6": '101100',
        "G#6": '101101',
        "A6": '101110',
        "A#6": '101111',
        "B6": '110000',
        "C7": '110001',
        "C#7": '110010',
        "D7": '110011',
        "D#7": '110100',
        "E7": '110101',
        "F7": '110110',
        "F#7": '110111',
        "G7": '111000',
        "G#7": '111001',
        "A7": '111010',
        "A#7": '111011',
        "B7": '111100',
        "C8": '111101',
        "C#8": '111110',
        "D8": '111111',
    }

    send_note = note_conversion.get(note, '000000')

    #GPIO.output(VERIFY_PIN, 0)      # Verify pin set low while writing other pins
    #for i in range(6):
    #    GPIO.output(i+BOTTOM_PIN, int(send_note[i]))
    #GPIO.output(VERIFY_PIN, 1)      # Verify pin set high after other pins are written to

    return


def readInput(input_device):
    while True:
        if input_device.poll():
            event = input_device.read(1)[0]
            data = event[0]
            note_number = data[1]
            velocity = data[2]
            if velocity != 0:
                #print (number_to_note(note_number), velocity)
                ansi_note = pygame.midi.midi_to_ansi_note(note_number)
                #print (ansi_note)
                send_note(ansi_note)
            else:
                #print ("Note released")
                send_note("RELEASE")


if __name__ == '__main__':
    #GPIO.setmode(GPIO.BCM)             # choose BCM or BOARD
    #for i in range(BOTTOM_PIN,TOP_PIN+1):             # GPIO pins 22-27
    #    GPIO.setup(i, GPIO.OUT)           # set GPIO pins as outputs
    #GPIO.setup(VERIFY_PIN, GPIO.OUT)
    pygame.midi.init()
    my_input = pygame.midi.Input(2) # ID will be different on PI
    readInput(my_input)
