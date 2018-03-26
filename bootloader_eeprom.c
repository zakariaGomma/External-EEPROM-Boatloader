/*
 * EEPROM_tEST.c
 *
 * Created: 2/13/2018 3:10:05 AM
 *  Author: Zakaria Gomaa
 
 */ 

#define F_CPU 16000000UL

/********************************** Includes **************************************/

#include <string.h> // memset function .
#include <stdint.h>

#include <avr/io.h>
#include <avr/boot.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

#include "External_EEPROM.h"
/********************************* Constants *****************************************/

//#define SPM_PAGESIZE          128      // 128 byte
#define EEPROM_BIN_START_LOC  0x0008
#define FLASH_ADD_ST          0x0000

/*************************************** Macros ***************************************/

#define EEPROM_Read_SPM_PAGE(addr , buf)   (EEPROM_Read_anylen(addr, SPM_PAGESIZE,buf))
#define EEPROM_Get_App_size()              (EEPROM_Read(0) | (EEPROM_Read(1)<<8))


/****************************************** Functions prototypes **********************/

void EEPROM_Read_anylen(uint16_t addr, uint16_t len, uint8_t *buf) ;
void boot_program_page (uint32_t page, uint8_t *buf) ;

/**************************************** Main Function ******************************/
int main(void)
{
  volatile uint8_t boot_buf[SPM_PAGESIZE] ; // buffer to store data read from EEprom to write it to flash .
  // EEprom_initilization 
  EEPROM_Init() ; 
  
  
  // Get bin file size 
  volatile uint32_t bin_size =  EEPROM_Get_App_size() ;
  
  uint16_t integ  = bin_size / SPM_PAGESIZE ;
  uint8_t remain  = bin_size%SPM_PAGESIZE ;
    EEPROM_Write(0,0);
    EEPROM_Write(1,0);
  uint16_t n ;
 if(integ)
  { // Read bin data from External EEprom and store it page by page .

    for(n = 0 ; n< integ ; n++ )
    {
      EEPROM_Read_SPM_PAGE(EEPROM_BIN_START_LOC+ (n<<7) , boot_buf ) ;  
      boot_program_page(FLASH_ADD_ST + (n<< 7) , boot_buf) ;
    }
  
  }//if

  if(remain)
         //read remainng bin data if founded if bin size > n*128 
 {
  memset(boot_buf , 0xFF , SPM_PAGESIZE) ; // As the remaining bytes are less than PAGE_SIZE
  EEPROM_Read_anylen(EEPROM_BIN_START_LOC + (n<<7) , remain , boot_buf ) ; // read remaining bytes from eeprom .
  boot_program_page(FLASH_ADD_ST + (n<< 7) , boot_buf) ;
 } 
 
  ( (void (*)(void)) FLASH_ADD_ST)() ; // Go to Application start address
}




/***************************************** Function definitions ****************************/

//This function used to read any length <= 128 byte from EEprom memory as EEprom page size is 16 bytes

void EEPROM_Read_anylen(uint16_t addr, uint16_t len, uint8_t *buf)   //len <=128 byte
{
	uint8_t n = 0 ;
	uint8_t integ =  len/EEPROM_PAGE_SIZE ;
	uint8_t remain = len%EEPROM_PAGE_SIZE ; 
	
  if(integ)
  {	
	for(n = 0 ; n< integ ; n++ )
	{
		EEPROM_Read_Page(addr +(n<<3) , buf+(n<<3) ) ;
	}
  }  	
	
	if(remain)
	   EEPROM_Read_Len(addr + (n<<3) , remain, buf+(n<<3) ) ; 
}


void boot_program_page (uint32_t page, uint8_t *buf)
    {
        uint16_t i;
        uint8_t sreg;

        // Disable interrupts.

        sreg = SREG;
        cli();
    
        eeprom_busy_wait ();

        boot_page_erase (page);
        boot_spm_busy_wait ();      // Wait until the memory is erased.

        for (i=0; i<SPM_PAGESIZE; i+=2)
        {
            // Set up little-endian word.

            uint16_t w = *buf++;
            w += (*buf++) << 8;
        
            boot_page_fill (page + i, w);
        }

        boot_page_write (page);     // Store buffer in flash page.
        boot_spm_busy_wait();       // Wait until the memory is written.

        // Reenable RWW-section again. We need this if we want to jump back
        // to the application after bootloading.

        boot_rww_enable ();

        // Re-enable interrupts (if they were ever enabled).

        SREG = sreg;
    }
