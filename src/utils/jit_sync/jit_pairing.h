/****************************************************************************
 *   Aug 14 12:37:31 2020
 *   Copyright  2020  Dirk Brosswick
 *   Email: dirk.brosswick@googlemail.com
 ****************************************************************************/
 
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#ifndef _JIT_PAIRING_H
    #define _JIT_PAIRING_H

    #include <TTGO.h>
    #include "hardware/callback.h"

    #define   LOGIN_CLEAN           0
    #define   LOGIN_SCREEN          1
    #define   LOGIN_IN_PROCESS      2
    #define   CHECK_FOR_LOGOUT      3


// ---------EVENT GROUP----------- 

#define LOGIN_DONE                                            _BV(0)         /** @brief event mask for LOGIN_REALIZADO */
#define LOGOUT_REQUEST                                        _BV(1)         /** @brief event mask for LOGOUT_REQUEST */
#define LOGOUT_DONE                                           _BV(2)         /** @brief event mask for mqtt LOGOUT_REALIZADO */


    void jit_pairing_tile_setup( void );

    bool loginctrl_register_cb( EventBits_t event, CALLBACK_FUNC callback_func, const char *id );

#endif // _JIT_PAIRING_H


