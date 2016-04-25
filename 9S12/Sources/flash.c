/*******************************************************************
 *                    freescale semiconductor inc 
 *
 *    DESCRIPTION:   S12 single array Flash routines
 *    SOURCE:        flash.c
 *    COPYRIGHT:     © 04/2004   freescale
 *    AUTHOR:        rat579 
 *    REV. HISTORY:  09/2004   - Added support for multiple array devices
 *														 - Modified for MW standard stationery.
 *
 *
 *******************************************************************/
#include <mc9s12dp256.h>     /* derivative information */
														 /* the flash includes are common to all HCS12
														    devices including the DP256 should work
														    for all devices. */

#include "flash.h"					 /* include flash error information */

#define Flash_Sector_Size 0x400	 /* must be modified for particual device */
extern DoOnStack(unsigned int *far address);

#pragma MESSAGE DISABLE C1860		//Pointer conversion:Possible loss of data
//*****************************************************************************
//* Function Name    : Flash_Init
//* Description     : Initialize Flash NVM for HC9S12 by programming
//*                   FCLKDIV based on passed oscillator frequency, then
//*                      uprotect the array, and finally ensure PVIOL and
//*                      ACCERR are cleared by writing to them.
//*
//*****************************************************************************
void Flash_Init(unsigned long oscclk)
{
unsigned char fclk_val;
   
/* Next, initialize FCLKDIV register to ensure we can program/erase */
    if (oscclk >= 12000) {
        fclk_val = oscclk/8/200 - 1; /* FDIV8 set since above 12MHz clock */
        FCLKDIV |= fclk_val | FCLKDIV_PRDIV8_MASK;
    }
    else
    {
        fclk_val = oscclk/180 - 1;
        FCLKDIV |= fclk_val;
    }

//    FPROT = 0xFF; /* Disable all protection (only in special modes)*/
    FPROT = 0x18; /* Disable protection for 0x4000 - 0x4FFF (only in special modes)*/
    FSTAT |= (FSTAT_PVIOL|FSTAT_ACCERR);/* Clear any errors  */
}
//*****************************************************************************
//* Function Name    : Flash_Write_Word
//* Description     : Program a given Flash location using address and data
//*                   passed from calling function.
//*
//*****************************************************************************
signed char Flash_Write_Word(unsigned int *far_address,unsigned int data)
{
unsigned int *address;
    address = (unsigned int *)far_address;                        // strip page off
    FSTAT = FSTAT_ACCERR | FSTAT_PVIOL;
    if((unsigned int)address & 0x0001) {return Flash_Odd_Access;} // Aligned word?
    if(*far_address != 0xFFFF) {return Flash_Not_Erased;}         // Is it erased?
    //    FCNFG = 0x00;
    (*far_address) = data;            // Store desired data to address being programmed

    FCMD = 0x20;                     // Store programming command in FCMD
    (void)DoOnStack(far_address);
    if (FSTAT_ACCERR) {return Access_Error;}
    if (FSTAT_PVIOL) {return Protection_Error;}
    return 1;
}
//*****************************************************************************
//* Function Name    : Flash_Erase_Sector
//* Description     : Erases a given Flash sector using address
//*                   passed from calling function.
//*
//*****************************************************************************
signed char Flash_Erase_Sector(unsigned int *far_address)
{
unsigned int *address;
    address = (unsigned int *)far_address;                        // strip page off
    if((unsigned int)address & 0x0001) {return Flash_Odd_Access;} // Aligned word?
    if((unsigned int)address % Flash_Sector_Size !=0) {return Not_StartofSector_Error;}
  FCNFG = 0x00;
    FSTAT = (FSTAT_ACCERR | FSTAT_PVIOL);               //   clear errors
    (*far_address) = 0xFFFF;    /* Dummy store to page to be erased */

    FCMD=0x40;
    (void)DoOnStack(far_address);
    if (FSTAT_ACCERR) {return Access_Error;}
    if (FSTAT_PVIOL) {return Protection_Error;}
    return 1;
}
#pragma MESSAGE DEFAULT C1860        // Pointer conversion: Possible loss of data     