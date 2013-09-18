/* ex: set shiftwidth=4 tabstop=4 expandtab: */

/**
 * @file wave_utils.c
 * @brief Implementation of WAV file utilities.
 * @details
 * @author T.Ostaszewski
 * @date 04-Jan-2012
 * @par License
 * @code Copyright 2012 Tomasz Ostaszewski. All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 * 	1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *	2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation 
 * 	and/or other materials provided with the distribution.
  * THIS SOFTWARE IS PROVIDED BY Tomasz Ostaszewski AS IS AND ANY 
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL Tomasz Ostaszewski OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE.
  * The views and conclusions contained in the software and documentation are those of the 
 * authors and should not be interpreted as representing official policies, 
 * either expressed or implied, of Tomasz Ostaszewski.
 * @endcode 
 */

#include "pcc.h"
#include "wave_utils.h"
#include "debug_helpers.h"

/*! 
 * @brief Macro to facilitate structure creation.
 */
#define MAKE_STRUCT(x)	{x, #x}

static const char * wFormatTag2String(WORD wFormatTag)
{
	static const struct tag_wFormatTag2String {
		WORD wFormatTag_;
		const char * string_;
	} format2String_[] = {
		 MAKE_STRUCT(WAVE_FORMAT_PCM),
		 MAKE_STRUCT(WAVE_FORMAT_IEEE_FLOAT),
		 MAKE_STRUCT(WAVE_FORMAT_EXTENSIBLE),
		 MAKE_STRUCT(WAVE_FORMAT_ADPCM),
	};
	size_t idx = 0;
	for (idx = 0; idx < sizeof(format2String_)/sizeof(format2String_[0]); ++idx)
	{
		if (format2String_[idx].wFormatTag_ == wFormatTag)
			return format2String_[idx].string_;
	}
	return "NULL";
}

int dump_pcmwaveformat(char * psz_buffer, size_t buffer_size, pcmwaveformat_t const * p_wfe)
{
    HRESULT hr;
    hr = StringCchPrintf(psz_buffer, buffer_size, 
            "%-30s : %-10s\n"
            "%-30s : %u\n"
            "%-30s : %hu\n"
            "%-30s : %hu\n"
            "%-30s : %u\n"
            "%-30s : %hu\n",
            "Type", wFormatTag2String(p_wfe->wf.wFormatTag),
            "Sample rate [Hz]", p_wfe->wf.nSamplesPerSec, 
            "Bits per sample", p_wfe->wBitsPerSample,
            "Channels", p_wfe->wf.nChannels, 
            "Avg rate [bytes/sec]", p_wfe->wf.nAvgBytesPerSec, 
            "Block align", p_wfe->wf.nBlockAlign 
            );
    return SUCCEEDED(hr);
}

void copy_pcmwaveformat_2_WAVEFORMATEX(WAVEFORMATEX * p_dest, pcmwaveformat_t const * p_source)
{
	p_dest->wFormatTag 		= p_source->wf.wFormatTag;
	p_dest->nChannels 		= p_source->wf.nChannels;
	p_dest->nSamplesPerSec 	= p_source->wf.nSamplesPerSec;
	p_dest->nAvgBytesPerSec = p_source->wf.nAvgBytesPerSec;
	p_dest->nBlockAlign 	= p_source->wf.nBlockAlign;
	p_dest->wBitsPerSample	= p_source->wBitsPerSample;
	p_dest->cbSize 			= sizeof(WAVEFORMATEX);
}

void waveformat_normalize(WAVEFORMATEX * p_dest)
{
	p_dest->nAvgBytesPerSec = p_dest->nSamplesPerSec * (p_dest->wBitsPerSample/8) * p_dest->nChannels ;
}

int LoadWavFromResoure(P_MASTER_RIFF_CONST * pp_chunk, HINSTANCE hModule, LPCTSTR lpResName)
{
    HRSRC hRes;
    int result = 0;
    if (NULL == hModule)
        hModule = GetModuleHandle(NULL);
    hRes = FindResource(hModule, lpResName, "0");
    if (NULL != hRes)
    {
        HGLOBAL globRes;
        globRes = LoadResource(hModule, hRes);
        if (NULL != hRes)
        {
            *pp_chunk = (P_MASTER_RIFF_CONST)LockResource(globRes);
            if (NULL != *pp_chunk)
            {
                result = 1;
            }
        }
        else
        {
            debug_outputln("%s %5.5d : %10.10d %8.8x", __FILE__, __LINE__, GetLastError(), GetLastError());
        }
    }
    else
    {
        debug_outputln("%s %5.5d : %10.10d %8.8x", __FILE__, __LINE__, GetLastError(), GetLastError());
    }
    return result;
}

int16_t const * get_wave_data(P_MASTER_RIFF_CONST p_master_riff)
{
    switch (p_master_riff->format_chunk_2_.wFormatTag_)
    {
        case WAVE_FORMAT_PCM:
            return &p_master_riff->format_chunk_2_.plain_wav_.subchunk_.samples16_[0];
            break;
        default:
            return NULL;
    }
}

uint32_t get_wave_data_size(P_MASTER_RIFF_CONST p_master_riff)
{
    switch (p_master_riff->format_chunk_2_.wFormatTag_)
    {
        case WAVE_FORMAT_PCM:
            return p_master_riff->format_chunk_2_.plain_wav_.subchunk_.subchunk_size_;
            break;
        default:
            return 0;
    }
}

void get_waveformat(P_MASTER_RIFF_CONST p_master_riff, WAVEFORMAT * p_output)
{
    
}

