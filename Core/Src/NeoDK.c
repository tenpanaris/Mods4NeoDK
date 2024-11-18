#include "NeoDK.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

/*Some terminology:
 * A pulse is single on/off cycle, the smallest and lowest level of a burst
 * A burst is a series of identical (except for modulation) pulses, defined by a duration, frequency, pulse_width, voltage, rest period and no_of_repeats. These are streamed from the PC to the NeoDK and kept in a small buffer. Bursts with special magic numbers are used for special purposes, like emergency stop, flush cache so next burst runs immediately etc.
 * Each aspect of a burst can be modulated, using a waveform function, like sine, triangle, sawtooth, square etc. Each modulator has a frequency and min/max values
 * A pattern is a sequence of bursts. These are defined in JSON and interpreted on the PC or ESP32, which will stream the resultant burst information to the NeoDK. They use input from the intensity knob, pushbutton and can do basic calculations with those inputs as well as random numbers.
 * A program generates patterns, and can use loops, random numbers, variables, inputs from middleware and internet. It allows remote control, response to bio sensors, etc. A program may just be a more fleshed out pattern.
*/

//variables needed from CubeMX
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern DAC_HandleTypeDef hdac1;
extern UART_HandleTypeDef hlpuart1;
extern DMA_HandleTypeDef hdma_lpuart1_rx;
extern DMA_HandleTypeDef hdma_lpuart1_tx;
extern TIM_HandleTypeDef htim14;


//GLOBAL VARIABLES
BURST_FIFO_Buffer burst_buffer;
_pulse_running pulse_running;
uint32_t 	tick_burst_started_at;
_burst USART_burst;
_burst current_burst;
uint8_t usart_buffer[50];
uint8_t in_a_burst = 0;			//0=false; 1=true
uint32_t LED_timer=0;
volatile uint16_t adc_buffer[4];
uint8_t rt_Msg[50] = "MSGNOTSET";
uint8_t rt_Msg_size=0;


#define VPRIM_MIN_mV     1202   //  1202 for Tokmas buck chip, 1064 for SGM 61410.
#define VPRIM_MAX_mV    10195   // 10195 for Tokmas, 10057 for SGM.

static uint16_t Vcap_mV_ToDacVal(uint16_t Vcap_mV)
{
    if (Vcap_mV < VPRIM_MIN_mV) Vcap_mV = VPRIM_MIN_mV;
    else if (Vcap_mV > VPRIM_MAX_mV) Vcap_mV = VPRIM_MAX_mV;
    return (uint16_t)(((1865 * (uint32_t)(VPRIM_MAX_mV - Vcap_mV)) + 2048) / 4096);
}


//Implement all the USER CODE sections of the CubeMX generated code here, to make testing on Nucleo board easier.
void Do_User_Code_Begin_While()
{
  //HAL_TIM_Base_Start_IT(&htim14);		//Tim14 set with /32 prescalar so should be 1us per clock.

  global_vars_init();   //TODO Run an Init() for our global variables. Check all variables, esp recently added ones, have been properly initialized.
  HAL_UARTEx_ReceiveToIdle_DMA(&hlpuart1, usart_buffer, USART_BUFFER_SIZE);
  __HAL_DMA_DISABLE_IT(&hdma_lpuart1_rx, DMA_IT_HT);
  HAL_ADCEx_Calibration_Start(&hadc1);
  //On NeoDK board: PA0= Current sense; PA1= capacitor bank voltage; PA6=battery voltage; PA7= potentiometer voltage)
  __HAL_ADC_DISABLE_IT(&hadc1, ADC_IT_EOC | ADC_IT_EOS | ADC_IT_OVR); //disable ADC interrupts
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&adc_buffer, 4);		//AFAIK, The DMA is going to cycle through the 4 ADC channels  indefinitely in the background, and we can just get the value from the buffer for the last sampled value, at any time.

  HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);
  HAL_Delay(50);	//TODO: Check if this is enough time for the ADC to calibrate and get the first batch of samples.

  //enable the buck
  HAL_GPIO_WritePin(BUCK_EN_GPIO_Port, BUCK_EN_Pin, GPIO_PIN_SET);


}

