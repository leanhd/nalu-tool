#include <stdio.h>
#include<stdlib.h> 

#include "nalu_tool.h"

extern Param *p_param;

//////////////////////////////////////////////////////////////////////////
// dump information of one NALU
//////////////////////////////////////////////////////////////////////////
int DumpOneNalu(Nalu *p_nalu, FILE *f_dump)
{
	NaluHeader *header = p_nalu->header;

	// printf("%d..", p_nalu->index);

	fprintf(f_dump, "%4d ",    p_nalu->index);
	fprintf(f_dump, "%4d ",    header->type);
	fprintf(f_dump, "%6d ",    p_nalu->length);
	fprintf(f_dump, "%11d ",   header->nal_ref_idc);

	if (header->is_svc_header)
	{
		fprintf(f_dump, "%3d ",    header->idr_flag);
		fprintf(f_dump, "%3d ",    header->priority_id);
		fprintf(f_dump, "%19d ",   header->no_inter_layer_pred_flag);
		fprintf(f_dump, "%d%d%d ", header->dependency_id, header->quality_id, header->temporal_id);
		fprintf(f_dump, "%16d ",   header->use_ref_base_pic_flag);
		fprintf(f_dump, "%11d ",   header->discardable_flag);
		fprintf(f_dump, "%6d",     header->output_flag);
	}

	fprintf(f_dump,"\n");
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// read nalu header
//////////////////////////////////////////////////////////////////////////
int ParseNaluHeader(Nalu *p_nalu, FILE *f_dump)
{
	NaluHeader *header = p_nalu->header;

	header->type               = (p_nalu->buffer[4]) & 31;
	header->forbidden_zero_bit = ( (p_nalu->buffer[4]) & 128 ) >> 7;
	header->nal_ref_idc        = ( (p_nalu->buffer[4]) & 96 ) >> 5;

	if( header->type == PREFIX_NALU || header->type == SLICE_LAYER_EXT )
		header->is_svc_header = 1;
	else
		header->is_svc_header = 0;

	if (header->is_svc_header)
	{
		header->reserved_one_bit      = ( (p_nalu->buffer[5]) & 128 ) >> 7;
		header->idr_flag              = ( (p_nalu->buffer[5]) & 64 ) >> 6;
		header->priority_id           = (p_nalu->buffer[5]) & 63;
		header->no_inter_layer_pred_flag = ( (p_nalu->buffer[6]) & 128 ) >> 7;
		header->dependency_id         = ( (p_nalu->buffer[6]) & 112 ) >> 4;
		header->quality_id            = (p_nalu->buffer[6]) & 15;
		header->temporal_id           = ( (p_nalu->buffer[7]) & 224 ) >> 5;
		header->use_ref_base_pic_flag = ( (p_nalu->buffer[7]) & 16 ) >> 4;
		header->discardable_flag      = ( (p_nalu->buffer[7]) & 8 ) >> 3;		
		header->output_flag           = ( (p_nalu->buffer[7]) & 4 ) >> 2;
		header->reserved_three_2bits  = (p_nalu->buffer[7]) & 3;
	}

	if(p_param->dump_flag == 1)
		DumpOneNalu(p_nalu, f_dump);

	return 0;
}

