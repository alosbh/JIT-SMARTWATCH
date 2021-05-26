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
 * 
 */
#include "config.h"
#include "jit_pairing.h"

#include "gui/mainbar/mainbar.h"
// #include "gui/mainbar/setup_tile/setup_tile.h"
#include "gui/statusbar.h"
#include "gui/widget_factory.h"
#include "gui/widget_styles.h"
#include "hardware/blectl.h"
#include "hardware/pmu.h"
#include "hardware/powermgm.h"
#include "hardware/motor.h" 
#include "gui/keyboard.h"
#include "hardware/wifictl.h" 
#include "hardware/callback.h"

#include "hardware/powermgm.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include "ArduinoJson.h"

lv_obj_t *jit_pairing_tile=NULL;
lv_style_t jit_pairing_style;
lv_obj_t *login_btn = NULL;

TaskHandle_t _login_task =NULL;


lv_obj_t *jit_pairing_img = NULL;
lv_obj_t *jit_pairing_info_label = NULL;
lv_obj_t *jit_pairing_status_label=NULL;

lv_obj_t *jit_pairing_code_textfield = NULL;

LV_IMG_DECLARE(cancel_32px);
LV_IMG_DECLARE(update_64px);
LV_FONT_DECLARE(Ubuntu_16px);

static void jabil_pairing_num_textarea_event_cb( lv_obj_t * obj, lv_event_t event );
static void exit_jit_pairing_event_cb( lv_obj_t * obj, lv_event_t event );
bool jit_pairing_powermgm_loop_cb( EventBits_t event, void *arg ); 
static void login_btn_event_cb(lv_obj_t * obj, lv_event_t event);
bool jit_pairing_login_screen();

bool start_login_flag=true;
uint8_t login_state=LOGIN_SCREEN;
uint32_t jit_pairing_tile_num;
char token[10];
int UserID;

void login_task(void * pvParameters);
bool check_token();

EventGroupHandle_t xLoginCtrlEvent=NULL;
callback_t *loginctrl_callback = NULL;

//--------- Prototipos------------

void loginctrl_set_event( EventBits_t bits );
void loginctrl_clear_event( EventBits_t bits);
bool loginctrl_register_cb( EventBits_t event, CALLBACK_FUNC callback_func, const char *id ); 
bool loginctrl_send_event_cb( EventBits_t event, void *arg );



void jit_pairing_tile_setup( void ) {
    // get an app tile and copy mainstyle
    jit_pairing_tile_num = mainbar_add_app_tile( 1, 1, "jit pairing" );
    jit_pairing_tile = mainbar_get_tile_obj( jit_pairing_tile_num );

    lv_style_copy( &jit_pairing_style, ws_get_setup_tile_style() );
    lv_style_set_text_font( &jit_pairing_style, LV_STATE_DEFAULT, &Ubuntu_16px);
    lv_obj_add_style( jit_pairing_tile, LV_OBJ_PART_MAIN, &jit_pairing_style );

   // lv_obj_t *jit_pairing_exit_btn = wf_add_image_button( jit_pairing_tile, cancel_32px, exit_jit_pairing_event_cb, &jit_pairing_style);
   // lv_obj_align( jit_pairing_exit_btn, jit_pairing_tile, LV_ALIGN_IN_TOP_LEFT, 10, 10 );

    jit_pairing_img = lv_img_create( jit_pairing_tile, NULL );
    lv_img_set_src( jit_pairing_img, &update_64px );
    lv_obj_align( jit_pairing_img, jit_pairing_tile, LV_ALIGN_IN_TOP_MID, 0, 10);

    jit_pairing_info_label = lv_label_create( jit_pairing_tile, NULL);
    lv_obj_add_style( jit_pairing_info_label, LV_OBJ_PART_MAIN, &jit_pairing_style  );
    lv_label_set_text( jit_pairing_info_label, "INSERT TOKEN");
    lv_obj_align( jit_pairing_info_label, jit_pairing_img, LV_ALIGN_IN_BOTTOM_MID, 0, 5 );

    jit_pairing_status_label = lv_label_create( jit_pairing_tile, NULL);
    lv_obj_add_style( jit_pairing_status_label, LV_OBJ_PART_MAIN, &jit_pairing_style  );
    lv_label_set_text( jit_pairing_status_label, "");
    lv_obj_align( jit_pairing_status_label, jit_pairing_tile, LV_ALIGN_IN_BOTTOM_LEFT, 2, -20);


    lv_obj_t *jit_pairing_code_cont = lv_obj_create( jit_pairing_tile, NULL );
    lv_obj_set_size(jit_pairing_code_cont, lv_disp_get_hor_res( NULL ) / 2 , 35 );
    lv_obj_add_style( jit_pairing_code_cont, LV_OBJ_PART_MAIN, &jit_pairing_style  );
    lv_obj_align( jit_pairing_code_cont, jit_pairing_info_label, LV_ALIGN_OUT_BOTTOM_MID, -40, 35 );
  
    jit_pairing_code_textfield = lv_textarea_create( jit_pairing_code_cont, NULL);
    lv_textarea_set_text( jit_pairing_code_textfield, "" );
    lv_textarea_set_pwd_mode( jit_pairing_code_textfield, true);
    lv_textarea_set_accepted_chars( jit_pairing_code_textfield, "-.0123456789.");
    lv_textarea_set_one_line( jit_pairing_code_textfield, true);
    lv_textarea_set_cursor_hidden( jit_pairing_code_textfield, true);

    lv_obj_set_width( jit_pairing_code_textfield, lv_disp_get_hor_res( NULL ) / 2 );
    lv_obj_set_height( jit_pairing_code_textfield, lv_disp_get_ver_res( NULL ) / 2 );
    lv_obj_align( jit_pairing_code_textfield, jit_pairing_tile, LV_ALIGN_CENTER, -40, 50);

    //Add button for SPIFFS format
    login_btn = lv_btn_create( jit_pairing_tile, NULL);
    lv_obj_set_event_cb( login_btn, login_btn_event_cb );
    lv_obj_set_size( login_btn, 60, 35);
    lv_obj_add_style( login_btn, LV_BTN_PART_MAIN, ws_get_button_style() );
    lv_obj_align( login_btn, jit_pairing_code_textfield, LV_ALIGN_OUT_RIGHT_MID, 15, -40);
    lv_obj_t *login_btn_label = lv_label_create( login_btn, NULL );
    lv_label_set_text( login_btn_label, "LOGIN");

    lv_obj_set_event_cb( jit_pairing_code_textfield, jabil_pairing_num_textarea_event_cb );

    xLoginCtrlEvent=xEventGroupCreate();
    loginctrl_clear_event(LOGIN_DONE|LOGOUT_REQUEST|LOGOUT_DONE); 

    powermgm_register_loop_cb( POWERMGM_SILENCE_WAKEUP | POWERMGM_STANDBY | POWERMGM_WAKEUP, jit_pairing_powermgm_loop_cb, "jitsupport app loop" );
    jit_pairing_login_screen();

}