void Do_User_Code_While_1()
{
	uint32_t modulated_period=0;
	uint32_t modulated_pw=0;
	uint32_t modulated_v=0;
	uint32_t angle;
	uint32_t time_in_burst;
	uint16_t ADC_batt_voltage=0;
	uint16_t ADC_cap_voltage=0;
	uint16_t ADC_pot=0;
//	uint16_t ADC_current=0;
	uint32_t DAC_value=0;
	uint32_t loop_count=0;

    // ----------------------
	// This is the main loop.
	// ----------------------
	while (1)		//repeating the while(1) here so we don't end up jumping to this function from main.c every loop.
	{

		loop_count++;

		time_in_burst=HAL_GetTick()-tick_burst_started_at;  //how far along we are in the burst, in milliseconds
		ADC_batt_voltage=adc_buffer[2] / 31;		// 4096 = 3.3V. Voltage divider is 3:1 or 25%. so / 4096 * 3.3 * 4 = /31.03 (result in 0.1 volts, so 133 - 13.3V)
		ADC_cap_voltage=adc_buffer[1] / 31;
		ADC_pot=adc_buffer[3] / 41;		//A level from 0 to 100
//		ADC_current=adc_buffer[0];  //not sure on the scaling of this yet.

		//Set the output voltage. TODO: ramp the voltage up over time.
		ADC_pot=30;			//hard code this for now. Need to solder a lead onto PA4 on the NeoDK - not an easy task. Then connect that to pot - will need to be in box by then.
		DAC_value=Vcap_mV_ToDacVal((uint16_t)pulse_running.volts*ADC_pot);  //works out well, because pulse_running.volts is specified in 0.1V, so 15 = 1.5V. To convert to millivolts for the function, we would x100. But we have a 0 to 100 value from the pot, so if we multiply by that instead, we get our maximum voltage limited by the pot
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, DAC_value);

		//flash the LED so we know we're not frozen
		if (HAL_GetTick() > LED_timer) {
			LED_timer=HAL_GetTick()+500;
			HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
			rt_Msg_size=sprintf ((char*)rt_Msg,"Batt V is %u . \n",ADC_batt_voltage);
			uart_buffer_write(rt_Msg, rt_Msg_size);
			rt_Msg_size=sprintf ((char*)rt_Msg,"Cap V is %u . \n",ADC_cap_voltage);
			uart_buffer_write(rt_Msg, rt_Msg_size);
			HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);

			rt_Msg_size=sprintf ((char*)rt_Msg,"We did %u loops. \n",loop_count);
			uart_buffer_write(rt_Msg, rt_Msg_size);
			loop_count=0;
		}

		if (in_a_burst) {
			//has burst time finished?
			if ((time_in_burst) > (current_burst.duration+current_burst.pause_after))
			{
				if (current_burst.repetitions>0)
				{
					pulse_running.stopped=0;
					current_burst.repetitions--;
					tick_burst_started_at=HAL_GetTick();
					rt_Msg_size=sprintf ((char*)rt_Msg,"Repeating burst. ");
					uart_buffer_write(rt_Msg, rt_Msg_size);
				}	else
				{
					in_a_burst=0;
					rt_Msg_size=sprintf ((char*)rt_Msg,"Burst complete. ");
					uart_buffer_write(rt_Msg, rt_Msg_size);
					continue;
				}
			}
			else { //still in burst, but are in pause period at end?
				if ((time_in_burst) > current_burst.duration)
				{
					//we are in the pause after burst. This will run multiple times throughout the pause,
					pulse_running.stopped=1;
				} else
				{
					// Do the modulations

					//modulate period (frequency)
					angle=(time_in_burst % current_burst.period_mod_freq);
					angle=angle*360;
					angle=angle / current_burst.period_mod_freq;

					//period_mod_waveform;	//0 = none, 1 = sine, 2 = sawtooth, 3 = triangle, 4 = square
					//modulated_period :
					switch (current_burst.period_mod_waveform)	{
							case 1: {
								modulated_period = fast_sine (angle);
								modulated_period=(current_burst.period_mod_max-current_burst.period_mod_min)*modulated_period;
								modulated_period=current_burst.period_mod_min+(modulated_period/1000);
								break;
							}
							case 2: {
								modulated_period = sawtooth_wave(angle);
								modulated_period=(current_burst.period_mod_max-current_burst.period_mod_min)*modulated_period;
								modulated_period=current_burst.period_mod_min+(modulated_period/1000);
								break;
							}
							case 3: {
								modulated_period = triangle_wave(angle);
								modulated_period=(current_burst.period_mod_max-current_burst.period_mod_min)*modulated_period;
								modulated_period=current_burst.period_mod_min+(modulated_period/1000);
								break; }
							case 4: {
								modulated_period = square_wave(angle);
								modulated_period=(current_burst.period_mod_max-current_burst.period_mod_min)*modulated_period;
								modulated_period=current_burst.period_mod_min+(modulated_period/1000);
								break; }
							default: {modulated_period=current_burst.period; }
					}

					//pulse_running.off_time=modulated_period-current_burst.pw;

					//modulate pulse width
					angle=(time_in_burst % current_burst.pw_mod_freq);
					angle=angle*360;
					angle=angle / current_burst.pw_mod_freq;

					//period_mod_waveform;	//0 = none, 1 = sine, 2 = sawtooth, 3 = triangle, 4 = square
					//modulated_period :
					switch (current_burst.pw_mod_waveform)	{
							case 1: {
								modulated_pw = fast_sine (angle);
								modulated_pw=(current_burst.pw_mod_max-current_burst.pw_mod_min)*modulated_pw;
								modulated_pw=current_burst.pw_mod_min+(modulated_pw/1000);
								break;
							}
							case 2: {
								modulated_pw = sawtooth_wave(angle);
								modulated_pw=(current_burst.pw_mod_max-current_burst.pw_mod_min)*modulated_pw;
								modulated_pw=current_burst.pw_mod_min+(modulated_pw/1000);
								break;
							}
							case 3: {
								modulated_pw = triangle_wave(angle);
								modulated_pw=(current_burst.pw_mod_max-current_burst.pw_mod_min)*modulated_pw;
								modulated_pw=current_burst.pw_mod_min+(modulated_pw/1000);
								break; }
							case 4: {
								modulated_pw = square_wave(angle);
								modulated_pw=(current_burst.pw_mod_max-current_burst.pw_mod_min)*modulated_pw;
								modulated_pw=current_burst.pw_mod_min+(modulated_pw/1000);
								break; }
							default: {modulated_pw=current_burst.pw; }
					}
					pulse_running.on_time=modulated_pw;
					pulse_running.off_time=modulated_period-modulated_pw;

					//modulate voltage
					//TODO voltage can't be changed quickly, so some limits may need to be imposed on the frequency of the modulator. Even 1Hz is probably too fast.
					angle=(time_in_burst % current_burst.v_mod_freq);
					angle=angle*360;
					angle=angle / current_burst.v_mod_freq;

					//waveform;	//0 = none, 1 = sine, 2 = sawtooth, 3 = triangle, 4 = square
					switch (current_burst.v_mod_waveform)	{
							case 1: {
								modulated_v = fast_sine (angle);
								modulated_v=(current_burst.v_mod_max-current_burst.v_mod_min)*modulated_v;
								modulated_v=current_burst.v_mod_min+(modulated_v/1000);
								break;
							}
							case 2: {
								modulated_v = sawtooth_wave(angle);
								modulated_v=(current_burst.v_mod_max-current_burst.v_mod_min)*modulated_v;
								modulated_v=current_burst.v_mod_min+(modulated_v/1000);
								break;
							}
							case 3: {
								modulated_v = triangle_wave(angle);
								modulated_v=(current_burst.v_mod_max-current_burst.v_mod_min)*modulated_v;
								modulated_v=current_burst.v_mod_min+(modulated_v/1000);
								break; }
							case 4: {
								modulated_v = square_wave(angle);
								modulated_v=(current_burst.v_mod_max-current_burst.v_mod_min)*modulated_v;
								modulated_v=current_burst.v_mod_min+(modulated_v/1000);
								break; }
							default: {modulated_v=current_burst.volts; }
					}
					pulse_running.volts=modulated_v;

					//TODO: modulate polarity
					// for now, I think polarity should be alternated by default, and then either 2 or 3 consecutive pulses of the same polarity and then the same number in reverse. Theoretically this could be achieved with one of the modulators
					// so if pol_mod_frequency:  1= -_-_-_,   2= --_ _--_ _--_ _,   3=  ---_ _ _---_ _ _---_ _ _. Limit to these 3 options for now.
					// Okay, based on the above decision, polarity modulation is done in the pulse interrupt, with a counter that counts down and resets to current_buffer.pol_mod_freq.
				}
			}
		} else
		{
			if (fifo_is_empty(&burst_buffer)){
				//nothing to do - make sure all outputs are off
				pulse_running.stopped=1;
				pulse_running.volts=5;
				while (pulse_running.currently_on) {};	//wait until interrupt timer turns off before disabling interrupt.
				HAL_TIM_Base_Stop_IT(&htim14);
				continue;
			} else
			{
				burst_fifo_dequeue(&burst_buffer, &current_burst);
				in_a_burst=1;
				tick_burst_started_at=HAL_GetTick();
				pulse_running.currently_on=0;
				pulse_running.on_time=current_burst.pw;
				pulse_running.off_time=current_burst.period-current_burst.pw;
				pulse_running.output_triacs=1; //AB
				pulse_running.polarity=current_burst.polarity;
				pulse_running.volts=current_burst.volts;
				pulse_running.stopped=0;

				HAL_TIM_Base_Start_IT(&htim14);

				strcpy((char*)rt_Msg, "Burst processing... ");
				uart_buffer_write(rt_Msg, 20);

			}
		}


	}
}



