/******* STARTUP PANEL *******/

void startPanels(){
  osd.clear();
  // Display our logo  
  panLogo(8,5);
 
}

/******* PANELS - POSITION *******/

void writePanels(){ 
    //  if (osd_off == 1){
    //  if (osd_set != 2){
    //    osd.clear();}
    //  }

    if (EEPROM.read(200) == 0){
        converts = 3.6;
        converth = 1;
        spe = 0x81;
        high = 0x8D;
    }
    else{
        converts = 2.23;
        converth = 3.28;
        spe = 0xfb;
        high = 0x66;
    }
    if(millis() < (lastMAVBeat + 2000)){

        //osd.clear();
        //Testing bits from 8 bit register A 
        if(ISa(Cen_BIT)) panCenter(panCenter_XY[0], panCenter_XY[1]);   //4x2
        if(ISa(Pit_BIT)) panPitch(panPitch_XY[0], panPitch_XY[1]); //5x1
        if(ISa(Rol_BIT)) panRoll(panRoll_XY[0], panRoll_XY[1]); //5x1
        if(ISa(BatA_BIT)) panBatt_A(panBatt_A_XY[0], panBatt_A_XY[1]); //7x1
        //  if(ISa(BatB_BIT)) panBatt_B(panBatt_B_XY[0], panBatt_B_XY[1]); //7x1
        if(ISa(GPSats_BIT)) panGPSats(panGPSats_XY[0], panGPSats_XY[1]); //5x1
        if(ISa(GPL_BIT)) panGPL(panGPL_XY[0], panGPL_XY[1]); //2x1
        if(ISa(GPS_BIT)) panGPS(panGPS_XY[0], panGPS_XY[1]); //12x3
        if(ISa(Bp_BIT)) panBatteryPercent(panBatteryPercent_XY[0], panBatteryPercent_XY[1]); //

        //Testing bits from 8 bit register B
        if(ISb(Rose_BIT)) panRose(panRose_XY[0], panRose_XY[1]);        //13x3
        if(ISb(Head_BIT)) panHeading(panHeading_XY[0], panHeading_XY[1]); //13x3
        if(ISb(MavB_BIT)) panMavBeat(panMavBeat_XY[0], panMavBeat_XY[1]); //13x3

        if(osd_got_home == 1){
            if(ISb(HDis_BIT)) panHomeDis(panHomeDis_XY[0], panHomeDis_XY[1]); //13x3
            if(ISb(HDir_BIT)) panHomeDir(panHomeDir_XY[0], panHomeDir_XY[1]); //13x3
        }
        if(ISb(Time_BIT)) panTime(panTime_XY[0], panTime_XY[1]);
        //  if(ISb(WDir_BIT)) panWayPDir(panWayPDir_XY[0], panWayPDir_XY[1]); //??x??
        //  if(ISb(WDis_BIT)) panWayPDis(panWayPDis_XY[0], panWayPDis_XY[1]); //??x??
        //  if(ISb(WRSSI_BIT)) panRSSI(panRSSI_XY[0], panRSSI_XY[1]); //??x??

        //Testing bits from 8 bit register C 
        //if(osd_got_home == 1){
        if(ISc(Alt_BIT)) panAlt(panAlt_XY[0], panAlt_XY[1]); //
        if(ISc(Halt_BIT)) panHomeAlt(panHomeAlt_XY[0], panHomeAlt_XY[1]); //
        if(ISc(Vel_BIT)) panVel(panVel_XY[0], panVel_XY[1]); //
        if(ISc(As_BIT)) panAirSpeed(panAirSpeed_XY[0], panAirSpeed_XY[1]); //

        //}
        if(ISc(Thr_BIT)) panThr(panThr_XY[0], panThr_XY[1]); //
        if(ISc(FMod_BIT)) panFlightMode(panFMod_XY[0], panFMod_XY[1]);  //
        if(ISc(Hor_BIT)) panHorizon(panHorizon_XY[0], panHorizon_XY[1]); //14x5
        if(ISc(CurA_BIT)) panCur_A(panCur_A_XY[0], panCur_A_XY[1]);

        //Testing bits from 8 bit register D 
        if(ISd(Warn_BIT)) panWarn(panWarn_XY[0], panWarn_XY[1]);
        if(ISd(Off_BIT)) panOff(panOff_XY[0], panOff_XY[1]);
        if(ISd(WindS_BIT)) panWindSpeed(panWindSpeed_XY[0], panWindSpeed_XY[1]);
        if(ISd(Climb_BIT)) panClimb(panClimb_XY[0], panClimb_XY[1]);
        //    if(ISd(Tune_BIT)) panTune(panTune_XY[0], panTune_XY[1]);
        if(ISd(Setup_BIT)) panSetup(panSetup_XY[0], panSetup_XY[1]);
    }

    else{
        osd.clear();
        waitingMAVBeats = 1;
        // Display our logo and wait... 
        panWaitMAVBeats(5,10); //Waiting for MAVBeats...
    }


    // OSD debug for development (Shown on top-middle panels) 
#ifdef membug
    osd.setPanel(13,4);
    osd.openPanel();
    osd.printf("%i",freeMem()); 
    osd.closePanel();
#endif

}

