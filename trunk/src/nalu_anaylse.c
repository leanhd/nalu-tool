#include <stdio.h>
#include<stdlib.h> 

#include "nalu_tool.h"

extern Param *p_param;

//////////////////////////////////////////////////////////////////////////
// dump information of one NALU
//////////////////////////////////////////////////////////////////////////
int  DumpOneNalu(Nalu *p_nalu)
{
	printf("%4d: ",         p_nalu->index);
	printf("type = %2d, ",  p_nalu->type);
	printf("length= %5d.",   p_nalu->length);
	//printf("\n");
	return 0;
}

int  DumpOneSnalu(Snalu *p_snalu)
{
	DumpOneNalu(*(p_snalu->pnalu));
	printf("priority = %d, ",         p_snalu->priority_id);
	printf("dep_id = %2d, ",  p_snalu->dependency_id);
	printf("quality_id= %d, ",   p_snalu->quality_id);
    printf("temporal_id= %d.",   p_snalu->temporal_id);
	//printf("\n");
	return 0;
}

int ParseNaluHeader(Nalu *p_nalu)
{
		p_nalu->type = (p_nalu->buffer[4]) & 31;
		p_nalu->forbidden_zero_bit = ( (p_nalu->buffer[4]) & 128 ) >> 7;
		p_nalu->nal_ref_idc = ( (p_nalu->buffer[4]) & 96 ) >> 5;

		if( p_nalu->type == 14 || p_nalu->type == 20 )
		{
			Snalu *p_snalu;
			p_snalu = (Snalu*)calloc(1, sizeof(Snalu));
			if (NULL == p_snalu)
			{		
				printf("Mem allocation error in %s, %s\n", __FILE__, __LINE__);
				exit(1);
			}
			p_snalu->pnalu = &p_nalu;
			p_snalu->reserved_one_bit = ( (p_nalu->buffer[5]) & 128 ) >> 7;
			p_snalu->idr_flag = ( (p_nalu->buffer[5]) & 64 ) >> 6;
			p_snalu->priority_id = (p_nalu->buffer[5]) & 63;
			p_snalu->no_inter_layer_pred_flag = ( (p_nalu->buffer[6]) & 128 ) >> 7;
			p_snalu->dependency_id = ( (p_nalu->buffer[6]) & 112 ) >> 4;
			p_snalu->quality_id = (p_nalu->buffer[6]) & 15;
			p_snalu->temporal_id = ( (p_nalu->buffer[7]) & 224 ) >> 5;
			p_snalu->use_ref_base_pic_flag = ( (p_nalu->buffer[7]) & 16 ) >> 4;
			p_snalu->discardable_flag = ( (p_nalu->buffer[7]) & 8 ) >> 3;		
			p_snalu->output_flag = ( (p_nalu->buffer[7]) & 4 ) >> 2;
			p_snalu->reserved_three_2bits = (p_nalu->buffer[7]) & 3;

			if(p_param->dump_flag == 1)
				DumpOneSnalu(p_snalu);

			if (NULL != p_snalu)
			{
				free(p_snalu);
				p_snalu = NULL;
			}
		}


		else if(p_param->dump_flag == 1)
			DumpOneNalu(p_nalu);

		printf("\n");

		return 0;
}

