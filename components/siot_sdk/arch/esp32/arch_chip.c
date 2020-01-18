#include "arch_chip.h"

#include "nvs.h"
#include "nvs_flash.h"

void arch_chip_init(void)
{
	/* initialize flash memory */
	nvs_flash_init();
}