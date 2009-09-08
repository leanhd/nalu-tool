#include <stdio.h>

#include "nalu_tool.h"

extern Param *p_param;

//////////////////////////////////////////////////////////////////////////
// dump information of one NALU
//////////////////////////////////////////////////////////////////////////
int  DumpOneNalu(Nalu *p_nalu)
{
	printf("%4d: ",         p_nalu->index);
	printf("type = %2d, ",  p_nalu->type);
	printf("length= %4d.",   p_nalu->length);
	printf("\n");
	return 0;
}

int ParseNaluHeader(Nalu *p_nalu)
{
		p_nalu->type = (p_nalu->buffer[4]) & 31;//00011111
		p_nalu->forbidden_zero_bit = (p_nalu->buffer[4]) & 128;
		p_nalu->nal_ref_idc = (p_nalu->buffer[4]) & 96;//01100000
		if(p_param->dump_flag == 1)
			DumpOneNalu(p_nalu);
		return 0;
}