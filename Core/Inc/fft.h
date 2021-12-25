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
#define FFT_DIV 32

#define FFT_SIZE 512	//размер FFT

typedef struct analog_data
    {
	s16 data[FFT_DIV];
    } analog_data_t;

typedef struct fft_stuff_struct
    {
	analog_data_t analog_data;
	arm_rfft_instance_q15 S;
	QueueHandle_t fft_q;
	q15_t fft_buf[FFT_SIZE]; //буфер входа
	q15_t fft_in[FFT_SIZE]; //выборки
	q15_t fft_out[FFT_SIZE * 2]; //результат
	q15_t fft_res[FFT_SIZE]; //результат
    } fft_stuff_t;

    void send_analog_data_from_isr(fft_stuff_t *fft, s16 in_data);
void FFT_init(fft_stuff_t *fft);

#endif /* INC_FFT_H_ */