/******* PANELS - DEFINITION *******/

/* **************************************************************** */
// Panel  : panSetup
// Needs  : X, Y locations
// Output : Alt symbol and altitude value in meters from MAVLink
// Size   : 1 x 7Hea  (rows x chars)
// Staus  : done

void panSetup(int first_col, int first_line){

    osd.setPanel(first_col, first_line);
    osd.openPanel();

    if (chan1_raw_middle == 0 && chan2_raw_middle == 0){
        chan1_raw_middle = chan1_raw;
        chan2_raw_middle = chan2_raw;
    }


    if (osd_set == 1){
        if (millis() - a > 500){
            a = millis();
            if ((chan2_raw - 100) > chan2_raw_middle ){
                setup_menu = setup_menu + 1;
            }
            else if ((chan2_raw + 100) < chan2_raw_middle ){
                setup_menu = setup_menu - 1;
            }
            if (setup_menu < 0){
                setup_menu = 0;
            }
            if (setup_menu > 3){
                setup_menu = 3;}

        }
        switch (setup_menu){
        case 0:
            if (millis() - b > 500){
                b = millis();
                if (EEPROM.read(200) == 0){
                    osd.printf_P(PSTR("    metric system    "));
                    if ((chan1_raw - 100) > chan1_raw_middle || (chan1_raw + 100) < chan1_raw_middle ){
                        EEPROM.write(200, 1);

                    }
                }
                else {
                    osd.printf_P(PSTR("      US system       "));
                    if ((chan1_raw - 100) > chan1_raw_middle || (chan1_raw + 100) < chan1_raw_middle){
                        EEPROM.write(200, 0);

                    }
                }
            }
            break;
        case 1:
            osd.printf_P(PSTR("    Overspeed    "));
            osd.printf("%3.0i%c", overspeed, spe);
            if ((chan1_raw - 100) > chan1_raw_middle ){
                overspeed = overspeed - 1;}
            if ((chan1_raw + 100) < chan1_raw_middle ){
                overspeed = overspeed + 1;} 
            EEPROM.write(202, overspeed);
            break;
        case 2:
            osd.printf_P(PSTR("   Stall Speed   "));
            osd.printf("%3.0i%c", stall , spe);
            if ((chan1_raw - 100) > chan1_raw_middle ){
                stall = stall - 1;}
            if ((chan1_raw + 100) < chan1_raw_middle ){
                stall = stall + 1;} 
            EEPROM.write(204, stall);
            break;
        case 3:
            osd.printf_P(PSTR("Battery warning "));
            osd.printf("%3.1f%c", battv , 0x76, 0x20);
            if ((chan1_raw - 100) > chan1_raw_middle ){
                battv = battv - 0.1;}
            if ((chan1_raw + 100) < chan1_raw_middle ){
                battv = battv + 0.1;} 
            EEPROM.write(206, battv);
            break;
            //      case 4:
            //        osd.printf_P(PSTR("Battery warning "));
            //        osd.printf("%3.0i%c", battp , 0x25);
            //        if ((chan1_raw - 100) > chan1_raw_middle ){
            //        battp = battp - 1;}
            //        if ((chan1_raw + 100) < chan1_raw_middle ){
            //        battp = battp + 1;} 
            //        EEPROM.write(208, battp);
            //        break;
        }
    }

    osd.closePanel();
}

/* **************************************************************** */
// Panel  : pan wind speed
// Needs  : X, Y locations
// Output : Velocity value from MAVlink with symbols
// Size   : 1 x 7  (rows x chars)
// Staus  : done

