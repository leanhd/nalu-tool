#ifndef __NALU_TOOL_H__
#define __NALU_TOOL_H__

#define DEFAULT_NALU_NUM     3
#define DEFAULT_IN_FILENAME  "in.264"
#define DEFAULT_OUT_FILENAME "out.264"

#define MAX_NALU_SIZE 1000000

int  NALU_index;

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
	NON_PART_SLICE  = 19
} NaluType;

typedef struct nal_unit
{
	int      forbidden_zero_bit;
	int      nal_ref_idc;
	NaluType type;

	int      length;
	byte    *buffer;
} Nalu;

int  ParseParam(FILE** f_in,FILE** f_out,int argc, char **argv);
int  GetOneNalu(FILE* pf_in, Nalu *p_nalu);
int  PutOneNalu(FILE* pf_out, Nalu *p_nalu);

int  DumpOneNalu(Nalu *p_nalu);

#endif // __NALU_TOOL_H__