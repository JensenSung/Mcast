/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/*!
 * @file abstract-tone.h
 * @brief The abstraction of the tone to be played. The interface.
 * @author T.Ostaszewski
 * @date Jan-2012
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
#if !defined ABSTRACT_TONE_BB10D7AB_D9C8_4A96_926F_15CA2C7F27F0
#define ABSTRACT_TONE_BB10D7AB_D9C8_4A96_926F_15CA2C7F27F0

#if defined __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <tchar.h>

struct abstract_tone;
struct PCMWAVEFORMAT; /* defined int <mmsystem.h> and <mmreg.h> from WINDOWS SDK */

/*! 
 * @brief Type of the tone to be created.
 */
typedef enum eToneType {
    EMBEDDED_TEST_TONE = 0, /*!< Indicates that the tone embedded in the resource shall be created. */
    EXTERNAL_WAV_TONE = 1,  /*!< Indicates that the custom WAV tone shall be created.*/
} tone_type_t;

/*!
 * @brief Tones factory.
 * @details Creates a tone.
 * @param[in] eType Type of the tone to be created.
 * @param[in] psz_tone_name Tone specific creation data.
 * @return returns a handle to the abstract tone.
 * @sa abstract_tone_destroy
 */
struct abstract_tone * abstract_tone_create(tone_type_t eType, LPCTSTR psz_tone_name);

/*!
 * @brief Destroys a tone.
 * @details Performs tone specific destruction. For a tone embedded in a WAV file, no special action is needed.
 * For a tone which is read from external file, they may be some actions needed (closing underlying file and its
 * memory mappings, if such exist).
 * @param[in] p_tone handle to the tone 
 * @sa abstract_tone_create
 */
void abstract_tone_destroy(struct abstract_tone * p_tone);

#if defined DEBUG
/*!
 * @brief Returns tone type.
 * @details Although such a method is usually bashed by OO evangelists, I decided to use it.
 * Having a single method that returns a type avoids cluttering this abstract object aobut UI details.
 * It is the UI widgets that are to be changed/updated to reflect tone type.
 * @param[in] p_tone handle to the tone 
 * @return returns the type of the tone.
 * @sa abstract_tone_create
 */
tone_type_t abstract_tone_get_type(struct abstract_tone * p_tone);
#endif

/*!
 * @brief Returns the PCMWAVEFORMAT structure for the tone.
 * @param[in] p_tone tone for which PCMWAVEFORMAT structure is to be retrieved.
 * @return retuns
 */
PCMWAVEFORMAT const * abstract_tone_get_pcmwaveformat(struct abstract_tone const * p_tone);

/*!
 * @brief Returns the pointer to tone data.
 * @details Const-correct variant of the abstract_tone_get_wave_data routine.
 * @param[in] p_tone handle of the tone, for which data is to be returned.
 * @param[in,out] p_data_size Pointer to the caller allocated memory, which will be written with size of the array returned as function value.
 * @return Pointer to the first byte of the tone data.
 * @sa abstract_tone_get_wave_data
 */
const void * abstract_tone_get_wave_data(struct abstract_tone const * p_tone, size_t * p_data_size);

/*!
 * @brief Dumps the tone details into caller's provided buffer.
 * @details <b>Fill me</b>
 * @param[in] p_tone handle of the tone, for which data is to be returned.
 * @param[in] pszBuffer caller's allocated buffer into which tone details will be dump.
 * @param[in] size size of the buffer, measured in number of characters, that the caller's buffer can hold, including terminating NULL. 
 * @return returns number of characters written into the buffer.
 */
size_t abstract_tone_dump(struct abstract_tone const * p_tone, LPTSTR pszBuffer, size_t size);

#if defined __cplusplus
}
#endif

#endif /* ABSTRACT_TONE_BB10D7AB_D9C8_4A96_926F_15CA2C7F27F0 */

