#include "backdrop.h"

void GetBackdrop(unsigned char **Backdrop, unsigned int *BackdropSize)
{
	*Backdrop = g_BackTex;
	*BackdropSize = 921656UL;
}