void panWindSpeed(int first_col, int first_line){
    osd.setPanel(first_col, first_line);
    osd.openPanel();
    if (osd_off == 0){
        //if (osd_airspeed > 6){
        //    if (heding_check == -2){
        //        heding_check = osd_heading;
        //    }
        //    if (millis() - wind_time > 60000){
        //        wind_time = millis();
        //        if (heding_check == -1){
        //            osd_windspeed = osd_windspeed_check;
        //            osd_winddirection = osd_winddirection_check;
        //            osd_windspeed_check = 0;
        //            wind = 1;
        //        }
        //        heding_check = osd_heading;      

        //    }

        //    if ((heding_check - osd_heading) < 0 ){
        //        if (((heding_check - osd_heading) * -1) >= 177.5 && ((heding_check - osd_heading) * -1) <= 182.5){
        //            heding_check = -1;
        //        }
        //    }
        //    else if ((heding_check - osd_heading) >= 177.5 && (heding_check - osd_heading) <= 182.5){
        //        heding_check = -1;
        //    }

        //    if (osd_airspeed > osd_groundspeed){
        //        if ((osd_airspeed - osd_groundspeed) > osd_windspeed_check){
        //            osd_windspeed_check = (osd_airspeed - osd_groundspeed);
        //            if (osd_heading > 180){
        //                osd_winddirection_check = (osd_heading - 180);}
        //            else {
        //                osd_winddirection_check = (osd_heading + 180);
        //            }
        //        }
        //    }

        //    else if (osd_groundspeed > osd_airspeed){
        //        if ((osd_groundspeed - osd_airspeed) > osd_windspeed_check){
        //            osd_windspeed_check = (osd_groundspeed - osd_airspeed);
        //            osd_winddirection_check = osd_heading;
        //        }
        //    }  

        //}
        //
        //osd_wind_arrow_rotate = osd_winddirection - osd_heading; //I don't really know if we should do this, because I think wind direction is already in airplane body reference
        //if (osd_winddirection - osd_heading < 0){
        //    osd_wind_arrow_rotate = osd_wind_arrow_rotate + 360;
        //}

        //osd_wind_arrow_rotate_int = round((float)(osd_wind_arrow_rotate/360.0f) * 16.0f) + 1;//Convert to int 1-16
        osd_wind_arrow_rotate_int = round((float)(osd_winddirection/360.0f) * 16.0f) + 1;//Convert to int 1-16
        if(osd_wind_arrow_rotate_int > 16) osd_home_direction = 0;
        showWindOSD();
    }
    osd.closePanel();
}

/* **************************************************************** */
// Panel  : panOff
// Needs  : X, Y locations
// Output : OSD off
// Size   : 1 x 7Hea  (rows x chars)
// Staus  : done

void panOff(int first_col, int first_line){
    osd.setPanel(first_col, first_line);
    osd.openPanel();

    if (((apm_mav_type == 1) && ((osd_mode != 11) && (osd_mode != 1))) || ((apm_mav_type == 2) && ((osd_mode != 6) && (osd_mode != 7)))){
        if (osd_off_switch != osd_mode){ 
            osd_off_switch = osd_mode;
            osd_switch_time = millis();

            if (osd_off_switch == osd_switch_last){
                if (osd_off == 1){
                    osd_off = 0;
                    osd_set = 0;
                    osd.clear();
                }
                else {
                    osd_off = 1;
                    osd.clear();
                    if (millis() <= 60000){
                        osd_set = 1;  
                    }
                }
            }
        }
        if ((millis() - osd_switch_time) > 2000){
            osd_switch_last = osd_mode;
        }
    }

    osd.closePanel();
}

/* **************************************************************** */
// Panel  : panTune
// Needs  : X, Y locations
// Output : Current symbol and altitude value in meters from MAVLink
// Size   : 1 x 7Hea  (rows x chars)
// Staus  : done

//void panTune(int first_col, int first_line){
//  osd.setPanel(first_col, first_line);
//  osd.openPanel();
//  if (osd_off == 0){
//  osd.printf("%c%c%3.0f%c|%c%c%3.0f%c|%c%c%3.0i%c|%c%c%3.0i%c|%c%c%3.0i%c|%c%c%3.0f%c|%c%c%3.0f%c|%c%c%3.0f%c", 0x4E, 0x52, (nav_roll), 0xB0, 0x4E, 0x50, (nav_pitch), 0xB0, 0x4E, 0x48, (nav_bearing), 0xB0, 0x54, 0x42, (target_bearing), 0xB0, 0x57, 0x44, (wp_dist), 0x6D, 0x41, 0x45, (alt_error), 0x6D, 0x58, 0x45, (xtrack_error), 0x6D, 0x41, 0x45, (aspd_error * 3.6), 0x81);
//  }
 
//  osd.closePanel();
//}

/* **************************************************************** */
// Panel  : panCur_A
// Needs  : X, Y locations
// Output : Current symbol and altitude value in meters from MAVLink
// Size   : 1 x 7Hea  (rows x chars)
// Staus  : done

void panCur_A(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
  if (osd_off == 0){
  osd.printf("%c%5.2f%c", 0xE4, (osd_curr_A * .01), 0x8F);
  }

  osd.closePanel();
}

/* **************************************************************** */
// Panel  : panAlt
// Needs  : X, Y locations
// Output : Alt symbol and altitude value in meters from MAVLink
// Size   : 1 x 7Hea  (rows x chars)
// Staus  : done

void panAlt(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
  //osd.printf("%c%5.0f%c",0x85, (double)(osd_alt - osd_home_alt), 0x8D);
  if (osd_off == 0){
  osd.printf("%c%5.0f%c",0xE6, (double)(osd_alt * converth), high);
  }

  osd.closePanel();
}

/* **************************************************************** */
// Panel  : panClimb
// Needs  : X, Y locations
// Output : Alt symbol and altitude value in meters from MAVLink
// Size   : 1 x 7Hea  (rows x chars)
// Staus  : done

