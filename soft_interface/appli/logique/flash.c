/**
  ******************************************************************************
  * @file    flash.c
  * @author  GAILLARD Axel
  * @date    may-2024
  * @brief   flash used like eeprom functions.
  ******************************************************************************
*/
#include "flash.h"

/**
 * @brief write data in the flash
 * 
 * @param buffer 
 */
void flash_write_values(uint8_t *buffer, uint32_t size)
{
	if(size<SIZE_SECTOR)
	{
		for(uint32_t i = 0; i<size; i++)
			FLASH_set_word(i, (uint32_t)buffer[i]);
	}
}

/**
 * @brief read data in the flash
 * 
 * @param buffer 
 */
void flash_read_values(uint8_t *buffer, uint32_t size)
{
	if(size<SIZE_SECTOR)
	{
		for(uint32_t i = 0; i<size; i++)
			buffer[i] = (uint8_t)FLASH_read_word(i);
	}
}
