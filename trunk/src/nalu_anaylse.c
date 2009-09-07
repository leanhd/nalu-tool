#include <stdio.h>

#include "nalu_tool.h"


//////////////////////////////////////////////////////////////////////////
// dump information of one NALU
//////////////////////////////////////////////////////////////////////////
int  DumpOneNalu(Nalu *p_nalu)
{
	printf("%4d: ",         p_nalu->index);
	printf("type = %2d, ",  p_nalu->type);
	printf("length= %d.",   p_nalu->length);
	printf("\n");
	return 0;
}