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

		i_info3 = FindStartCode(&p_nalu->buffer[i_pos-4], 3);
		if(i_info3 != 1)
			i_info2 = FindStartCode(&p_nalu->buffer[i_pos-3], 2);
		i_start_code_found = (i_info2 == 1 || i_info3 == 1);
	}
	fseek (pf_in, -4, SEEK_CUR);

	return i_pos - 4;
}

//////////////////////////////////////////////////////////////////////////
// Write one nalu frame buffer pbt_buf into file pf_out
//////////////////////////////////////////////////////////////////////////
int  PutOneNalu(FILE* pf_out, Nalu *p_nalu)
{
	size_t len;
	len = fwrite(p_nalu->buffer, sizeof(byte), p_nalu->length, pf_out);

	fflush(pf_out);
	// printf("write %d bytes\n", len);

	return (int)len;
}

//////////////////////////////////////////////////////////////////////////
// Init bitstream for current nalu
//////////////////////////////////////////////////////////////////////////
void InitBitstream(Bitstream *p_bs, Nalu *p_nalu)
{
	p_bs->buffer      = p_nalu->buffer + 4;
	p_bs->len         = p_nalu->length - 4;
	p_bs->byte_offset = 0;
	p_bs->bit_offset  = 0;
	p_bs->curr_byte   = p_bs->buffer[p_bs->byte_offset];
}

//////////////////////////////////////////////////////////////////////////
// Read n bits from the bitstream
//////////////////////////////////////////////////////////////////////////
int u_n(Bitstream *bs, int n)
{
	int ret = 0;
	int mask;
	int i;

	for (i = 0; i < n; i++)
	{
		mask = 0x01 << (7 - bs->bit_offset);
		ret <<= 1;
		ret += (bs->curr_byte & mask) >> (7 - bs->bit_offset);
		bs->bit_offset ++;

		if (bs->bit_offset == 8)
		{
			bs->byte_offset ++;
			bs->bit_offset = 0;
			bs->curr_byte  = bs->buffer[bs->byte_offset];
		}
	}

	return ret;
}