void panClimb(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
  if (osd_off == 0){
  osd.printf("%c%3.0f%c",0x16, (double)(osd_climb), 0x88);
  }

  osd.closePanel();
}

/* **************************************************************** */
// Panel  : panHomeAlt
// Needs  : X, Y locations
// Output : Alt symbol and home altitude value in meters from MAVLink
// Size   : 1 x 7Hea  (rows x chars)
// Staus  : done

void panHomeAlt(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
  //osd.printf("%c%5.0f%c",0x85, (double)(osd_alt - osd_home_alt), 0x8D);
  if (osd_off == 0){
  osd.printf("%c%5.0f%c",0xE7, (double)((osd_alt - osd_home_alt) * converth), high);
  }

  osd.closePanel();
}

/* **************************************************************** */
// Panel  : panVel
// Needs  : X, Y locations
// Output : Velocity value from MAVlink with symbols
// Size   : 1 x 7  (rows x chars)
// Staus  : done

void panVel(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
  if (osd_off == 0){
  osd.printf("%c%3.0f%c",0xE9,(double)(osd_groundspeed * converts),spe);
  }

  osd.closePanel();
}

/* **************************************************************** */
// Panel  : panAirSpeed
// Needs  : X, Y locations
// Output : Airspeed value from MAVlink with symbols
// Size   : 1 x 7  (rows x chars)
// Staus  : done

void panAirSpeed(int first_col, int first_line){
    osd.setPanel(first_col, first_line);
    osd.openPanel();
    if (osd_off == 0){
        osd.printf("%c%3.0f%c", 0xE8, (double)(osd_airspeed * converts), spe);
    }

    osd.closePanel();
}

/* **************************************************************** */
// Panel  : panWarn
// Needs  : X, Y locations
// Output : Airspeed value from MAVlink with symbols
// Size   : 1 x 7  (rows x chars)
// Staus  : done

void panWarn(int first_col, int first_line){
     osd.setPanel(first_col, first_line);
     osd.openPanel();
     
     if (osd_set == 0){
          if ((start_Time - text_blink) > 0){
              if (warning_type == 1){
                  warning_type = 0;}
              else{
                  if (warning_type > 1){ }
                  else{
                      if ((osd_fix_type) < 2){ 
                          warning_type = 1;
                          text_blink = start_Time + 1;
                          goto Warning;}
                  }
              }

              if (warning_type == 2){
                  warning_type = 0;}
              else{
                  if (warning_type > 2){}
                  else{
                      if ((osd_airspeed * converts) < stall && (osd_airspeed) > 1.12){
                          warning_type = 2;
                          text_blink = start_Time + 1;
                          goto Warning;}
                  }
              }
              if (warning_type == 3){
                  warning_type = 0; }
              else{
                  if (warning_type > 3){}
                  else {
                      if ((osd_airspeed * converts) > overspeed){
                          warning_type = 3;
                          text_blink = start_Time + 1;
                          goto Warning;}
                  }
              }

              if (warning_type == 4){
                  warning_type = 0; }
              else{
                  if (warning_type > 4){}
                  else {
                      if (osd_vbat_A < battv){
                          warning_type = 4;
                          text_blink = start_Time + 1;
                          goto Warning;}
                  }
              }

              if (warning_type == 5){
                  warning_type = 0; }
              else{
                  if (warning_type > 5){}
                  else {
                      if (osd_battery_remaining_A < 10){
                          warning_type = 5;
                          text_blink = start_Time + 1;
                          goto Warning;}
                  }
              }
              //if (warning_type == 6){
              //  warning_type = 0; }
              //      else{
              //    if (warning_type > 6){}
              //      else {
              //      if ((osd_alt - osd_home_alt) < 10 && (osd_alt - osd_home_alt) > 0){
              //      warning_type = 6;
              //text_blink = start_Time + 1;
              //      goto Warning;}
              //    }
              //}
          }
Warning:
          switch(warning_type){
          case 0:
              osd.printf_P(PSTR("\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"));
              break;   
          case 1:  
              osd_off = 0;
              osd.printf_P(PSTR("\x20\x4E\x6F\x20\x47\x50\x53\x20\x66\x69\x78\x21"));
              break;
          case 2:
              osd_off = 0;
              osd.printf_P(PSTR("\x20\x20\x20\x53\x74\x61\x6c\x6c\x21\x20\x20\x20"));
              break;
          case 3:
              osd_off = 0;
              osd.printf_P(PSTR("\x20\x4f\x76\x65\x72\x53\x70\x65\x65\x64\x21\x20"));
              break;
          case 4:
              osd_off = 0;
              osd.printf_P(PSTR("\x42\x61\x74\x74\x65\x72\x79\x20\x4c\x6f\x77\x21"));
              break;
          case 5:
              osd_off = 0;
              osd.printf_P(PSTR("\x42\x61\x74\x74\x65\x72\x79\x20\x4c\x6f\x77\x21"));
              break;
              //  case 6:
              //    osd_off = 0;
              //    osd.printf_P(PSTR("\x20\x20\x50\x75\x6c\x6c\x20\x55\x70\x21\x20\x20"));
              //    break;
          }

      }

      osd.closePanel();
}
/* **************************************************************** */
// Panel  : panThr
// Needs  : X, Y locations
// Output : Throttle value from MAVlink with symbols
// Size   : 1 x 7  (rows x chars)
// Staus  : done

