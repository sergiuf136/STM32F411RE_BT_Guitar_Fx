#include "tremolo.h"

float depth;
float lfoDir;
float lfoCount;
float MAX_LFO_COUNT;
float sampleRate_Hz;
float out;

float MIN_FREQ = 1;


void tremolo_init(float d, float lfo_freq_Hz, float samp_rate_Hz) {
	tremolo_set_depth(d);

	sampleRate_Hz = samp_rate_Hz;
	tremolo_set_LFO_frequency(lfo_freq_Hz);

	lfoCount = 0;
	lfoDir = COUNT_UP;

	out = 0.0f;
}

void tremolo_set_depth(float d) {
	d *= 0.01;

	if (d < 0.0f) {
		d = 0.0f;
	} else if (d > 1.0f) {
		d = 1.0f;
	}

	depth = d;
}

void tremolo_set_rate(float rate) {
	float step = 0.59f;
	tremolo_set_LFO_frequency(MIN_FREQ + step * rate);
}

void tremolo_set_LFO_frequency(float lfo_freq_Hz) {
	if (lfo_freq_Hz <= 0.0f) {
		lfo_freq_Hz = 1.0f;
	} else if (lfo_freq_Hz > 0.5f * sampleRate_Hz) {
		lfo_freq_Hz = 0.5f * sampleRate_Hz;
	}

	MAX_LFO_COUNT = 0.25f *  (sampleRate_Hz / lfo_freq_Hz);

	if (lfoCount > MAX_LFO_COUNT) {
		lfoCount = MAX_LFO_COUNT;
	} else if (lfoCount < -MAX_LFO_COUNT) {
		lfoCount = -MAX_LFO_COUNT;
	}
}

float tremolo_update(float input) {

	/* output = input * ((1-depth) + depth * lfo[t]) */
	out = input * ((1.0f - depth) + depth * (lfoCount / MAX_LFO_COUNT));

	if (lfoCount >= MAX_LFO_COUNT) {
		lfoDir = COUNT_DOWN;
	}
	if (lfoCount <= -MAX_LFO_COUNT) {
		lfoDir = COUNT_UP;
	}

	lfoCount += lfoDir;

	return out;
}
