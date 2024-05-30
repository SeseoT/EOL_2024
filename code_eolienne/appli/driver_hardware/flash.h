/*
 * flash.h
 *
 *  Created on: 25 mai 2024
 *      Author: axel GAILLARD
 */

#ifndef FLASH_H_
#define FLASH_H_
#define	SIZE_SECTOR		(1024/4)

#include "stm32f1_flash.h"

void flash_write_values(uint8_t *buffer, uint32_t size);

void flash_read_values(uint8_t *buffer, uint32_t size);

#endif /* FLASH_H_ */
