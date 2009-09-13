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

	if (SVC_HEADER == header->subset)
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
int ParseNaluHeader(Nalu *p_nalu, Bitstream *p_bs, FILE *f_dump)
{
	NaluHeader *header = p_nalu->header;

	header->forbidden_zero_bit = u_n(p_bs, 1);
	header->nal_ref_idc        = u_n(p_bs, 2);
	header->type               = u_n(p_bs, 5);

	if( header->type == PREFIX_NALU || header->type == SLICE_LAYER_EXT )
	{
		header->svc_extension_flag    = u_n(p_bs, 1);
		header->subset = header->svc_extension_flag ? SVC_HEADER : MVC_HEADER;
	}
	else
		header->subset = AVC_HEADER;

	if (SVC_HEADER == header->subset)
	{
		header->idr_flag              = u_n(p_bs, 1);
		header->priority_id           = u_n(p_bs, 6);
		header->no_inter_layer_pred_flag = u_n(p_bs, 1);
		header->dependency_id         = u_n(p_bs, 3);
		header->quality_id            = u_n(p_bs, 4);
		header->temporal_id           = u_n(p_bs, 3);
		header->use_ref_base_pic_flag = u_n(p_bs, 1);
		header->discardable_flag      = u_n(p_bs, 1);		
		header->output_flag           = u_n(p_bs, 1);
		header->reserved_three_2bits  = u_n(p_bs, 2);
	}

// 	if (MVC_HEADER == header->subset)
// 	{
// 		;
// 	}

	if(p_param->dump_flag == 1)
		DumpOneNalu(p_nalu, f_dump);

	return 0;
}

