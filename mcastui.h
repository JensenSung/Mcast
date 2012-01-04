#/* ex: set shiftwidth=4 tabstop=4 expandtab: */

/**
 * @file mcastui.h
 * @author T.Ostaszewski
 * @date 04-Jan-2012
 * @brief
 * @details
 */
ifndef WIN32

#define GET_WM_COMMAND_ID(wp, lp)               (wp)
#define GET_WM_COMMAND_HWND(wp, lp)             (HWND)(LOWORD(lp))
#define GET_WM_COMMAND_CMD(wp, lp)              HIWORD(lp)

#endif

#define ID_LISTVIEW  2000
