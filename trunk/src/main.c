#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nalu_tool.h"

Param param, *p_param = &param;

__inline void PrintHelp(void)
{
	printf("nalu_tool.exe -i in_file [options] -o [out_file]\n");
	printf("options: \n");
	printf("\t-n N : fetch N NALUs from in_file to out_file\n");
	printf("\t-v   : dump NALU details\n");
}

void InitDump(FILE** f_dump)
{
	printf("----------------------------- NALU Tool %s ----------------------------\n", VERSION);
	printf("Input file                       : %s \n", p_param->in_filename);
	printf("Output file                      : %s \n", p_param->out_filename);
	printf("NALU numbers                     : %d \n", p_param->nalu_num);
	printf("Dump NALU headers                : %s \n", p_param->dump_flag ? "Yes" : "No");
	if (p_param->dump_flag == 1)
	printf("Dump file                        : %s \n", p_param->dump_filename);
	printf("---------------------------------------------------------------------------\n");

	if (p_param->dump_flag == 1)
	{
		*f_dump = fopen(p_param->dump_filename, "w");
		fprintf(*f_dump, "NALU type length nal_ref_idc idr Pid no_inter_layer_pred DTQ use_ref_base_pic discardable output\n");
		fprintf(*f_dump, "------------------------------------------------------------------------------------------------\n");
	}
}

void FinishDump(FILE** f_dump)
{
	if (p_param->dump_flag == 1)
	{
		fprintf(*f_dump, "------------------------------------------------------------------------------------------------\n");
		fclose(*f_dump);

		printf("\n");
		printf("finished!\n");
	}
}

//////////////////////////////////////////////////////////////////////////
// init nalu
//////////////////////////////////////////////////////////////////////////
int InitNalu(Nalu **nalu)
{
	*nalu = (Nalu*)calloc(1, sizeof(Nalu));
	if (NULL == (*nalu))
	{		
		printf("Mem allocation error in %s, %s\n", __FILE__, __LINE__);
		exit(1);
	}

	(*nalu)->buffer = (byte*)calloc(MAX_NALU_SIZE, sizeof(byte));
	if (NULL == ((*nalu)->buffer))
	{		
		printf("Mem allocation error in %s, %s\n", __FILE__, __LINE__);
		exit(1);
	}

	(*nalu)->header = (NaluHeader*)calloc(1, sizeof(NaluHeader));
	if (NULL == ((*nalu)->header))
	{		
		printf("Mem allocation error in %s, %s\n", __FILE__, __LINE__);
		exit(1);
	}

	(*nalu)->index = -1;

	return 1;
}

//////////////////////////////////////////////////////////////////////////
// free nalu
//////////////////////////////////////////////////////////////////////////
int FreeNalu(Nalu **nalu)
{

	if (NULL != ((*nalu)->header))
	{
		free((*nalu)->header);
		(*nalu)->header = NULL;
	}

	if (NULL != ((*nalu)->buffer))
	{
		free((*nalu)->buffer);
		(*nalu)->buffer = NULL;
	}

	if (NULL != (*nalu))
	{
		free(*nalu);
		*nalu = NULL;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// main function
//////////////////////////////////////////////////////////////////////////
int main (int argc, char **argv)
{
	FILE *pf_in      = NULL;
	FILE *pf_out     = NULL;
	FILE *pf_dump    = NULL;
	Nalu *p_nalu     = NULL;

	Bitstream bs, *p_bs = &bs;
	
	InitNalu(&p_nalu);

	if (0 == ParseParam(&pf_in,&pf_out,argc, argv))
		exit(1);

	InitDump(&pf_dump);

	while (p_param->nalu_num-- && !feof(pf_in))
	{
		p_nalu->length = GetOneNalu(pf_in,  p_nalu);

		InitBitstream(p_bs, p_nalu);

		ParseNaluHeader(p_nalu, p_bs, pf_dump);

		PutOneNalu(pf_out, p_nalu);
	}

	FinishDump(&pf_dump);

	FreeNalu(&p_nalu);

	if (pf_in != NULL)
		fclose(pf_in);
	if (pf_out != NULL)
		fclose(pf_out);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// Set the parameters.
//////////////////////////////////////////////////////////////////////////
int  ParseParam(FILE** f_in,FILE** f_out,int argc, char **argv)
{
	// set default parameters
	// printf("Setting Default Parameters...\n");
	int i;
	p_param->in_filename   = DEFAULT_IN_FILENAME;
	p_param->out_filename  = DEFAULT_OUT_FILENAME;
	p_param->dump_filename = DEFAULT_DUMP_FILENAME;
	p_param->nalu_num      = DEFAULT_NALU_NUM;
	p_param->dump_flag     = DEFAULT_DUMP_FLAG;
	
	// parse command line parameters
	// printf("Parsing Command Line Parameters...\n");
	if (argc < 3 || argc > 8)
	{
		PrintHelp();
		return 0;
	}

	for(i=1;i<argc;i++)
	{
		if(0 == strncmp (argv[i], "-i", 2))
		{
			if((*f_in = fopen(argv[i+1], "rb")) == NULL)
			{
				printf( "Error: file %s not found\n",argv[i+1]);
			}
			p_param->in_filename = argv[i+1];
			i++;
		}

		if(0 == strncmp (argv[i], "-o", 2))
		{
			p_param->out_filename = argv[i+1];
			i++;
		}

		if(0 == strncmp (argv[i], "-n", 2))
		{
			p_param->nalu_num = atoi(argv[i+1]);
			i++;
		}

		if(0 == strncmp (argv[i], "-v", 2))
		{
			p_param->dump_flag = 1;
		}
	}

	if((*f_out = fopen(p_param->out_filename, "wb")) == NULL)
		printf( "Error: file can not open\n");

	if(i=argc)
		return 1;

	PrintHelp();
	return 0;
}