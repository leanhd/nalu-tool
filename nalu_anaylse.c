#include <stdio.h>

#include "nalu_tool.h"


//////////////////////////////////////////////////////////////////////////
// dump information of one NALU
//////////////////////////////////////////////////////////////////////////
int  DumpOneNalu(Nalu *p_nalu)
{
	printf("This is No.%d NALU.\n",NALU_index);
	printf("NALU type: %d ,  NALU length: %d .\n",p_nalu->type,p_nalu->length);
	return 0;
}