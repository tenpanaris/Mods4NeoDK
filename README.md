![image](https://github.com/user-attachments/assets/8ef71b16-ae78-4f1d-b284-6611cb648f99)


Experimental firmware for NeoDK board (https://github.com/Onwrikbaar/NeoDK)

The main purpose of this experiment is to add modulators to pulse width, voltage and frequency of the stim device, to see if they make for more interesting stimulation than just a frequency and pulse width.

To build, you will need to install the standard ST development tools: STM32CubeMX, STM32CubeIDE and whatever dependencies they have. Create a new project in STM32CubeMX with the exact microcontroller. You may be able to use my .ioc.  Copy the main.c main.h NeoDK.c and NeoDK.h files into your project and I think thats all you will need to compile. I haven't tried myself, but I believe you can upload the binary to the NeoDK over serial by holding the button down while powering on. 

Communication with the NeoDK is over USART, same as with the official firmware. 115200 baud 8N1. I'm using a FT232R based board that can also supply 5V to the NeoDK (which I intend to step up with a DC-DC boost board)

At this stage only the lowest level of functionality is implemented. A basic PC application (a Python Pyside6 made with Qtdesigner, so should run on any OS) called Burst Creator is included to craft bursts.

I was intending to add a pot to the NeoDK to control max voltage, but I think I will go with a software approach similar to ET312, where you can set and forget your preffered power level to min/med/max. I've just hardcoded the max level to 30% for now, to limit voltage on primary. 

Output is limited to just A and B outputs at the moment, acting like a single channel TENS.

-------------
Terminology
-------------
 * A 'pulse' is single on/off cycle, the smallest and lowest level of a burst
 * A 'burst' is a series of identical (except for modulation) pulses, defined by a duration, frequency, pulse_width, voltage, rest period after, and no_of_repeats. These are streamed from the PC to the NeoDK and kept in a small buffer. Bursts with special magic numbers are used for special purposes, like emergency stop, flush cache so next burst runs immediately etc.
 * Each aspect of a burst can be modulated, using a waveform function, like sine, triangle, sawtooth, square etc. Each modulator has a frequency and min/max values
 * A pattern is a sequence of bursts. These are defined in JSON and interpreted on the PC or ESP32, which will stream the resultant burst information to the NeoDK. They use input from the intensity knob, pushbutton and can do basic calculations with those inputs as well as random numbers.
 * A program generates patterns, and can use loops, random numbers, variables, inputs from middleware and internet. It allows remote control, response to bio sensors, etc. A program may just be a more fleshed out pattern.





---------------------------
basic description of loops
---------------------------
USART interrupt 
	accepts bursts, stores in burst_fifo_buffer (data size is fixed)
	check for magic packets. (TODO)
		Emergency stop clears FIFO and turns off outputs, and disables pulse interrupt. 
		"Do this now" packet clears fifo and this becomes the next packet, sets currently_in_a_burst to 0 so main loop will start on this burst immediately.
	Do flow control (TODO)
main while(1) loop 
	if not currently in a burst, dequeues off burst_fifo_buffer 
	as fast as it can loop around, and based on the time since burst started, it calculates the modulated values of volts, on_time, off_time, polarity 
  stores those values in pulse_running
  sets the voltage of the buck DAC
	loops around until the duration time of the burst is run out, then it pauses, then repeats if repititions >0. If not, sets currently_in_a_burst to 0, so next loop will dequeue 
pulse_timer_interrupt
	sets itself to re_run after pulse_current_action.on_time (or off_time)
	turns mosfets and triacs on/off (taking care of polarity)

-----------------------------

As a disclaimer, I am a hobbyist, not a professional. The code is amateur.
