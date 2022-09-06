#include "distortion.h"

float distort(float initialSample)
{
	initialSample -= 0.5f; // center in 0
	initialSample *= 2.5f; // amplify by 2.5

	float processedSample = initialSample;

	float d_threshold = 0.1f

	if(abs(initialSample) > d_threshold)
		processedSample *= 2.0f;
	else
	{
		processedSample = initialSample * initialSample;
	    if(initialSample < 0.0f)
	    	processedSample = -processedSample;
	}

	if(processedSample > 0.45f)
		processedSample = 0.45f;
	if(processedSample < -0.45f)
		processedSample = -0.45f;

	return processedSample + 0.5; // shift back to range (0,1)
}
