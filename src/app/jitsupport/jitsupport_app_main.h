
#include "AsyncJson.h"
#include "ArduinoJson.h"

#ifndef _JITSUPPORT_APP_MAIN_H
    #define _JITSUPPORT_APP_MAIN_H

    #include <TTGO.h>
    #include "hardware/callback.h"

    #define MAX_NUMBER_TICKETS              30
    #define EMPTY                           1
    #define FULL                            0
    
    //#define NO_HTTP_RESPONSE

    #define VIBRATION_DISABLE               0
    #define VIBRATION_SMOOTH                20
    #define VIBRATION_INTENSE               70



    void jitsupport_app_main_setup( uint32_t tile_num );
    void newticket( JsonObject jsonObj);
    void MQTT_callback(char* topic, byte* message, unsigned int length);
    void MQTT2_publish(char *atualizartopico, char *payload);
    void cmd_reset();












    


#endif // _JITSUPPORT_APP_MAIN_H