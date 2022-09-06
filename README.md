# STM32F411RE_BT_Guitar_Fx
Code files for my Bachelor's Degree in CompSci.

A Guitar Effects device designed for practice. 
Available audio effects: delay, tremolo and distortion. 
Delay and tremolo have configurable parameters through Bluetooth, using USART1 and a HC-05 module.

###

The device uses the onboard 12-bit ADC and a 16-bit DAC, PCM1781. The input data is shifted to match the 16-bit data required for the DAC.