void panThr(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
  if (osd_off == 0){
  osd.printf("%c%3.0i%c",0x87,osd_throttle,0x25);
  }

  osd.closePanel();
}

/* **************************************************************** */
// Panel  : panBatteryPercent
// Needs  : X, Y locations
// Output : Battery state from MAVlink with symbols
// Size   : 1 x 7  (rows x chars)
// Staus  : done

void panBatteryPercent(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
  if (osd_off == 0){
  osd.printf("%c%3.0i%c", 0xB9, (osd_battery_remaining_A), 0x25);
  }

  osd.closePanel();
}

/* **************************************************************** */
// Panel  : panTime
// Needs  : X, Y locations
// Output : Time from start with symbols
// Size   : 1 x 7  (rows x chars)
// Staus  : done

void panTime(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
  start_Time = millis()/1000;
  if (osd_off == 0){
  osd.printf("%c%2i%c%02i", 0xB3,(start_Time/60)%60,0x3A,start_Time%60);
  }

  osd.closePanel();
}

/* **************************************************************** */
// Panel  : panHomeDis
// Needs  : X, Y locations
// Output : Home Symbol with distance to home in meters
// Size   : 1 x 7  (rows x chars)
// Staus  : done

void panHomeDis(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
  if (osd_off == 0){
  osd.printf("%c%5.0f%c", 0x1F, (double)((osd_home_distance) * converth), high);
  }

  osd.closePanel();
}

/* **************************************************************** */
// Panel  : panCenter
// Needs  : X, Y locations
// Output : 2 row croshair symbol created by 2 x 4 chars
// Size   : 2 x 4  (rows x chars)
// Staus  : done

void panCenter(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
  if (osd_off == 0){
  osd.printf_P(PSTR("\x05\x03\x04\x05|\x15\x13\x14\x15"));
  }

  osd.closePanel();
}

/* **************************************************************** */
// Panel  : panHorizon
// Needs  : X, Y locations
// Output : 12 x 4 Horizon line surrounded by 2 cols (left/right rules)
// Size   : 14 x 4  (rows x chars)
// Staus  : done

void panHorizon(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
  if (osd_off == 0){
  osd.printf_P(PSTR("\xc8\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\xc9|"));
  osd.printf_P(PSTR("\xc8\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\xc9|"));
  osd.printf_P(PSTR("\xd8\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\xd9|"));
  osd.printf_P(PSTR("\xc8\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\xc9|"));
  osd.printf_P(PSTR("\xc8\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\xc9"));
  }

  osd.closePanel();
  showHorizon((first_col + 1), first_line);
}

/* **************************************************************** */
// Panel  : panPitch
// Needs  : X, Y locations
// Output : -+ value of current Pitch from vehicle with degree symbols and pitch symbol
// Size   : 1 x 6  (rows x chars)
// Staus  : done

void panPitch(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
  if (osd_off == 0){
  osd.printf("%4i%c%c",osd_pitch,0xb0,0xb1);
  }

  osd.closePanel();
}

/* **************************************************************** */
// Panel  : panRoll
// Needs  : X, Y locations
// Output : -+ value of current Roll from vehicle with degree symbols and roll symbol
// Size   : 1 x 6  (rows x chars)
// Staus  : done

void panRoll(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
  if (osd_off == 0){
  osd.printf("%4i%c%c",osd_roll,0xb0,0xb2);
  }

  osd.closePanel();
}

/* **************************************************************** */
// Panel  : panBattery A (Voltage 1)
// Needs  : X, Y locations
// Output : Voltage value as in XX.X and symbol of over all battery status
// Size   : 1 x 8  (rows x chars)
// Staus  : done

void panBatt_A(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
/*************** This commented code is for the next ArduPlane Version
#ifdef MAVLINK10
  if(osd_battery_remaining_A > 100){
    osd.printf(" %c%5.2f%c", 0xE2, (double)osd_vbat_A, 0x8E);
  }
#else
  if(osd_battery_remaining_A > 1000){
    osd.printf(" %c%5.2f%c", 0xE2, (double)osd_vbat_A, 0x8E);
  }
#endif //MAVLINK10
  else osd.printf("%c%5.2f%c%c", 0xE2, (double)osd_vbat_A, 0x8E, osd_battery_pic_A);
*/
  if (osd_off == 0){
  osd.printf("%c%5.2f%c", 0xE2, (double)osd_vbat_A, 0x8E);
  }

  osd.closePanel();
}

//------------------ Panel: Startup ArduCam OSD LOGO -------------------------------

