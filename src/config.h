/****************************************************************************
              config.h

    Tu May 22 21:23:51 2020
    Copyright  2020  Dirk Brosswick
 *  Email: dirk.brosswick@googlemail.com
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
#ifndef _CONFIG_H

    #if defined( LILYGO_WATCH_2020_V1 )
        #define WATCH_VERSION_NAME  "V1"
    #elif defined( LILYGO_WATCH_2020_V2 )
        #define WATCH_VERSION_NAME  "V2"
    #elif defined( LILYGO_WATCH_2020_V3 )
        #define WATCH_VERSION_NAME  "V3"
    #else
        #error "no ttgo t-watch 2020 version defined"
    #endif


    #define LILYGO_WATCH_LVGL                       /** @brief To use LVGL, you need to enable the macro LVGL */
    #define TWATCH_USE_PSRAM_ALLOC_LVGL             /** @brief enabled lillygo-lib to use PSRAM */ 
    /**
     * Built-in applications
     */
    #define ENABLE_WEBSERVER                        /** @brief To disable built-in webserver, comment this line */
    #define ENABLE_FTPSERVER                        /** @brief To disable built-in ftpserver, comment this line */
    /**
     * Enable non-latin languages support
     */
    #define USE_EXTENDED_CHARSET CHARSET_CYRILLIC
    
    /**
     * firmeware version string
     */

    #define __FIRMWARE__            "2021050001"


    //-----------Global---------

    extern volatile int wifi_connected;
    extern int boot_finish;

    //------MQTT---CONNECTION------
    //#define MQTT_BROKER_MOSQUITTO                        
    //#define MQTT_BROKER_CLOUD    
    #define MQTT_BROKER_JABIL

    #ifdef  MQTT_BROKER_CLOUD                                

    #define MQTT_SERVER                                 "m16.cloudmqtt.com"
    #define MQTT_USER                                   "pbgjzbad"
    #define MQTT_PSSWD                                  "KhwBxhvkWZFq"
    #define MQTT_PORT                                   10304
    #define MQTT_KEEPALIVE_SECONDS                      120
    #define MQTT_CLEAN_SESSION                          0  

    #endif

    #ifdef  MQTT_BROKER_MOSQUITTO                                

    #define MQTT_SERVER                                 "test.mosquitto.org"
    #define MQTT_USER                                   NULL
    #define MQTT_PSSWD                                  NULL
    #define MQTT_PORT                                   1883
    #define MQTT_KEEPALIVE_SECONDS                      120
    #define MQTT_CLEAN_SESSION                          0    
    #endif

    #ifdef  MQTT_BROKER_JABIL                                

    #define MQTT_SERVER                                 "brbelm0mat81.corp.jabil.org"
    #define MQTT_USER                                   NULL
    #define MQTT_PSSWD                                  NULL
    #define MQTT_PORT                                   1883
    #define MQTT_KEEPALIVE_SECONDS                      120
    #define MQTT_CLEAN_SESSION                          0    
    #endif

    #define CHECK_MQTT_CONNECTION_MILLI_SECONDS          5000
    #define WIFI_TENTATIVES_TO_RECONNECT                 20                         //before go to sleep 

    //-----WIFI--AUTHENTICATION---
    //#define WIFI_SSID         "2.4 CLARO VIRTUA 15 CS 2"
    //#define WIFI_PASSWORD     "3617970200"

    //#define WIFI_SSID         "TooPrede"                     /** @brief define SSID DA REDE */
    //#define WIFI_PASSWORD     "12345678"                     /** @brief Password  */

    #define WIFI_SSID         "JAB_RASP0001"
    #define WIFI_PASSWORD     "g4keKDI2RkXQT"

    //#define WIFI_SSID            "2.4_netvirtua283bl2ap1002"
    //#define WIFI_PASSWORD        "3348981600"


   //--------------JABIL API-----------------

#define BRBELM0APPS99

#ifdef BRBELM0APPS99

    #define FIRMWARE_UPDATE_URL            "http://172.24.72.137:3089/params"
    #define LOGIN_API_URL                  "http://172.24.72.137:3089/login" 

#endif

#ifdef BRBELIE031

    #define FIRMWARE_UPDATE_URL            "http://172.24.73.44:3089/params"
    #define LOGIN_API_URL                  "http://172.24.73.44:3089/login" 

#endif

    #define JITAPI_GET_BY_TEAM_URL          "http://172.24.72.137/JITAPI/Smartwatch/GetByTeam/"
    #define JITAPI_GET_WATCH_BY_ID_URL      "http://172.24.72.137/JITAPI/Smartwatch/GetByIP/"
    #define JITAPI_GET_WATCH_BY_USERID_URL  "http://172.24.72.137/JITAPI/Smartwatch/GetByUserId/"

//------------OPTIONS----------------

    #define AUTO_UPDATE_AND_RESTART                    
    #define JABIL_SILENCE_WAKEUP_INTERVAL                       5

    #ifdef __cplusplus
        #include <LilyGoWatch.h>
        #define _CONFIG_H 
    #endif

#endif // _CONFIG_H
