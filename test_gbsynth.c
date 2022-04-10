#include <gb/gb.h>
#include <stdio.h>
#include <stdint.h>


const uint16_t noteFreq[73] = {0, 44, 156, 262, 363, 457, 547, 631, 710, 786, 854, 923, 986, 1046, 1102, 1155, 1205, 1253, 1297, 1339, 1379, 1417, 1452, 1486, 1517, 1546, 1575, 1602, 1627, 1650, 1673, 1694, 1714, 1732, 1750, 1767, 1783, 1798, 1812, 1825, 1837, 1849, 1860, 1871, 1881, 1890, 1899, 1907, 1915, 1923, 1930, 1936, 1943, 1949, 1954, 1959, 1964, 1969, 1974, 1978, 1982, 1985, 1988, 1992};

//const uint8_t waveTable[32] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

uint8_t activeChannel = 0;


void set_note(uint8_t value)
{
	uint8_t note = value & 0x3F;
	uint8_t freqMSB = ((0x0700 & noteFreq[value]) >> 8);
	uint8_t freqLSB = noteFreq[value] & 0xFF;
	uint8_t lenEn = NR14_REG & 0x40;

    if(note == 0) {
        // Key release
		NR51_REG = 0x00;
    } else {
		// Key press
		NR51_REG = 0xFF;
		if (activeChannel == 0) {
        	NR13_REG = freqLSB;
			NR14_REG = 0x80 | lenEn | freqMSB;
		} else if (activeChannel == 1) {
			NR23_REG = freqLSB;
			NR24_REG = 0x80 | lenEn | freqMSB;
		} else if (activeChannel == 2) {
			NR33_REG = freqLSB;
			NR34_REG = 0x80 | lenEn | freqMSB;
		} else {
			NR44_REG = 0x80 | lenEn;
		}
    }

    return;
}


void set_channel(uint8_t value) {
	switch(value) {
		case 0: // Set channel 1 active
			activeChannel = 0;
			break;
		case 1:// Set channel 2 active
			activeChannel = 1;
			break;
		case 2:// Set channel 3 active
			activeChannel = 2;
			break;
		case 3:// Set channel 4 active
			activeChannel = 3;
			break;
		default:
			activeChannel = 0;
			break;
	}
	return;
}


void set_param(uint8_t value) {
	switch(value) {
		case 2: // Len Enable On
			NR14_REG = NR14_REG | 0x40;
			NR24_REG = NR24_REG | 0x40;
			NR34_REG = NR34_REG | 0x40;
			NR44_REG = NR44_REG | 0x40;
			break;
		case 3: // Len Enable Off
			NR14_REG = NR14_REG & 0xBF;
			NR24_REG = NR24_REG & 0xBF;
			NR34_REG = NR34_REG & 0xBF;
			NR44_REG = NR44_REG & 0xBF;
			break;
		case 4: // Envelope Add On
			NR12_REG = NR12_REG | 0x08;
			NR22_REG = NR22_REG | 0x08;
			NR42_REG = NR42_REG | 0x08;
			break;
		case 5: // Envelope Add Off
			NR12_REG = NR12_REG & 0xF7;
			NR22_REG = NR22_REG & 0xF7;
			NR42_REG = NR42_REG & 0xF7;
			break;
		case 6: // Negate On
			NR10_REG = NR10_REG | 0x08;
			break;
		case 7: // Negate Off
			NR10_REG = NR10_REG & 0xF7;
			break;
		case 8: // Width Mode LFSR On
			NR43_REG = NR43_REG | 0x08;
			break;
		case 9: // Width Mode LFSR Off
			NR43_REG = NR43_REG & 0xF7;
			break;
		default:
			// Do nothing
			break;
	}
	return;
}


void set_sw(uint8_t value)
{
	uint8_t channel_sw = value & 0x30;
	uint8_t sw_val = value & 0x0F;

	if (channel_sw == 0x30) {
		set_channel(sw_val);
	} else {
		set_param(sw_val);
	}

    return;
}


void set_pot(uint8_t value)
{
    uint8_t pot_id = value & 0x38;
	uint8_t pot_val = value & 0x07;

	switch(pot_id) {
		case 0x00: // Update Volume
			NR12_REG = (pot_val<<5) | (0x0F & NR12_REG);
			NR22_REG = (pot_val<<5) | (0x0F & NR22_REG);
			NR42_REG = (pot_val<<5) | (0x0F & NR42_REG);
			NR32_REG = ((pot_val<<4) & 0x60) | (0x0F & NR32_REG);
			break;
		case 0x08: // Update Length Load
			NR11_REG = (NR11_REG & 0xC0) | ((pot_val<<3) & 0x38);
			NR21_REG = (NR21_REG & 0xC0) | ((pot_val<<3) & 0x38);
			NR31_REG = ((pot_val<<5) & 0xE0);
			NR41_REG = (NR41_REG & 0xC0) | ((pot_val<<3) & 0x38);
			break;
		case 0x10: // Update Period
			NR12_REG = (NR12_REG & 0xF8) | pot_val;
			NR22_REG = (NR22_REG & 0xF8) | pot_val;
			NR42_REG = (NR42_REG & 0xF8) | pot_val;
			break;
		case 0x18: // Update Duty
			NR11_REG = (NR11_REG & 0x3F) | ((pot_val<<5) & 0xC0);
			NR21_REG = (NR21_REG & 0x3F) | ((pot_val<<5) & 0xC0);
			break;
		case 0x20: // Update Sweep Pad
			NR10_REG = (NR10_REG & 0x8F) | ((pot_val<<4) & 0x70);
			break;
		case 0x28: // Update Shift
			NR10_REG = (NR10_REG & 0xF8) | pot_val;
			break;
		case 0x30: // Update Clock Shift
			NR43_REG = (NR43_REG & 0x0F) | ((pot_val<<5) & 0xE0);
			break;
		case 0x38: // Update Divisor Code
			NR43_REG = (NR43_REG & 0xF8) | pot_val;
			break;
		default:
			// Do nothing
			break;
	}

    return;
}


void apply_ser_val(uint8_t data) 
{
	uint8_t enc = data & 0x38;

	switch(enc) {
		case 0x00:
			set_note(data);
			break;
		case 0x40:
			set_sw(data);
			break;
		case 0x80:
			set_pot(data);
			break;
		default:
			// Do nothing
			break;
	}
    return;
}


int main(void)
{

	// these registers must be in this specific order!
    NR52_REG = 0x80; // is 1000 0000 in binary and turns on sound
    NR50_REG = 0x77; // sets the volume for both left and right channel just set to max 0x77
    NR51_REG = 0xFF; // is 1111 1111 in binary, select which chanels we want to use in this case all of them. One bit for the L one bit for the R of all four channels

    puts("                    ");
    puts(" Annexe Interactive ");
    puts("   GB Synth v1.3    ");
    puts("                    ");

    uint8_t recv_data;

    while(1)
    {
		// This while loop is modified from the GB library example file
        /* Receive 1 byte */
        printf("Receiving byte... \n");
        receive_byte();
        /* Wait for IO completion... */
        while(_io_status == IO_RECEIVING && joypad() == 0);
        if(_io_status == IO_IDLE)
	        printf("OK\n%d\n", _io_in); // _io_in: Byte just read after calling receive_byte()
        else
	        printf("#%d\n", _io_status);

        recv_data = _io_in;
        apply_ser_val(recv_data);

    }

    return 0;

}