bool jit_pairing_login_screen() {
    
    statusbar_hide( true );
    powermgm_set_event( POWERMGM_WAKEUP_REQUEST );
    mainbar_jump_to_tilenumber( jit_pairing_tile_num, LV_ANIM_OFF );
    lv_label_set_text( jit_pairing_info_label, "INSERT TOKEN" );
    lv_obj_align( jit_pairing_info_label, jit_pairing_img, LV_ALIGN_OUT_BOTTOM_MID, 0, 5 );
    lv_obj_invalidate( lv_scr_act() );
    //motor_vibe(20);

    return( true );
} 

bool jit_pairing_powermgm_loop_cb( EventBits_t event, void *arg ) {

   TTGOClass *ttgo = TTGOClass::getWatch();

   if(event==POWERMGM_WAKEUP){

       switch(login_state){

           case(LOGIN_CLEAN):

            lv_textarea_set_text( jit_pairing_code_textfield,"");
            lv_label_set_text( jit_pairing_status_label, "" );
            login_state=LOGIN_SCREEN;

           break;

           case(LOGIN_SCREEN):

            if(pmu_is_vbus_plug() || (start_login_flag==true))
            {
                //start_login_flag=true;
                jit_pairing_login_screen();
            }
            else
            {
                //Start Login Falso e cabo USB desplugado 
                //Volta para o estado Login Clean 
                start_login_flag=false;
                login_state=LOGIN_CLEAN;                               
            } 

           break;

           case(LOGIN_IN_PROCESS):
    
            start_login_flag=false;

           break;
       
           case(CHECK_FOR_LOGOUT):

                if(pmu_is_vbus_plug())
                {
                    //jit_pairing_login_screen();
                    login_state=LOGIN_CLEAN;
                }

           break;
    
        }

    }
    else
    {   
        login_state=LOGIN_CLEAN;
    }

return( true );
}


static void jabil_pairing_num_textarea_event_cb ( lv_obj_t * obj, lv_event_t event ) {
    if( event == LV_EVENT_CLICKED ) {

        char token[10];
        num_keyboard_set_textarea(obj);


        if(strcmp(token,"123456")==0)
        {          
            mainbar_jump_to_maintile( LV_ANIM_OFF );
            start_login_flag=false;
            login_state=CHECK_FOR_LOGOUT;

        }
    }
}

static void exit_jit_pairing_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       mainbar_jump_to_maintile( LV_ANIM_OFF );
                                        break;
    }
}