void panLogo(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
  overspeed = EEPROM.read(202);
  stall = EEPROM.read(204);
  battv = EEPROM.read(206);
//  battp = EEPROM.read(208);
  EEPROM.write(194, 1);
  EEPROM.write(196, 5);
  EEPROM.write(198, 7);
  
  {
  osd.printf_P(PSTR("\x20\x20\x20\x20\x20\xba\xbb\xbc\xbd\xbe|\x20\x20\x20\x20\x20\xca\xcb\xcc\xcd\xce|MinimOSD Extra|    1.29.0"));
  }

  osd.closePanel();
}

//------------------ Panel: Waiting for MAVLink HeartBeats -------------------------------

void panWaitMAVBeats(int first_col, int first_line){
  panLogo(8,5);
  osd.setPanel(first_col, first_line);
  osd.openPanel();
  osd.printf_P(PSTR("Waiting for|MAVLink heartbeats..."));
  osd.closePanel();
}

/* **************************************************************** */
// Panel  : panGPL
// Needs  : X, Y locations
// Output : 1 static symbol with changing FIX symbol
// Size   : 1 x 2  (rows x chars)
// Staus  : done

void panGPL(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
  if (osd_off == 0){
  switch(osd_fix_type) {
    case 0: 
      osd.printf_P(PSTR("\x10\x20"));
      break;
    case 1: 
      osd.printf_P(PSTR("\x10\x20"));
      break;
    case 2: 
      osd.printf_P(PSTR("\x11\x20"));//If not APM, x01 would show 2D fix
      break;
    case 3: 
      osd.printf_P(PSTR("\x11\x20"));//If not APM, x02 would show 3D fix
      break;
  }
  }

    /*  if(osd_fix_type <= 1) {
    osd.printf_P(PSTR("\x10"));
  } else {
    osd.printf_P(PSTR("\x11"));
  }  */
  osd.closePanel();
}

/* **************************************************************** */
// Panel  : panGPSats
// Needs  : X, Y locations
// Output : 1 symbol and number of locked satellites
// Size   : 1 x 5  (rows x chars)
// Staus  : done

void panGPSats(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
  if (osd_off == 0){
  osd.printf("%c%2i", 0x0f,osd_satellites_visible);
  }

  osd.closePanel();
}

/* **************************************************************** */
// Panel  : panGPS
// Needs  : X, Y locations
// Output : two row numeric value of current GPS location with LAT/LON symbols as on first char
// Size   : 2 x 12  (rows x chars)
// Staus  : done

void panGPS(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
  if (osd_off == 0){
  osd.printf("%c%11.6f|%c%11.6f", 0x83, (double)osd_lat, 0x84, (double)osd_lon);
  }

  osd.closePanel();
}

/* **************************************************************** */
// Panel  : panHeading
// Needs  : X, Y locations
// Output : Symbols with numeric compass heading value
// Size   : 1 x 5  (rows x chars)
// Staus  : not ready

void panHeading(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
  if (osd_off == 0){
    osd.printf("%4.0f%c", (double)osd_heading, 0xb0);
  }

  osd.closePanel();
}

/* **************************************************************** */
// Panel  : panRose
// Needs  : X, Y locations
// Output : a dynamic compass rose that changes along the heading information
// Size   : 2 x 13  (rows x chars)
// Staus  : done

void panRose(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
  //osd_heading  = osd_yaw;
  //if(osd_yaw < 0) osd_heading = 360 + osd_yaw;
  if (osd_off == 0){
  osd.printf("%s|%c%s%c", "\x20\xc0\xc0\xc0\xc0\xc0\xc7\xc0\xc0\xc0\xc0\xc0\x20", 0xd0, buf_show, 0xd1);
  }

  osd.closePanel();
}


/* **************************************************************** */
// Panel  : panBoot
// Needs  : X, Y locations
// Output : Booting up text and empty bar after that
// Size   : 1 x 21  (rows x chars)
// Staus  : done

void panBoot(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
  osd.printf_P(PSTR("Booting up:\xed\xf2\xf2\xf2\xf2\xf2\xf2\xf2\xf3")); 
  osd.closePanel();

}

/* **************************************************************** */
// Panel  : panMavBeat
// Needs  : X, Y locations
// Output : 2 symbols, one static and one that blinks on every 50th received 
//          mavlink packet.
// Size   : 1 x 2  (rows x chars)
// Staus  : done

void panMavBeat(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
 if (osd_off == 0){
  if(mavbeat == 1){
    osd.printf_P(PSTR("\xEA\xEC"));
    mavbeat = 0;
  }
  else{
    osd.printf_P(PSTR("\xEA\xEB"));
  }
  }

  osd.closePanel();
}


/* **************************************************************** */
// Panel  : panWPDir
// Needs  : X, Y locations
// Output : 2 symbols that are combined as one arrow, shows direction to next waypoint
// Size   : 1 x 2  (rows x chars)
// Staus  : not ready

