#ifndef AUDIO_H_
#define AUDIO_H_

#define AUDIO_I2C_ADDRESS (0x94U)
#define AUDIO_FREQUENCY (8000U)
#define DECIMATION_FACTOR (64U)
#define N_PDM_1BIT_SAMPLES_PER_MS (AUDIO_FREQUENCY*DECIMATION_FACTOR)


void Audio_Init(void);
void Audio_Step(void);


#endif /* AUDIO_H_ */

struct Effect {
	int delay_on;
	int trem_on;
	int od_on;
	int dt; // delay time
	int df; // delay feedback
	int tr; // tremolo rate
	int td; // tremolo depth
};