// ---------------------------------------------
// FIFO buffer for bursts coming in over USART
// ---------------------------------------------

// Initialize the FIFO buffer
void burst_fifo_init(BURST_FIFO_Buffer *fifo) {
    fifo->head = 0;
    fifo->tail = 0;
    fifo->count = 0;
}

// Check if the buffer is full
bool burst_fifo_is_full(BURST_FIFO_Buffer *fifo) {
    return fifo->count == BURST_FIFO_BUFFER_SIZE;
}

// Check if the buffer is empty
bool fifo_is_empty(BURST_FIFO_Buffer *fifo) {
    return fifo->count == 0;
}

// Add an item to the buffer (returns true if successful, false if buffer is full)
uint8_t rt_Ack2[5] = "ACK2";
bool burst_fifo_enqueue(BURST_FIFO_Buffer *fifo, _burst item) {
    if (burst_fifo_is_full(fifo)) {
        return false;  // Buffer overflow
    }
    fifo->buffer[fifo->head] = item;
    fifo->head = (fifo->head + 1) % BURST_FIFO_BUFFER_SIZE;
    fifo->count++;
	HAL_UART_Transmit_DMA(&hlpuart1, &rt_Ack2[0], 4);
    return true;
}

// Remove an item from the buffer (returns true if successful, false if buffer is empty)
bool burst_fifo_dequeue(BURST_FIFO_Buffer *fifo, _burst *item) {
    if (fifo_is_empty(fifo)) {
        return false;  // Buffer underflow
    }
    *item = fifo->buffer[fifo->tail];
    fifo->tail = (fifo->tail + 1) % BURST_FIFO_BUFFER_SIZE;
    fifo->count--;
    return true;
}