void panWPDir(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
  showArrow(osd_home_direction);
  osd.closePanel();
}

/* **************************************************************** */
// Panel  : panHomeDir
// Needs  : X, Y locations
// Output : 2 symbols that are combined as one arrow, shows direction to home
// Size   : 1 x 2  (rows x chars)
// Status : not tested

void panHomeDir(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
  showArrow(osd_home_direction);
  osd.closePanel();
}

/* **************************************************************** */
// Panel  : panFlightMode 
// Needs  : X, Y locations
// Output : 2 symbols, one static name symbol and another that changes by flight modes
// Size   : 1 x 2  (rows x chars)
// Status : done

void panFlightMode(int first_col, int first_line){
  osd.setPanel(first_col, first_line);
  osd.openPanel();
if (osd_off == 0){
  if(apm_mav_type == 2){//ArduCopter MultiRotor or ArduCopter Heli
    if(osd_mode == 0) osd.printf_P(PSTR("\xE0""stab"));//Stabilize
    if(osd_mode == 1) osd.printf_P(PSTR("\xE0""acro"));//Acrobatic
    if(osd_mode == 2) osd.printf_P(PSTR("\xE0""alth"));//Alt Hold
    if(osd_mode == 3) osd.printf_P(PSTR("\xE0""auto"));//Auto
    if(osd_mode == 4) osd.printf_P(PSTR("\xE0""guid"));//Guided
    if(osd_mode == 5) osd.printf_P(PSTR("\xE0""loit"));//Loiter
    if(osd_mode == 6) osd.printf_P(PSTR("\xE0""retl"));//Return to Launch
    if(osd_mode == 7) osd.printf_P(PSTR("\xE0""circ")); // Circle
    if(osd_mode == 8) osd.printf_P(PSTR("\xE0""posi")); // Position
    if(osd_mode == 9) osd.printf_P(PSTR("\xE0""land")); // Land
    if(osd_mode == 10) osd.printf_P(PSTR("\xE0""oflo")); // OF_Loiter
  }
  else if(apm_mav_type == 1){//ArduPlane
    if(osd_mode == 2 ) osd.printf_P(PSTR("\xE0""stab"));//Stabilize
    if(osd_mode == 0) osd.printf_P(PSTR("\xE0""manu"));//Manual
    if(osd_mode == 12) osd.printf_P(PSTR("\xE0""loit"));//Loiter
    if(osd_mode == 11 ) osd.printf_P(PSTR("\xE0""retl"));//Return to Launch
    if(osd_mode == 5 ) osd.printf_P(PSTR("\xE0""fbwa"));//FLY_BY_WIRE_A
    if(osd_mode == 6 ) osd.printf_P(PSTR("\xE0""fbwb"));//FLY_BY_WIRE_B
    if(osd_mode == 15) osd.printf_P(PSTR("\xE0""guid"));//GUIDED
    if(osd_mode == 10 ) osd.printf_P(PSTR("\xE0""auto"));//AUTO
    if(osd_mode == 1) osd.printf_P(PSTR("\xE0""circ"));//CIRCLE
  }
}

  osd.closePanel();

}


// ---------------- EXTRA FUNCTIONS ----------------------
// Show those fancy 2 char arrows
void showArrow(uint8_t  arrow_dir) // Arrow direction pointing (1-16 to CW loop)
{           
    if (osd_off == 0){
        switch(arrow_dir) {
        case 0: 
            osd.printf_P(PSTR("\x90\x91"));
            break;
        case 1: 
            osd.printf_P(PSTR("\x90\x91"));
            break;
        case 2: 
            osd.printf_P(PSTR("\x92\x93"));
            break;
        case 3: 
            osd.printf_P(PSTR("\x94\x95"));
            break;
        case 4: 
            osd.printf_P(PSTR("\x96\x97"));
            break;
        case 5: 
            osd.printf_P(PSTR("\x98\x99"));
            break;
        case 6: 
            osd.printf_P(PSTR("\x9A\x9B"));
            break;
        case 7: 
            osd.printf_P(PSTR("\x9C\x9D"));
            break;
        case 8: 
            osd.printf_P(PSTR("\x9E\x9F"));
            break;
        case 9: 
            osd.printf_P(PSTR("\xA0\xA1"));
            break;
        case 10: 
            osd.printf_P(PSTR("\xA2\xA3"));
            break;
        case 11: 
            osd.printf_P(PSTR("\xA4\xA5"));
            break;
        case 12: 
            osd.printf_P(PSTR("\xA6\xA7"));
            break;
        case 13: 
            osd.printf_P(PSTR("\xA8\xA9"));
            break;
        case 14: 
            osd.printf_P(PSTR("\xAA\xAB"));
            break;
        case 15: 
            osd.printf_P(PSTR("\xAC\xAD"));
            break;
        case 16: 
            osd.printf_P(PSTR("\xAE\xAF"));
            break;
        }  
    }

}

