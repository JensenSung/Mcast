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

static struct receiver_settings * g_settings;

static HWND g_poll_sleep_time_edit;
static HWND g_poll_sleep_time_spin;
static HWND g_play_buffer_size_edit;
static HWND g_play_buffer_size_spin;

static void data_to_controls(struct receiver_settings const * p_settings)
{
	char text_buf[8];
	StringCchPrintf(text_buf, 8, "%d", p_settings->poll_sleep_time_);
	SetWindowText(g_poll_sleep_time_edit, text_buf);
	StringCchPrintf(text_buf, 8, "%d", p_settings->play_buffer_size_);
	SetWindowText(g_play_buffer_size_edit, text_buf);
}

static BOOL Handle_wm_initdialog(HWND hwnd, HWND hWndFocus, LPARAM lParam)
{
	g_settings = (struct receiver_settings*)lParam;
	g_poll_sleep_time_edit = GetDlgItem(hwnd, IDC_POLL_SLEEP_TIME_EDIT);
	assert(g_poll_sleep_time_edit);
	g_poll_sleep_time_spin = GetDlgItem(hwnd, IDC_POLL_SLEEP_TIME_SPIN);
	assert(g_poll_sleep_time_spin);
	g_play_buffer_size_edit = GetDlgItem(hwnd, IDC_PLAY_BUFFER_SIZE_EDIT); 
	assert(g_play_buffer_size_edit);
	g_play_buffer_size_spin = GetDlgItem(hwnd, IDC_PLAY_BUFFER_SIZE_SPIN);
	assert(g_play_buffer_size_spin);


	SendMessage(g_poll_sleep_time_spin, UDM_SETBUDDY, (WPARAM)g_poll_sleep_time_edit, (LPARAM)0);
	SendMessage(g_play_buffer_size_spin, UDM_SETBUDDY, (WPARAM)g_play_buffer_size_edit, (LPARAM)0);
	data_to_controls(g_settings);
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
	struct receiver_settings rcv_settings_copy;
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
					switch (p_up_down->hdr.idFrom)
					{
						case IDC_POLL_SLEEP_TIME_SPIN:
							{
								memcpy(&rcv_settings_copy, g_settings, sizeof(struct receiver_settings));
								rcv_settings_copy.poll_sleep_time_ -= p_up_down->iDelta;
								debug_outputln("%s %d : %d", __FILE__, __LINE__, p_up_down->iPos);
								if (receiver_validate_settings(&rcv_settings_copy))
									memcpy(g_settings, &rcv_settings_copy, sizeof(struct receiver_settings));
							}
							break;
						case IDC_PLAY_BUFFER_SIZE_SPIN:
							{
								memcpy(&rcv_settings_copy, g_settings, sizeof(struct receiver_settings));
								rcv_settings_copy.play_buffer_size_ -= p_up_down->iDelta;
								if (receiver_validate_settings(&rcv_settings_copy))
									memcpy(g_settings, &rcv_settings_copy, sizeof(struct receiver_settings));
								debug_outputln("%s %d : %d", __FILE__, __LINE__, p_up_down->iPos);
							}
							break;
						default:
							break;
					}
					data_to_controls(g_settings);
					break;
				default:
					break;
			}
			break;
		case WM_COMMAND:
			switch(wParam)
			{
				case IDC_MCAST_SETTINGS: 
					get_settings_from_dialog(hDlg, &g_settings->mcast_settings_);
					break;
				case IDCANCEL:
				case IDOK:
					EndDialog(hDlg, wParam);
					break;
			}
			return TRUE;
	}
	return FALSE;
}

int receiver_settings_do_dialog(HWND hWndParent, struct receiver_settings * p_settings)
{
	struct receiver_settings rcv_settings_copy;
	memcpy(&rcv_settings_copy, p_settings, sizeof(struct receiver_settings));
	if (IDOK == DialogBoxParam(g_hInst, MAKEINTRESOURCE(IDD_RECEIVER_SETTINGS), hWndParent, McastSettingsProc, (LPARAM)&rcv_settings_copy))
	{
		memcpy(p_settings, &rcv_settings_copy, sizeof(struct receiver_settings));
		return 0;
	}
	return 1;
}