// ------------------------------
// This the main pulse generator
// ------------------------------
//It turns the outputs on, and then resets the timer for itself for the duration of the pulse width, then turns off and sets the timer for the off time.
//It handles polarity, and will turn off if pulse_running.stopped is set.
void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim)
{
	if (htim == &htim14) // pulse on/off timer
	{
		HAL_TIM_Base_Stop(&htim14);			//stop the timer. we will restart it manually later
		if (pulse_running.currently_on  || pulse_running.stopped)
		{
			//HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET); //simple feedback through LED for now. TODO: invent a better visual feedback system, possibly with bar LEDs.
			//switch off. just turn off triacs and Q1 and Q2
			HAL_GPIO_WritePin(TRIAC_1_GPIO_Port, TRIAC_1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(TRIAC_2_GPIO_Port, TRIAC_2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(TRIAC_3_GPIO_Port, TRIAC_2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(TRIAC_4_GPIO_Port, TRIAC_2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Q1_GPIO_Port, Q1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Q2_GPIO_Port, Q2_Pin, GPIO_PIN_RESET);

			pulse_running.currently_on=0;
			//restart Timer
			htim14.Instance->ARR = pulse_running.off_time;
			HAL_TIM_Base_Start(&htim14); HAL_TIM_Base_Start_IT(&htim14);
		} else
		{
			//HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_SET); //debugging //debugging
			//switch on

			if (! pulse_running.polarity_switch_count-- )
			{
				pulse_running.polarity_switch_count=current_burst.pol_mod_freq;
				pulse_running.polarity=pulse_running.polarity^1;
			}


			if (pulse_running.polarity)
			{
				HAL_GPIO_WritePin(Q2_GPIO_Port, Q2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Q1_GPIO_Port, Q1_Pin, GPIO_PIN_SET);

			} else
			{
				HAL_GPIO_WritePin(Q1_GPIO_Port, Q1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Q2_GPIO_Port, Q2_Pin, GPIO_PIN_SET);
			}

			// turn triacs on (for now just hardcode A and B). Triacs are wired active low, so reset pins to turn on.
			// not sure if this should be done here, as I assume the triacs will retrigger themselves as long as the optoisolating LED is on
			// If in future we do modulation between outputs, then we'd need to turn off the triacs and the mosfets to break the triac holding current, then turn on the triacs we want before turning the mosfets on.

			HAL_GPIO_WritePin(TRIAC_3_GPIO_Port, TRIAC_2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(TRIAC_4_GPIO_Port, TRIAC_2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(TRIAC_1_GPIO_Port, TRIAC_1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(TRIAC_2_GPIO_Port, TRIAC_2_Pin, GPIO_PIN_RESET);

			pulse_running.currently_on=1;

			//set the timer to trigger this interrupt again
			htim14.Instance->ARR = pulse_running.on_time;
			HAL_TIM_Base_Start(&htim14); HAL_TIM_Base_Start_IT(&htim14);
		}

	}
}






void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

	if (huart->Instance==LPUART1)
	{
		strcpy((char*)rt_Msg, "Got a packet. ");
		uart_buffer_write(rt_Msg, 14);

		if (Size==USART_BUFFER_SIZE)
		{
			if (1==1) 	//TODO: do checksum
			{
				if (burst_fifo_is_full(&burst_buffer))
				{
					strcpy((char*)rt_Msg, "Buffer full. Dropping data. ");
					uart_buffer_write(rt_Msg, 28);

					HAL_UARTEx_ReceiveToIdle_DMA(&hlpuart1, usart_buffer, USART_BUFFER_SIZE);
					  __HAL_DMA_DISABLE_IT(&hdma_lpuart1_rx, DMA_IT_HT);
					return;
				} else
				{
					enqueue_burst_from_usart();

					strcpy((char*)rt_Msg, "Adding to queue. ");
					uart_buffer_write(rt_Msg, 17);

					HAL_UARTEx_ReceiveToIdle_DMA(&hlpuart1, usart_buffer, USART_BUFFER_SIZE);
					  __HAL_DMA_DISABLE_IT(&hdma_lpuart1_rx, DMA_IT_HT);
					return;
				}
			} else
			{
				strcpy((char*)rt_Msg, "Failed Checksum. ");
				uart_buffer_write(rt_Msg, 17);

				HAL_UARTEx_ReceiveToIdle_DMA(&hlpuart1, usart_buffer, USART_BUFFER_SIZE);
				  __HAL_DMA_DISABLE_IT(&hdma_lpuart1_rx, DMA_IT_HT);
				return;
			}
		}
		else
		{
			//packet wrong size
			strcpy((char*)rt_Msg, "Bad sized packet. ");
			uart_buffer_write(rt_Msg, 18);

			HAL_UARTEx_ReceiveToIdle_DMA(&hlpuart1, usart_buffer, USART_BUFFER_SIZE);
			  __HAL_DMA_DISABLE_IT(&hdma_lpuart1_rx, DMA_IT_HT);
		}

	}

}


void global_vars_init()
{
	USART_burst.duration=0;
	USART_burst.pw=0;
	USART_burst.period=0;
	USART_burst.volts=0;
	USART_burst.polarity=0;
	USART_burst.v_mod_waveform=0;
	USART_burst.v_mod_freq=0;
	USART_burst.v_mod_min=0;
	USART_burst.v_mod_max=0;
	USART_burst.pw_mod_waveform=0;
	USART_burst.pw_mod_freq=0;
	USART_burst.pw_mod_min=0;
	USART_burst.pw_mod_max=0;
	USART_burst.period_mod_waveform=0;
	USART_burst.period_mod_freq=0;
	USART_burst.period_mod_min=0;
	USART_burst.period_mod_max=0;
	USART_burst.pol_mod_waveform=0;
	USART_burst.pol_mod_freq=0;
	USART_burst.repetitions=0;
	USART_burst.pause_after=0;

	pulse_running.volts=5;		// or 0.5 volts
	pulse_running.polarity=1;
	pulse_running.output_triacs=1;	//0=all off, 1="AB", 2="CD", 3="AD", 4="BC", 5="ABC", 6="ABD", 7="CDA", 8="CDB", 9="ABCD"
	pulse_running.on_time=0;
	pulse_running.off_time=65000;
	pulse_running.currently_on=0;	//0= false; 1=true
	pulse_running.stopped=1;
	pulse_running.polarity_switch_count=0;
}



void enqueue_burst_from_usart()
{
	USART_burst.duration = (uint32_t)usart_buffer[3] << 24 | (uint32_t)usart_buffer[2] << 16 | (uint32_t)usart_buffer[1] << 8 | (uint32_t)usart_buffer[0];
	USART_burst.pw=(uint8_t)usart_buffer[4];
	USART_burst.period=(uint16_t)usart_buffer[6] << 8 | (uint16_t)usart_buffer[5];
	USART_burst.volts=(uint8_t)usart_buffer[7];
	USART_burst.polarity=(uint8_t)usart_buffer[8];
	USART_burst.v_mod_waveform=(uint8_t)usart_buffer[9];
	USART_burst.v_mod_freq=(uint16_t)usart_buffer[11] << 8 | (uint16_t)usart_buffer[10];
	USART_burst.v_mod_min=(uint8_t)usart_buffer[12];
	USART_burst.v_mod_max=(uint8_t)usart_buffer[13];
	USART_burst.pw_mod_waveform=(uint8_t)usart_buffer[14];
	USART_burst.pw_mod_freq=(uint16_t)usart_buffer[16] << 8 | (uint16_t)usart_buffer[15];
	USART_burst.pw_mod_min=(uint8_t)usart_buffer[17];
	USART_burst.pw_mod_max=(uint8_t)usart_buffer[18];
	USART_burst.period_mod_waveform=(uint8_t)usart_buffer[19];
	USART_burst.period_mod_freq=(uint16_t)usart_buffer[21] << 8 | (uint16_t)usart_buffer[20];
	USART_burst.period_mod_min=(uint16_t)usart_buffer[23] << 8 | (uint16_t)usart_buffer[22];
	USART_burst.period_mod_max=(uint16_t)usart_buffer[25] << 8 | (uint16_t)usart_buffer[24];
	USART_burst.pol_mod_waveform=(uint8_t)usart_buffer[26];
	USART_burst.pol_mod_freq=(uint16_t)usart_buffer[28] << 8 | (uint16_t)usart_buffer[27];
	USART_burst.pause_after=(uint16_t)usart_buffer[30] << 8 | (uint16_t)usart_buffer[29];
	USART_burst.repetitions=(uint16_t)usart_buffer[32] << 8 | (uint16_t)usart_buffer[31];

	burst_fifo_enqueue (&burst_buffer, USART_burst);
}



// -----------------------------
// modulator waveform functions
// -----------------------------

#define TABLE_SIZE 361
const uint16_t sine_table[TABLE_SIZE] = {
		0, 9, 17, 26, 35, 44, 52, 61, 70, 78, 87, 96, 105, 113, 122, 131, 139, 148, 156, 165, 174, 182, 191, 199, 208, 216, 225, 233, 242, 250, 259, 267, 276, 284, 292, 301, 309, 317, 326, 334, 342, 350, 358, 367, 375, 383, 391, 399, 407, 415, 423, 431, 438, 446, 454, 462, 469, 477, 485, 492, 500, 508, 515, 522, 530, 537, 545, 552, 559, 566, 574, 581, 588, 595, 602, 609, 616, 623, 629, 636, 643, 649, 656, 663, 669, 676, 682, 688, 695, 701, 707, 713, 719, 725, 731, 737, 743, 749, 755, 760, 766, 772, 777, 783, 788, 793, 799, 804, 809, 814, 819, 824, 829, 834, 839, 843, 848, 853, 857, 862, 866, 870, 875, 879, 883, 887, 891, 895, 899, 903, 906, 910, 914, 917, 921, 924, 927, 930, 934, 937, 940, 943, 946, 948, 951, 954, 956, 959, 961, 964, 966, 968, 970, 972, 974, 976, 978, 980, 982, 983, 985, 986, 988, 989, 990, 991, 993, 994, 995, 995, 996, 997, 998, 998, 999, 999, 999, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 999, 999, 999, 998, 998, 997, 996, 995, 995, 994, 993, 991, 990, 989, 988, 986, 985, 983, 982, 980, 978, 976, 974, 972, 970, 968, 966, 964, 961, 959, 956, 954, 951, 948, 946, 943, 940, 937, 934, 930, 927, 924, 921, 917, 914, 910, 906, 903, 899, 895, 891, 887, 883, 879, 875, 870, 866, 862, 857, 853, 848, 843, 839, 834, 829, 824, 819, 814, 809, 804, 799, 793, 788, 783, 777, 772, 766, 760, 755, 749, 743, 737, 731, 725, 719, 713, 707, 701, 695, 688, 682, 676, 669, 663, 656, 649, 643, 636, 629, 623, 616, 609, 602, 595, 588, 581, 574, 566, 559, 552, 545, 537, 530, 522, 515, 508, 500, 492, 485, 477, 469, 462, 454, 446, 438, 431, 423, 415, 407, 399, 391, 383, 375, 367, 358, 350, 342, 334, 326, 317, 309, 301, 292, 284, 276, 267, 259, 250, 242, 233, 225, 216, 208, 199, 191, 182, 174, 165, 156, 148, 139, 131, 122, 113, 105, 96, 87, 78, 70, 61, 52, 44, 35, 26, 17, 9, 0
};

//these functions are doing integer math, so results are in percent, or 0 to 1000.
uint16_t fast_sine(uint16_t angle) {
    //angle %= 360;  // Handle angles > 360
    //if (angle < 0) angle += 360;  // Handle negative angles

       return sine_table[angle];

}

uint16_t triangle_wave(uint16_t angle) {
    //angle %= 360;  // Handle angles > 360
    //if (angle < 0) angle += 360;  // Handle negative angles
    if (angle <= 180) {
        return ((angle*100) / 18);
    } else {
        return ( (36000-(angle*100)) / 18 );
    }
}

uint16_t sawtooth_wave(uint16_t angle) {
    //angle %= 360;  // Handle angles > 360
    //if (angle < 0) angle += 360;  // Handle negative angles
        return ((angle*100) / 36);
}

uint16_t square_wave(uint16_t angle) {
    //angle %= 360;  // Handle angles > 360
   // if (angle < 0) angle += 360;  // Handle negative angles
    if (angle <= 180) {
        return (1000);
    } else {
        return (0) ;
    }
}







// ----------------
//  USART TX Stuff
// ----------------

#define TX_BUFFER_SIZE 64
uint8_t tx_buffer[TX_BUFFER_SIZE];
volatile uint16_t head = 0;
volatile uint16_t tail = 0;
volatile uint8_t dma_active = 0;  // Flag for DMA status
volatile uint16_t tx_size=0;

void uart_buffer_write(const uint8_t* data, uint16_t size) {
    for (uint16_t i = 0; i < size; i++) {
        tx_buffer[head] = data[i];
        head = (head + 1) % TX_BUFFER_SIZE;
        if (head == tail) { // Buffer full condition
            // Handle overflow (e.g., drop data, signal error, etc.)
        }
    }
    if (!dma_active) {
        start_uart_dma();
    }
}

void start_uart_dma()
{
    if (head != tail)
    { // Check if data is available
        dma_active = 1;
        tx_size = (head > tail) ? (head - tail) : (TX_BUFFER_SIZE - tail);
        HAL_UART_Transmit_DMA(&hlpuart1, &tx_buffer[tail], tx_size);
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    tail = (tail + tx_size) % TX_BUFFER_SIZE; // Update tail pointer
    if (tail != head) {
        start_uart_dma(); // Continue sending if more data remains
    } else
    {
        dma_active = 0;
    }
}



//#endif
