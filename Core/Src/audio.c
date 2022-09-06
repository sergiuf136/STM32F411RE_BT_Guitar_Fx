#include "audio.h"
#include "i2s.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "dma.h"
#include "tremolo.h"
#include "delay.h"
#include "distortion.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>


#define BUFFER_SIZE (1024U)


/* Audio IN buffer */
static volatile uint16_t 	audioInBuffer[2*BUFFER_SIZE];

/* Audio OUT buffer is double the size of the IN buffer
 * because we need to duplicate the samples to have stereo.
 * The CODEC seems not to support MONO. */
static volatile uint16_t 	audioOutBuffer[2*2*BUFFER_SIZE];


/* Conversion time for a buffer */
static volatile uint16_t conv_time_us = 0U;

/* I2S transmission time for a buffer */
static volatile uint16_t i2s_time_us = 0U;

/* I2S transmission time for a buffer */
static volatile uint16_t filter_time_us = 0U;


static volatile uint32_t i2s_half = 0U;
static volatile uint32_t i2s_full = 0U;

static volatile uint32_t adc_half = 0U;
static volatile uint32_t adc_full = 0U;

#define INT_TO_FLOAT 1.0f / 65536.0f
#define FLOAT_TO_INT 65536.0f

extern struct Effect effects;

void Audio_Init(void)
{


	//INITIAL EFFECTS PARAMS
	effects.delay_on = 0;
	effects.trem_on = 1;
	effects.od_on = 0;
	effects.dt = 50;
	effects.df = 50;
	effects.tr = 10;
	effects.td = 40;

	tremolo_init(40.0f, 6.0f, 4*44108.0f);
	tremolo_set_rate(effects.tr);
	tremolo_set_depth(effects.td);

	delay_init(); // init number of samples in half a second 28k works
	delay_update_fb_gain(effects.df);

	HAL_TIM_Base_Start(&htim2);
	HAL_I2S_Transmit_DMA(&hi2s3,(uint16_t*)audioOutBuffer,2*2*BUFFER_SIZE);
}

void Audio_Step(void)
{
		/* Wait for half transfer to complete */
		while(i2s_half == 0U);
		i2s_half = 0U;

		// First half of the buffer
		for(int i = 0 ; i<BUFFER_SIZE; ++i)
		{
			float floatSample = INT_TO_FLOAT * (audioInBuffer[i] << 3);

			/* process data */
			if (effects.od_on) {
				floatSample = distort(floatSample);
			}
			if (effects.trem_on) {
				floatSample = tremolo_update(floatSample);
			}
			if (effects.delay_on) {
				floatSample = delay_apply(floatSample);
			}

			audioOutBuffer[2*i] = FLOAT_TO_INT * floatSample;
			audioOutBuffer[2*i+1] = audioOutBuffer[2*i];
		}

		// Wait for full i2s transfer
		while(i2s_full == 0U);
		while(i2s_full == 0U);
		i2s_full = 0U;

		// Second half of the buffer
		for(int i = BUFFER_SIZE; i<2*BUFFER_SIZE; ++i)
		{

			float floatSample = INT_TO_FLOAT * (audioInBuffer[i] << 3);

			/* process data */
			if (effects.od_on) {
				floatSample = distort(floatSample);
			}
			if (effects.trem_on) {
				floatSample = tremolo_update(floatSample);
			}
			if (effects.delay_on) {
				floatSample = delay_apply(floatSample);
			}

			audioOutBuffer[2*i] = FLOAT_TO_INT * floatSample;
			audioOutBuffer[2*i+1] = audioOutBuffer[2*i];
		}

}


void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
	adc_half = 1U;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	adc_full = 1U;
}

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
	i2s_half = 1U;
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)audioInBuffer, BUFFER_SIZE);

}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
	i2s_full = 1U;
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)(audioInBuffer + BUFFER_SIZE), BUFFER_SIZE);

}



