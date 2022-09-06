#include "delay.h"
#include <stdlib.h>

int DELAY_LENGTH = 29000;
int MIN_DELAY = 4000;
float *delay_buffer;
int readIndex, writeIndex;
float fb_gain;


void delay_init()
{
	delay_buffer = (float*)calloc(DELAY_LENGTH, sizeof(float));

	readIndex = 0;
	writeIndex = DELAY_LENGTH - 2;
}

void delay_update_time(int percent)
{
	int step = 250;
	// set delay length between 4000 and DELAY_LEnGTH
	DELAY_LENGTH = MIN_DELAY + step * percent;
	// Reset indexes in order not to access invalid memory zones.
	readIndex = 0;
	writeIndex = DELAY_LENGTH - 2;
}

void delay_update_fb_gain(int percent)
{
	// set gain from 1 to 100
	fb_gain = percent * 0.01f;
}

float delay_read_last()
{
    float ret = delay_buffer[readIndex];
    readIndex++;
    if(readIndex >= DELAY_LENGTH) readIndex = 0;
    return ret;
}

void delay_write_next(float inputSample)
{
	delay_buffer[writeIndex] = inputSample;
    ++writeIndex;
    if(writeIndex >= DELAY_LENGTH) writeIndex = 0;
}

float delay_apply(float currentSample) {
	float ret_val = (1-fb_gain) * currentSample + fb_gain * delay_read_last();
	delay_write_next(ret_val);
	return ret_val;
}
