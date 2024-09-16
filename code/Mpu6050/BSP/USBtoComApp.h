#ifndef USBCOMAPP_H
#define USBCOMAPP_H
#include  "stm32f1xx_hal.h"


void USBtoComApp(uint8_t *data,uint32_t len);
void USBtoComTask(void);
void USBtoComAppInit(void);
#endif

