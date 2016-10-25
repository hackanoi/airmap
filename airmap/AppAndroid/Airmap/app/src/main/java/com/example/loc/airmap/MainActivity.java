package com.example.loc.airmap;

import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

//public class MainActivity extends AppCompatActivity implements ActionBarActivity {
public class MainActivity extends ActionBarActivity {
    TextView tv_Address, tv_Light, tv_Humi, tv_CO2, tv_trangthai, tv_Temperature, tv_CurentTemp, tv_HisTemp;
    TextView tv_Solight,tv_Sohumi, tv_Soco2;
    TextView tv_Dvlight, tv_Dvhumi,  tv_Dvco2;
    TextView tv_lastUpdate, tv_timelast;
    RelativeLayout mh;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tv_Address = (TextView) findViewById(R.id.textView_Address);
        tv_Temperature = (TextView) findViewById(R.id.textView_Temperature);
        tv_CurentTemp= (TextView) findViewById(R.id.textView_CurrentTemp);
        tv_HisTemp = (TextView) findViewById(R.id.textView_HisTemp);
        tv_Light =(TextView) findViewById(R.id.textView_Light);
        tv_Humi =(TextView) findViewById(R.id.textView_Humi);
        tv_CO2 =(TextView) findViewById(R.id.textView_C02);
        tv_trangthai =(TextView) findViewById(R.id.textView_trangthai);

        tv_Solight =(TextView) findViewById(R.id.textView_Solight);
        tv_Sohumi = (TextView) findViewById(R.id.textView_Sohumi);
        tv_Soco2 =(TextView) findViewById(R.id.textView_Soco2);

        tv_Dvlight =(TextView) findViewById(R.id.textView_Dvlight);
        tv_Dvhumi = (TextView) findViewById(R.id.textView_Dvhumi);
        tv_Dvco2 =(TextView) findViewById(R.id.textView_Dvco2);

        tv_lastUpdate=(TextView) findViewById(R.id.textView_lastUpdate);
        tv_timelast=(TextView) findViewById(R.id.textView_timelast);



        mh = (RelativeLayout) findViewById(R.id.activity_main_id);
        mh.setBackgroundResource(R.drawable.manhinhchinh);

        //Xoa thanh actionbar(thanh ghi tên project)
//        android.support.v7.app.ActionBar AB=getSupportActionBar();
//        AB.hide();

        //Set text dia diem
        tv_Address.setText("Hanoi");
        tv_Address.setTextColor(Color.WHITE);
        tv_Address.setTextSize(22);
        //Set text nhiet do
        tv_Temperature.setText("Temperature");
        tv_Temperature.setTextSize(25);
        tv_Temperature.setTextColor(Color.WHITE);
        //set text Light
        tv_Light.setText("Light");
        tv_Light.setTextColor(Color.WHITE);
        tv_Light.setTextSize(16);
        //set text Humi
        tv_Humi.setText("Humi");
        tv_Humi.setTextColor(Color.WHITE);
        tv_Humi.setTextSize(16);
        //set text CO2
        tv_CO2.setText("C02");
        tv_CO2.setTextColor(Color.WHITE);
        tv_CO2.setTextSize(16);
        //set text trang thai
        tv_trangthai.setText("Moderate");
        tv_trangthai.setTextColor(Color.WHITE);
        tv_trangthai.setTextSize(22);
        //Set text Temp
        tv_HisTemp.setText("TEMP");
        tv_HisTemp.setTextColor(Color.WHITE);
        tv_HisTemp.setTextSize(30);


        tv_CurentTemp.setText("30");
        tv_CurentTemp.setTextSize(150);
        tv_CurentTemp.setTextColor(Color.WHITE);

        tv_Solight.setText("30");
        tv_Solight.setTextSize(33);
        tv_Solight.setTextColor(Color.WHITE);

        tv_Sohumi.setText("30");
        tv_Sohumi.setTextSize(33);
        tv_Sohumi.setTextColor(Color.WHITE);

        tv_Soco2.setText("30");
        tv_Soco2.setTextSize(33);
        tv_Soco2.setTextColor(Color.WHITE);

        tv_Dvlight.setText("lx");
        tv_Dvlight.setTextSize(14);
        tv_Dvlight.setTextColor(Color.WHITE);

        tv_Dvhumi.setText("%");
        tv_Dvhumi.setTextSize(14);
        tv_Dvhumi.setTextColor(Color.WHITE);

        tv_Dvco2.setText("ppm");
        tv_Dvco2.setTextSize(14);
        tv_Dvco2.setTextColor(Color.WHITE);

        tv_lastUpdate.setText("Last update");
        tv_lastUpdate.setTextSize(15);
        tv_lastUpdate.setTextColor(Color.WHITE);

        tv_timelast.setText("9:00PM");
        tv_timelast.setTextSize(15);
        tv_timelast.setTextColor(Color.WHITE);


        tv_HisTemp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent mh2 = new Intent(MainActivity.this, ManHinh2.class);
                startActivity(mh2);
            }
        });


        Bundle bd = getIntent().getExtras();
        if(bd!=null){
            String temp = bd.getString("Temp");
            tv_CurentTemp.setText(temp);
            tv_CurentTemp.setTextSize(130);
            tv_CurentTemp.setTextColor(Color.WHITE);

            String light = bd.getString("Light");
            tv_Solight.setText(light);
            tv_Solight.setTextSize(33);
            tv_Solight.setTextColor(Color.WHITE);

            String humi = bd.getString("Humi");
            tv_Sohumi.setText(humi);
            tv_Sohumi.setTextSize(33);
            tv_Sohumi.setTextColor(Color.WHITE);

            String c02 = bd.getString("c02");
            tv_Soco2.setText(c02);
            tv_Soco2.setTextSize(33);
            tv_Soco2.setTextColor(Color.WHITE);

            String str2 = bd.getString("lasttime");
            String str3 = str2.substring(0,2);//lay gio
            String str4 = str2.substring(2,5);//lay phut
            Integer i = Integer.valueOf(str3);//ep kieu String sang Int để so sanh đe set AM or PM
            if(i>=12){
                tv_timelast.setText((i-12)+str4+" PM");}
            else
                tv_timelast.setText(str2+"AM");
            tv_timelast.setTextSize(15);
            tv_timelast.setTextColor(Color.WHITE);
        }
    }
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }


    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
//Lấy ra id của item vừa click
        int id = item.getItemId();
//Xử lý khi click vào sẽ show ra title của item đó
        if (id == R.id.item_update) {
            Toast.makeText(getApplicationContext(),"Update clicked", Toast.LENGTH_SHORT).show();
        }
        return super.onOptionsItemSelected(item);
    }
}