// Show 2 char arrows
void showWindOSD() // Arrow direction pointing WIND (1-16 to CW loop) (0 is the same as 1)
{        
    char wind_arrow_set1;
    char wind_arrow_set2;   
    switch(osd_wind_arrow_rotate_int) {
    case 0: 
        wind_arrow_set1 = 0x90;
        wind_arrow_set2 = 0x91;
        break;
    case 1: 
        wind_arrow_set1 = 0x90;
        wind_arrow_set2 = 0x91;
        break;
    case 2: 
        wind_arrow_set1 = 0x92;
        wind_arrow_set2 = 0x93;
        break;
    case 3: 
        wind_arrow_set1 = 0x94;
        wind_arrow_set2 = 0x95;
        break;
    case 4: 
        wind_arrow_set1 = 0x96;
        wind_arrow_set2 = 0x97;
        break;
    case 5: 
        wind_arrow_set1 = 0x98;
        wind_arrow_set2 = 0x99;
        break;
    case 6: 
        wind_arrow_set1 = 0x9A;
        wind_arrow_set2 = 0x9B;
        break;
    case 7: 
        wind_arrow_set1 = 0x9C;
        wind_arrow_set2 = 0x9D;
        break;
    case 8: 
        wind_arrow_set1 = 0x9E;
        wind_arrow_set2 = 0x9F;
        break;
    case 9: 
        wind_arrow_set1 = 0xA0;
        wind_arrow_set2 = 0xA1;
        break;
    case 10: 
        wind_arrow_set1 = 0xA2;
        wind_arrow_set2 = 0xA3;
        break;
    case 11: 
        wind_arrow_set1 = 0xA4;
        wind_arrow_set2 = 0xA5;
        break;
    case 12: 
        wind_arrow_set1 = 0xA6;
        wind_arrow_set2 = 0xA7;
        break;
    case 13: 
        wind_arrow_set1 = 0xA8;
        wind_arrow_set2 = 0xA9;
        break;
    case 14: 
        wind_arrow_set1 = 0xAA;
        wind_arrow_set2 = 0xAB;
        break;
    case 15: 
        wind_arrow_set1 = 0xAC;
        wind_arrow_set2 = 0xAd;
        break;
    case 16: 
        wind_arrow_set1 = 0xAE;
        wind_arrow_set2 = 0xAF;
        break;
    default:
        wind_arrow_set1 = 0x20;
        wind_arrow_set2 = 0x20;
        break;
    }  
    if (wind == 1){
        osd.printf("%c%3.0f%c|%c%c",0xFC,(double)(osd_windspeed * converts),spe, wind_arrow_set1, wind_arrow_set2);
    }
}

// Calculate and shows Artificial Horizon
void showHorizon(int start_col, int start_row) { 

  uint8_t x, nose, row, minval, hit, subval = 0;
  //int x, nose, row, minval, hit, subval = 0;
  //int cols = 12;
  //int rows = 5;
  const uint8_t cols = 12;
  const uint8_t rows = 5;

  int col_hit[cols];
  float  pitch, roll;
   
  (abs(osd_pitch) == 90)?pitch = 89.99 * (90/osd_pitch) * -0.017453293:pitch = osd_pitch * -0.017453293;
  (abs(osd_roll) == 90)?roll = 89.99 * (90/osd_roll) * 0.017453293:roll = osd_roll * 0.017453293;

  nose = round(tan(pitch) * (rows*9));
  for(int col=1;col <= cols;col++){
    x = (col * 12) - (cols * 6) - 6;//center X point at middle of each col
    col_hit[col-1] = (tan(roll) * x) + nose + (rows*9) - 1;//calculating hit point on Y plus offset to eliminate negative values
    //col_hit[(col-1)] = nose + (rows * 9);
  }

  for(int col=0;col < cols; col++){
    hit = col_hit[col];
    if(hit > 0 && hit < (rows * 18)){
      row = rows - ((hit-1)/18);
      minval = rows*18 - row*18 + 1;
      subval = hit - minval;
      subval = round((subval*9)/18);
      if(subval == 0) subval = 1;
      printHit(start_col + col, start_row + row - 1, subval);
    }
  }
}

void printHit(byte col, byte row, byte subval){
  osd.openSingle(col, row);
if (osd_off == 0){
    switch (subval){
      case 1:
        osd.printf_P(PSTR("\x06"));
        break;
      case 2:
        osd.printf_P(PSTR("\x07"));
        break;
      case 3:
        osd.printf_P(PSTR("\x08"));
        break;
      case 4:
        osd.printf_P(PSTR("\x09"));
        break;
      case 5:
        osd.printf_P(PSTR("\x0a"));
        break;
      case 6:
        osd.printf_P(PSTR("\x0b"));
        break;
      case 7:
        osd.printf_P(PSTR("\x0c"));
        break;
      case 8:
        osd.printf_P(PSTR("\x0d"));
        break;
      case 9:
        osd.printf_P(PSTR("\x0e"));
        break;
    }
}
}

