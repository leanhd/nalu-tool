#include <stdio.h>
#include <stdlib.h>

#include "nalu_tool.h"

//////////////////////////////////////////////////////////////////////////
// find start code (length = i_zeros)
//////////////////////////////////////////////////////////////////////////
static int FindStartCode(byte *pbt_buf, int i_zeros)
{   
	int info = 1;
	int i;

	for (i = 0; i < i_zeros; i++)
		if(pbt_buf[i] != 0)
			info = 0;

	if(pbt_buf[i] != 1)
		info = 0;
	return info;
}

//////////////////////////////////////////////////////////////////////////
// Read one nalu from file pf_in into buffer pbt_buf, 
// return length of nalu (in byte)
//////////////////////////////////////////////////////////////////////////
int GetOneNalu(FILE* pf_in, Nalu * p_nalu)
{
	int i_pos = 0;
	int i_start_code_found = 0;
	int i_info2 = 0;
	int i_info3 = 0;

	p_nalu->index++;

	while( !feof(pf_in) && (p_nalu->buffer[i_pos++]=fgetc(pf_in)) ==0)
		;

	while (!i_start_code_found)
	{
		if (feof (pf_in))
		{
			//			return -1;
			return i_pos-1;
		}
		p_nalu->buffer[i_pos++] = fgetc (pf_in);

		//initial the struct NALU
		if(p_nalu->buffer[i_pos-5]==0&&p_nalu->buffer[i_pos-4]==0
			&&p_nalu->buffer[i_pos-3]==0&&p_nalu->buffer[i_pos-2]==1)
		{
			p_nalu->type=(p_nalu->buffer[i_pos-1])&31;//00011111
			p_nalu->forbidden_zero_bit=(p_nalu->buffer[i_pos-1])&128;
			p_nalu->nal_ref_idc=(p_nalu->buffer[i_pos-1])&96;//01100000
		}

		i_info3 = FindStartCode(&p_nalu->buffer[i_pos-4], 3);
		if(i_info3 != 1)
			i_info2 = FindStartCode(&p_nalu->buffer[i_pos-3], 2);
		i_start_code_found = (i_info2 == 1 || i_info3 == 1);
	}
	fseek (pf_in, -4, SEEK_CUR);

	p_nalu->length = i_pos - 4;
	return i_pos - 4;
}

//////////////////////////////////////////////////////////////////////////
// Write one nalu frame buffer pbt_buf into file pf_out
//////////////////////////////////////////////////////////////////////////
int  PutOneNalu(FILE* pf_out, Nalu *p_nalu)
{
	fwrite(p_nalu->buffer, p_nalu->length, sizeof(byte), pf_out);
	
	fflush(pf_out);
	
	return 0;
}