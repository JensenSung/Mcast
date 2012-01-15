/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/*!
 * @brief Functions for multicast group membership settings manipulation.
 * @file receiver-settings-dlg.c
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
#include "pcc.h"
#include "debug_helpers.h"
#include "resource.h"
#include "receiver-settings.h"
#include "receiver-settings-dlg.h"
#include "mcast-settings-dlg.h"

extern HINSTANCE g_hInst;

/*!
 * @brief A copy of the receiver settings object that this dialog operates on.
 * @details If the user blesses the dialog with an OK button, and all the data
 * validates OK, then this copy becomes the settings object returned to the caller.
 */
static struct receiver_settings g_settings;

/*!
 * @brief Handle to the poll sleep time delay edit control.
 */
static HWND g_poll_sleep_time_edit;

/*!
 * @brief Handle to the poll sleep time delay spin control.
 */
static HWND g_poll_sleep_time_spin;

/*!
 * @brief Handle to the play buffer size edit control.
 */
static HWND g_play_buffer_size_edit;

/*!
 * @brief Handle to the play buffer size spin control.
 */
static HWND g_play_buffer_size_spin;

/*!
 * @brief Handle to the Multimedit timer timeout edit control.
 */
static HWND g_mmtimer_edit_ctrl;

/*!
 * @brief Handle to the Multimedit timer timeout spin control.
 */
static HWND g_mmtimer_spin_ctrl;

/*!
 * @brief Handle to the Multimedit timer timeout spin control.
 */
static HWND g_btok;

/*!
 * @brief Maximum number of digits in the dialogs edit controls.
 */
#define TEXT_LIMIT (4)

/*!
 * @brief Buffer that holds a number typed into one of the edit controls.
 */
static TCHAR text_buffer[TEXT_LIMIT+1];

/*!
 * @brief Transfer from data to UI
 * @details Takes values from the settings object and presents them on the UI
 */
static void data_to_controls(struct receiver_settings const * p_settings)
{
	StringCchPrintf(text_buffer, TEXT_LIMIT+1, "%hu", p_settings->poll_sleep_time_);
	SetWindowText(g_poll_sleep_time_edit, text_buffer);
	StringCchPrintf(text_buffer, TEXT_LIMIT+1, "%hu", p_settings->play_buffer_size_);
	SetWindowText(g_play_buffer_size_edit, text_buffer);
	StringCchPrintf(text_buffer, TEXT_LIMIT+1, "%hu", p_settings->timer_delay_);
	SetWindowText(g_mmtimer_edit_ctrl, text_buffer);
    //debug_outputln("%s %d : %u", __FILE__, __LINE__, p_settings->timer_delay_);
}

/*!
 * @brief Transfer data from UI to the object.
 * @details Takes the values form the UI controls and saves them to the provided object.
 * @param[in] p_settings object to be written with UI data.
 * @return returns a non-zero value on success, 0 if failure has occured.
 */
static int controls_to_data(struct receiver_settings * p_settings)
{
    int result;
    unsigned int poll_sleep_time, play_buffer_size, timer_delay;
    memset(text_buffer, 0, sizeof(text_buffer));
    *((WORD *)text_buffer) = TEXT_LIMIT;
    SendMessage(g_poll_sleep_time_edit, EM_GETLINE, 0, (LPARAM)text_buffer); 
    result = sscanf(text_buffer, "%u", &poll_sleep_time);
    if (!result)
        goto error;
    if (poll_sleep_time > USHRT_MAX)
        goto error;
    memset(text_buffer, 0, sizeof(text_buffer));
    *((WORD *)text_buffer) = TEXT_LIMIT;
    SendMessage(g_play_buffer_size_edit, EM_GETLINE, 0, (LPARAM)text_buffer); 
    result = sscanf(text_buffer, "%u", &play_buffer_size);
    if (!result)
        goto error;
    if (play_buffer_size > USHRT_MAX)
        goto error;
    memset(text_buffer, 0, sizeof(text_buffer));
    *((WORD *)text_buffer) = TEXT_LIMIT;
    SendMessage(g_mmtimer_edit_ctrl, EM_GETLINE, 0, (LPARAM)text_buffer); 
    result = sscanf(text_buffer, "%u", &timer_delay);
    if (!result)
        goto error;
    if (timer_delay > USHRT_MAX)
        goto error;
    p_settings->timer_delay_ = timer_delay;
    p_settings->play_buffer_size_ = play_buffer_size;
    p_settings->poll_sleep_time_ = poll_sleep_time;
    //debug_outputln("%u : %hu ", __LINE__, p_settings->timer_delay_); 
    return 1;
error:
    return 0;
}

/*!
 * @brief Handler for WM_INITDIALOG message.
 * @details This handler does as follows:
 * \li Initializes the control handles
 * \li Presents the settings on the UI
 */
