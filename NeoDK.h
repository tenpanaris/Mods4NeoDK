

#ifndef __NEODK_H
#define __NEODK_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"

// FIFO BUFFER FOR BURSTS
#define BURST_FIFO_BUFFER_SIZE 10


typedef struct {
	uint32_t 	duration;				//time in milliseconds
	uint8_t		pw;						//on time in us. Master device (PC/ESP32) converts frequency and pulse_width to an on time and total time in microseconds. On is usually between 40 and 250 us.
	uint16_t	period;					//in us. not sure if off time or total time is better, might have to refactor later. This is just pulse to pulse, whether polarity changes or not, so not exactly analogous to AC period.
	uint8_t		volts;					// in .1 volts, eg 113 = 11.3V
	uint8_t		polarity;				// 0 or 1. Allows master device to define if the first pulse
	uint8_t		v_mod_waveform;			//0 = none, 1 = sine, 2 = sawtooth, 3 = triangle, 4 = square
	uint16_t	v_mod_freq;				//milliseconds. actually transmit these as period, so we don't have to do floating point math here, let the PC / ESP32 do it.
	uint8_t		v_mod_min;				//minimum voltage (in absolute terms, no sense multiplying burst.volts if we don't have to)
	uint8_t		v_mod_max;				//maximum voltage (in absolute terms, no sense multiplying burst.volts if we don't have to. should be the same as burst.volts though)
	uint8_t		pw_mod_waveform;		//0 = none, 1 = sine, 2 = sawtooth, 3 = triangle, 4 = square
	uint16_t	pw_mod_freq;			//milliseconds. actually transmit these as period, so we don't have to do floating point math here, let the PC / ESP32 do it.
	uint8_t		pw_mod_min;				//minimum pw (in absolute terms)
	uint8_t		pw_mod_max;				//maximum pw (in absolute terms. should be same as burst.pw)
	uint8_t		period_mod_waveform;	//0 = none, 1 = sine, 2 = sawtooth, 3 = triangle, 4 = square
	uint16_t	period_mod_freq;		//milliseconds. actually transmit these as period, so we don't have to do floating point math here, let the PC / ESP32 do it.
	uint16_t	period_mod_min;			//minimum deviation % (in absolute terms)
	uint16_t	period_mod_max;			//maximum deviation % (in absolute terms. should be the same as burst.period though)
	uint8_t		pol_mod_waveform;		//0 = none, 4 = square. Not used currently.
	uint16_t	pol_mod_freq;			//A multiple of burst.period, so you get even changes in polarity, s0 1= -_-_-_,   2= --_ _--_ _--_ _,   3=  ---_ _ _---_ _ _---_ _ _. Limit to these 3 options for now.
	uint16_t	pause_after;			//pause after burst. milliseconds
	uint16_t	repetitions;			//repeat this burst this many times (includes the pause)
} _burst ;		//size 33 bytes

#define USART_BUFFER_SIZE 33

// Define the FIFO buffer structure
typedef struct {
    _burst buffer[BURST_FIFO_BUFFER_SIZE];  // Array to hold data
    uint8_t head;                 // Points to the next insertion index
    uint8_t tail;                 // Points to the next item to remove
    uint8_t count;                // Number of items in the buffer
} BURST_FIFO_Buffer;

typedef struct {
	uint8_t 		volts;		//in 0.1 volts. eg 113 = 11.3V
	uint8_t 		polarity;
	uint8_t 		output_triacs;	//0=all off, 1="AB", 2="CD", 3="AD", 4="BC", 5="ABC", 6="ABD", 7="CDA", 8="CDB", 9="ABCD"
	uint8_t			on_time;
	uint16_t		off_time;
	uint8_t			currently_on;	//0= false; 1=true
	uint8_t			stopped;
	uint8_t			polarity_switch_count;
} _pulse_running;



//GLOBAL VARIABLES
extern BURST_FIFO_Buffer burst_buffer;
extern _pulse_running pulse_running;
extern uint32_t tick_burst_started_at;
extern _burst USART_burst;
extern _burst current_burst;
extern uint8_t in_a_burst;
extern uint32_t LED_timer;

extern uint8_t rt_ChkFail[11];
extern uint8_t rt_BufFull[11];
extern uint8_t rt_Ack[4];
extern uint8_t usart_buffer[50];

void Do_User_Code_Begin_While();
void Do_User_Code_While_1();

void burst_fifo_init(BURST_FIFO_Buffer *fifo);
bool burst_fifo_is_full(BURST_FIFO_Buffer *fifo);
bool fifo_is_empty(BURST_FIFO_Buffer *fifo);
bool burst_fifo_enqueue(BURST_FIFO_Buffer *fifo, _burst item);
bool burst_fifo_dequeue(BURST_FIFO_Buffer *fifo, _burst *item);

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
void global_vars_init();
void enqueue_burst_from_usart();

void uart_buffer_write(const uint8_t* data, uint16_t size);
void start_uart_dma();
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);

uint16_t fast_sine(uint16_t angle);
uint16_t triangle_wave(uint16_t angle);
uint16_t sawtooth_wave(uint16_t angle);
uint16_t square_wave(uint16_t angle);


#endif /* __NEODK_H */