static void login_btn_event_cb(lv_obj_t * obj, lv_event_t event){

    switch( event ) {
        case( LV_EVENT_CLICKED ): 

        strlcpy( token, lv_textarea_get_text( jit_pairing_code_textfield), sizeof(token) );
        log_i("Peguei o token: %s",token);

        //---- Task para Reestabelecimento da Conexão MQTT
        xTaskCreatePinnedToCore( login_task,                               /* Function to implement the task */
                                "login_Task",                                 /* Name of the task */
                                5000,                                         /* Stack size in words */
                                NULL,                                         /* Task input parameter */
                                2,                                            /* Priority of the task */
                                &_login_task,                              /* Task handle. */
                                0);      
        break;
    }

}


void login_task(void * pvParameters)
{

  log_i("Inicialização de Login ");

  while(1)
  {           
      lv_label_set_text( jit_pairing_status_label, "CHECKING TOKEN...");
      vTaskDelay(1000/ portTICK_PERIOD_MS );
      
      if(check_token())
      {
        // LOGIN SUCCESS 
       
        loginctrl_set_event(LOGIN_DONE);
        loginctrl_send_event_cb( LOGIN_DONE, (int *)UserID );

        lv_label_set_text( jit_pairing_status_label, "OK READY TO GO... WELCOME !");          
        vTaskDelay(1000/ portTICK_PERIOD_MS );
        start_login_flag=false;
        login_state=CHECK_FOR_LOGOUT;
        mainbar_jump_to_maintile( LV_ANIM_OFF );

      }

    vTaskDelete(NULL);
  }

}

bool check_token(){
        
    if(wifi_connected==1)
    {
            
        HTTPClient http;
        char post[100];
        http.begin(LOGIN_API_URL);
        http.addHeader("Content-Type", "application/json");

        //---------- Create JSON POST-----------
        StaticJsonDocument<200> doc;
        doc["token"] = token;
        doc["mac"] = "BRBELME024";
    
        String postBody;
        serializeJson(doc, postBody);
        postBody.toCharArray(post,sizeof(post));
        //--------------------------------------

        log_i("HTTP POSTing... ");
        int httpResponseCode = http.POST(postBody);

        log_i("HTTP POSTing response.. ");

            String payload = http.getString();
     
            if(httpResponseCode == HTTP_CODE_OK) {

                //--------- READING RESPONSE-------
                
                StaticJsonDocument<400> doc;
                DeserializationError error = deserializeJson(doc, payload);

                if(error)
                {
                    log_i("Error deserializeJson during Login: %s", error.f_str()); 
                    lv_label_set_text( jit_pairing_status_label, "Login Error: HTTP OK Json Deserialize ");
                    return false;
                }

                
                const char* Status = doc["Status"];
                bool Success = doc["Success"];
                UserID = doc["UserId"]; 

                lv_label_set_text( jit_pairing_status_label, Status);
                
                log_i("User id pego: %d", UserID);
                
               
                http.end();
                return Success;  //Return true case LOGIN OK 
             
              } 
              else {

                //---------- ERROR MESSAGE ------------

                StaticJsonDocument<200> doc;
                DeserializationError error = deserializeJson(doc, payload);

                if(error)
                {
                    log_i("Error deserializeJson during Login: %s", error.f_str()); 
                    lv_label_set_text( jit_pairing_status_label, "Login Error: HTTP NOK Json Deserialize !");
                    return false;
                }

                const char* Message = doc["Message"];
                lv_label_set_text( jit_pairing_status_label, Message);
                http.end();

                  return false;
              }
    }
    else
    {

        lv_label_set_text( jit_pairing_status_label, "No Wifi !");
        return false;

    }

return true;
          
}


void loginctrl_set_event( EventBits_t bits ){
    xEventGroupSetBits( xLoginCtrlEvent, bits);
}

void loginctrl_clear_event( EventBits_t bits){
    xEventGroupClearBits( xLoginCtrlEvent, bits);
}

EventBits_t loginctrl_get_event( EventBits_t bits){

    EventBits_t temp = xEventGroupGetBits( xLoginCtrlEvent ) & bits;

    return( temp );
}


bool loginctrl_register_cb( EventBits_t event, CALLBACK_FUNC callback_func, const char *id ) {
    if ( loginctrl_callback == NULL ) {
        loginctrl_callback = callback_init( "LoginCtrl");
        if ( loginctrl_callback == NULL ) {
            log_e("LoginCtrl Callback alloc failed");
            while(true);
        }
    }    
    return(callback_register( loginctrl_callback, event, callback_func, id ));
}

bool loginctrl_send_event_cb( EventBits_t event, void *arg ) {
    return( callback_send( loginctrl_callback, event, arg ) );
}


