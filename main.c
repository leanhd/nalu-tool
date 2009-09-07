#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nalu_tool.h"

int  g_i_num;  // how many nalus will be cut out
int DUMPNALU=0;

__inline void PrintHelp()
{
	printf("nalu_tool.exe -i in_file [options] -o [out_file]\n");
	printf("options: \n");
	printf("\t-n N : fetch N NALUs from in_file to out_file\n");
	printf("\t if need NALU details,usage:add -v \n");
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

	return 1;
}

//////////////////////////////////////////////////////////////////////////
// free nalu
//////////////////////////////////////////////////////////////////////////
int FreeNalu(Nalu **nalu)
{
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
	Nalu *p_nalu     = NULL;

	InitNalu(&p_nalu);

	if (0 == ParseParam(&pf_in,&pf_out,argc, argv))
		exit(1);

	while (g_i_num-- && !feof(pf_in))
	{
		GetOneNalu(pf_in,  p_nalu);
	    NALU_index++;

		if(DUMPNALU==1)
			DumpOneNalu(p_nalu);

		PutOneNalu(pf_out, p_nalu);
	}

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
	int err;
	char *in_filename  = DEFAULT_IN_FILENAME;
	char *out_filename = DEFAULT_IN_FILENAME;
	g_i_num            = DEFAULT_NALU_NUM;
	

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
			if((err=fopen_s(f_in,argv[i+1],"rb"))!=0)
			{
				printf( "Error: file %s not found\n",argv[i+1]);
			}
			i++;
		}

		if(0 == strncmp (argv[i], "-o", 2))
		{
			out_filename=argv[i+1];
			i++;
		}

		if(0 == strncmp (argv[i], "-n", 2))
		{
			g_i_num = atoi(argv[i+1]);
			i++;
		}

		if(0 == strncmp (argv[i], "-v", 2))
		{
			DUMPNALU=1;
		}
	}

	if((err=fopen_s(f_out,out_filename,"wb"))!=0)
		printf( "Error: file can not open\n");

	if(i=argc)
		return 1;

	PrintHelp();
	return 0;
}