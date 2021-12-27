/*
 * fft.h
 *
 *  Created on: Dec 25, 2021
 *      Author: u
 */

#ifndef INC_FFT_H_
#define INC_FFT_H_
#include "main.h"
#include "cmsis_os.h"

/*
 * div - по сколько данных отправлять за раз
 * (каждые сколько отсчетов будет пересчитываться fft)
 */
#define FFT_DIV 256

#define FFT_3KHZ_SIZE 1024	//размер FFT 3кгц
#define FFT_12KHZ_SIZE 512	//размер FFT 12кгц
#define FFT_20KHZ_SIZE 256	//размер FFT 20кгц


typedef struct fft1_bufers_struct
    {
	arm_rfft_fast_instance_f32 S;
	float fft_buf[FFT_3KHZ_SIZE]; //буфер входа
	float fft_in[FFT_3KHZ_SIZE]; //выборки
	float fft_out[FFT_3KHZ_SIZE * 2]; //результат
	float fft_res[FFT_3KHZ_SIZE]; //результат
	float fft_window[FFT_3KHZ_SIZE];
    } fft_3khz_buf;

typedef struct fft2_bufers_struct
    {
	arm_rfft_fast_instance_f32 S;
	float fft_buf[FFT_12KHZ_SIZE]; //буфер входа
	float fft_in[FFT_12KHZ_SIZE]; //выборки
	float fft_out[FFT_12KHZ_SIZE * 2]; //результат
	float fft_res[FFT_12KHZ_SIZE]; //результат
	float fft_window[FFT_12KHZ_SIZE];
    } fft_12khz_buf;

typedef struct fft3_bufers_struct
    {
	arm_rfft_fast_instance_f32 S;
	float fft_buf[FFT_20KHZ_SIZE]; //буфер входа
	float fft_in[FFT_20KHZ_SIZE]; //выборки
	float fft_out[FFT_20KHZ_SIZE * 2]; //результат
	float fft_res[FFT_20KHZ_SIZE]; //результат
    } fft_20khz_buf;

typedef struct analog_data
    {
	s16 data[FFT_DIV];
    } analog_data_t;

typedef struct in_analog_data
    {
	s16 data1[FFT_DIV];
	s16 data2[FFT_DIV];
	s16 data3[FFT_DIV];
    } in_analog_data_t;

typedef struct fft_stuff_struct
    {
	in_analog_data_t in_data;
	analog_data_t analog_data_3;
	analog_data_t analog_data_12;
	analog_data_t analog_data_20;
	QueueHandle_t fft_q;
	fft_3khz_buf fft_3;
	fft_12khz_buf fft_12;
	fft_20khz_buf fft_20;
    } fft_stuff_t;

void send_analog_data_from_isr(fft_stuff_t *fft, s16 in_data1, s16 in_data2,
	s16 in_data3);
void FFT_init(fft_stuff_t *fft);

#endif /* INC_FFT_H_ */
