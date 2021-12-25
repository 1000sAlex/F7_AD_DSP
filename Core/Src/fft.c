/*
 * fft.c
 *
 *  Created on: Dec 25, 2021
 *      Author: u
 */
#include "fft.h"

void FFT_task(void *argument);

void FFT_init(fft_stuff_t *fft)
    {
    arm_rfft_init_q15(&fft->S, FFT_SIZE, 0, 1); //функция инициализации необходима для БФП
    xTaskCreate(FFT_task, "fft", configMINIMAL_STACK_SIZE * 2, fft,
	    osPriorityNormal, NULL);
    fft->fft_q = xQueueCreate(3, sizeof(analog_data_t));
    }

//отправить данные с ADAU в ффт
void send_analog_data_from_isr(fft_stuff_t *fft, s16 in_data)
    {
    static analog_data_t data;
    static u8 data_count = 0;
    BaseType_t xHigherPriorityTaskWoken;
    data.data[data_count++] = in_data;
    if (data_count == FFT_DIV)
	{
	data_count = 0;
	xQueueSendFromISR(fft->fft_q, &data, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
    }

void buf_data_add(q15_t *data, analog_data_t in)
    {
    for (u32 i = 0; i < FFT_SIZE - FFT_DIV; i++)
	{
	data[i] = data[i + FFT_DIV];
	}
    for (u32 i = 0; i < FFT_DIV; i++)
	{
	data[i + FFT_SIZE - FFT_DIV] = in.data[i];
	}
    }

extern UART_HandleTypeDef huart3;
void FFT_task(void *argument)
    {
    u32 send_count = 0;
    fft_stuff_t *fft = (fft_stuff_t*) argument;
    for (;;)
	{
	if (xQueueReceive(fft->fft_q, &fft->analog_data, 100) != pdFALSE)
	    {
	    buf_data_add(fft->fft_buf, fft->analog_data);
	    memcpy(fft->fft_in, fft->fft_buf, sizeof(fft->fft_in));
	    arm_rfft_q15(&fft->S, fft->fft_in, fft->fft_out); //выполнение БФП
	    arm_cmplx_mag_q15(fft->fft_out, fft->fft_res, FFT_SIZE); //вычисляем амплитуды гармоник
	    send_count++;
	    if (send_count == 3000)
		{
		send_count = 0;
//		//результат
		HAL_UART_Transmit(&huart3, fft->fft_res,
			sizeof(fft->fft_res) / 2, 0xFFFFFFFF);
//		HAL_UART_Transmit(&huart3, fft->fft_out,
//					sizeof(fft->fft_out), 0xFFFFFFFF);
		}
	    }
	else
	    {

	    }
	}
    }

