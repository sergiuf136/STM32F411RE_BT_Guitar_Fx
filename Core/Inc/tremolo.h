#include "math.h"

#define COUNT_UP 1.0f
#define COUNT_DOWN -1.0f

void tremolo_init(float d, float lfo_freq_Hz, float samp_rate_Hz);

void tremolo_set_depth(float d);

void tremolo_set_LFO_frequency(float lfo_freq_Hz);

void tremolo_set_rate(float rate);

float tremolo_update(float input);

