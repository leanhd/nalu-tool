#ifndef __NALU_TOOL_H__
#define __NALU_TOOL_H__

#define DEFAULT_NALU_NUM     1000    // large enough
#define DEFAULT_DUMP_FLAG    0
#define DEFAULT_IN_FILENAME  "in.264"
#define DEFAULT_OUT_FILENAME "out.264"

#define MAX_NALU_SIZE 1000000

typedef unsigned char byte;

typedef enum
{
	// JVT AD007
	UNSPECIFIED     = 0,
	NON_IDR_SLICE   = 1,
	SLICE_PARTA     = 2,
	SLICE_PARTB     = 2,
	SLICE_PARTC     = 4,
	IDR_SLICE       = 5,
	SEI             = 6,
	SPS             = 7,
	PPS             = 8,
	AU_DELIMITER    = 9,
	END_OF_SEQ      = 10,
	END_OF_STREAM   = 11,
	FILLER_DATA     = 12,
	SPS_EXTENTION   = 13,
	PREFIX_NALU     = 14,
	SUBSET_SPS      = 15,
	NON_PART_SLICE  = 19,
	SLICE_LAYER_EXT = 20
} NaluType;

typedef struct global_parameter 
{
	int nalu_num;       // how many nalus will be parsed
	int dump_flag;      // if nalu information will be dumped
} Param;

typedef struct nal_unit
{
	unsigned int index;

	int      forbidden_zero_bit;
	int      nal_ref_idc;
	NaluType type;

	int      length;
	byte    *buffer;
} Nalu;

typedef struct nal_unit_svc
{
	Nalu    **pnalu;
	int     reserved_one_bit;
	int     idr_flag;
	int     priority_id;
	int     no_inter_layer_pred_flag;
	int     dependency_id;
	int     quality_id;
	int     temporal_id;
	int     use_ref_base_pic_flag;
	int     discardable_flag;
	int     output_flag;
	int     reserved_three_2bits;
} Snalu;

int  ParseParam(FILE** f_in,FILE** f_out,int argc, char **argv);
int  GetOneNalu(FILE* pf_in, Nalu *p_nalu);
int  PutOneNalu(FILE* pf_out, Nalu *p_nalu);
int  DumpOneNalu(Nalu *p_nalu);
int  DumpOneSnalu(Snalu *p_snalu);
int  ParseNaluHeader(Nalu *p_nalu);

#endif // __NALU_TOOL_H__