package com.example.loc.airmap;

/**
 * Created by LOC on 10/26/2016.
 */

public class Parameter {
        public String Time;
        public String Temp;
        public String Light;
        public String Humi;
        public String CO2;
// phai theo thu tu trong activity_list_parameter
    public Parameter(String time, String temp, String humi, String light, String CO2) {
        Time = time;
        Temp = temp;
        Humi = humi;
        Light = light;
        this.CO2 = CO2;
    }
}