static BOOL Handle_wm_initdialog(HWND hwnd, HWND hWndFocus, LPARAM lParam)
{
	g_poll_sleep_time_edit = GetDlgItem(hwnd, IDC_POLL_SLEEP_TIME_EDIT);
	assert(g_poll_sleep_time_edit);
	g_poll_sleep_time_spin = GetDlgItem(hwnd, IDC_POLL_SLEEP_TIME_SPIN);
	assert(g_poll_sleep_time_spin);
	g_play_buffer_size_edit = GetDlgItem(hwnd, IDC_PLAY_BUFFER_SIZE_EDIT); 
	assert(g_play_buffer_size_edit);
	g_play_buffer_size_spin = GetDlgItem(hwnd, IDC_PLAY_BUFFER_SIZE_SPIN);
	assert(g_play_buffer_size_spin);
    g_mmtimer_edit_ctrl = GetDlgItem(hwnd, IDC_MMTIMER_EDIT_CTRL);
    assert(g_mmtimer_edit_ctrl);
    g_mmtimer_spin_ctrl = GetDlgItem(hwnd, IDC_MMTIMER_SPIN);
    assert(g_mmtimer_spin_ctrl);
    g_btok = GetDlgItem(hwnd, IDOK);
    assert(g_btok);

	SendMessage(g_poll_sleep_time_spin, UDM_SETBUDDY, (WPARAM)g_poll_sleep_time_edit, (LPARAM)0);
	SendMessage(g_play_buffer_size_spin, UDM_SETBUDDY, (WPARAM)g_play_buffer_size_edit, (LPARAM)0);
	SendMessage(g_mmtimer_spin_ctrl, UDM_SETBUDDY, (WPARAM)g_mmtimer_edit_ctrl, (LPARAM)0);
    SendMessage(g_mmtimer_edit_ctrl, EM_SETLIMITTEXT, (WPARAM)TEXT_LIMIT, (LPARAM)0);
    SendMessage(g_poll_sleep_time_edit, EM_SETLIMITTEXT, (WPARAM)TEXT_LIMIT, (LPARAM)0);
    SendMessage(g_play_buffer_size_edit, EM_SETLIMITTEXT, (WPARAM)TEXT_LIMIT, (LPARAM)0);
	data_to_controls(&g_settings);
	return TRUE;
} 

/**
 * @brief Multicast settings dialog message processing routine.
 * @details Processes the messages for the dialog, mainly the WM_COMMAND type.
 * @param hDlg
 * @param uMessage
 * @param wParam
 * @param lParam
 * @return  
 */
static INT_PTR CALLBACK McastSettingsProc(HWND hDlg, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    static struct receiver_settings copy_for_spins;
    static struct receiver_settings copy_for_edits;
    NMHDR * p_notify_header;
    NMUPDOWN * p_up_down;
    switch (uMessage)
    {
        case WM_INITDIALOG:
            return HANDLE_WM_INITDIALOG(hDlg, wParam, lParam, Handle_wm_initdialog);
        case WM_NOTIFY:
            p_notify_header = (NMHDR*)lParam;
            switch (p_notify_header->code)
            {
                case UDN_DELTAPOS:
                    p_up_down = (NMUPDOWN *)p_notify_header;
                    CopyMemory(&copy_for_spins, &g_settings, sizeof(struct receiver_settings));
                    switch (p_up_down->hdr.idFrom)
                    {
                        case IDC_POLL_SLEEP_TIME_SPIN:
                            copy_for_spins.poll_sleep_time_ -= p_up_down->iDelta;
                            if (receiver_validate_settings(&copy_for_spins))
                                data_to_controls(&copy_for_spins);
                            break;
                        case IDC_PLAY_BUFFER_SIZE_SPIN:
                            copy_for_spins.play_buffer_size_ -= p_up_down->iDelta;
                            if (receiver_validate_settings(&copy_for_spins))
                                data_to_controls(&copy_for_spins);
                            break;
                        case IDC_MMTIMER_SPIN:
                            copy_for_spins.timer_delay_ -= p_up_down->iDelta;
                            if (receiver_validate_settings(&copy_for_spins))
                                data_to_controls(&copy_for_spins);
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
            break;
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDC_POLL_SLEEP_TIME_EDIT:
                case IDC_PLAY_BUFFER_SIZE_EDIT:
                case IDC_MMTIMER_EDIT_CTRL:
                    if (EN_CHANGE == HIWORD(wParam))
                    {
                        CopyMemory(&copy_for_edits, &g_settings, sizeof(struct receiver_settings));
                        if (controls_to_data(&copy_for_edits) && receiver_validate_settings(&copy_for_edits))
                        {
                            CopyMemory(&g_settings, &copy_for_edits, sizeof(struct receiver_settings));
                            EnableWindow(g_btok, TRUE);
                        }
                        else
                        {
                            EnableWindow(g_btok, FALSE);
                        }
                    }
                    break;
                case IDC_MCAST_SETTINGS: 
                    get_settings_from_dialog(hDlg, &g_settings.mcast_settings_);
                    break;
                case IDCANCEL:
                case IDOK:
                    EndDialog(hDlg, wParam);
                    break;
            }
            break;
        default:
            break;
    }
    return FALSE;
}

int receiver_settings_do_dialog(HWND hWndParent, struct receiver_settings * p_settings)
{
	CopyMemory(&g_settings, p_settings, sizeof(struct receiver_settings));
	if (IDOK == DialogBox(g_hInst, MAKEINTRESOURCE(IDD_RECEIVER_SETTINGS), hWndParent, McastSettingsProc))
	{
		CopyMemory(p_settings, &g_settings, sizeof(struct receiver_settings));
		return 1;
	}
	return 0;
}

