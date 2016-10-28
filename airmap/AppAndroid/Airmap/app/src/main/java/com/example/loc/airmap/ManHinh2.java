package com.example.loc.airmap;

import android.content.Intent;
import android.graphics.Color;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.ListView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.github.mikephil.charting.charts.BarChart;
import com.github.mikephil.charting.data.BarData;
import com.github.mikephil.charting.data.BarDataSet;
import com.github.mikephil.charting.data.BarEntry;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;

public class ManHinh2  extends AppCompatActivity {
    ListView lvThongSo;
    String  tem, light, humi, co2, str2, str1, lasttime;
    RelativeLayout mhp;
    TextView tv_Back;

    BarChart barChart;
    ArrayList<BarEntry> barEntries = new ArrayList<>();
    ArrayList<String> theDates = new ArrayList<>();
    BarDataSet barDataSet = new BarDataSet(barEntries,"Humidity(%)");



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_man_hinh2);

        barChart = (BarChart) findViewById(R.id.bargraph);
        //Hien thi ListThongso: Thoigian, Nhietdo

        lvThongSo = (ListView) findViewById(R.id.listview_ThongSo);
        mhp =  (RelativeLayout) findViewById(R.id.activity_manhinh3) ;
        mhp.setBackgroundResource(R.drawable.water4);

        //Xoa thanh actionbar(thanh ghi tên project)
//        android.support.v7.app.ActionBar AB=getSupportActionBar();
//        AB.hide();


        //Xu ly su kien khi bam TextView Back
        tv_Back = (TextView) findViewById(R.id.textView_Back);
        tv_Back.setText("BACK");
        tv_Back.setTextSize(30);
        tv_Back.setTextColor(Color.WHITE);
        tv_Back.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent mh1 = new Intent(ManHinh2.this, MainActivity.class);
                mh1.putExtra("Temp", tem);
                mh1.putExtra("Light", light);
                mh1.putExtra("Humi", humi);
                mh1.putExtra("c02", co2);
                mh1.putExtra("lasttime", str2);

                startActivity(mh1);

            }
        });

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                new ReadJSON().execute("http://lekienhust94.mybluemix.net/jsonOutput.php");
            }
        });
    }


    //Tao class Read JSON
    class ReadJSON extends AsyncTask<String, Integer, String > {

        @Override
        protected String doInBackground(String... params) {
            String chuoi = docNoiDung_Tu_URL(params[0]);
            return chuoi;
        }

        @Override
        protected void onPostExecute(String s) {
            JSONArray mangJSON;
            ArrayList<Parameter> mangThongSo = new ArrayList<Parameter>();

            try {
                mangJSON = new JSONArray(s);
                for (int i = mangJSON.length()-1; i >= 0; i--){
                    mangJSON.getJSONObject(i);
                    JSONObject time = mangJSON.getJSONObject(i);
                    mangThongSo.add(new Parameter(
                            time.getString("time"),
                            time.getString("tem"),
                            time.getString("humi"),
                            time.getString("light"),
                            time.getString("c02")));

                    //Add humi vao chart co 7 gia tri
                    if(i==(mangJSON.length()- 1)){
                        barEntries.add(new BarEntry(Float.valueOf(time.getString("humi")),0));}
                    if(i==(mangJSON.length()- 2)){
                        barEntries.add(new BarEntry(Float.valueOf(time.getString("humi")),1));}
                    if(i==(mangJSON.length()- 3)){
                        barEntries.add(new BarEntry(Float.valueOf(time.getString("humi")),2));}
                    if(i==(mangJSON.length()- 4)){
                        barEntries.add(new BarEntry(Float.valueOf(time.getString("humi")),3));}
                    if(i==(mangJSON.length()- 5)){
                        barEntries.add(new BarEntry(Float.valueOf(time.getString("humi")),4));}
                    if(i==(mangJSON.length()- 6)){
                        barEntries.add(new BarEntry(Float.valueOf(time.getString("humi")),5));}
                    if(i==(mangJSON.length()- 7)){
                        barEntries.add(new BarEntry(Float.valueOf(time.getString("humi")),6));}

                    if (i == (mangJSON.length()- 1)) {
                        tem = time.getString("tem");
                        lasttime = time.getString("time");
                        //ham lay kis tu thu 11 den ki tu thu 16 trong chuoi time :)
                        str1 = new String(lasttime);
                        str2 = str1.substring(11,16);
                        light = time.getString("light");
                        humi = time.getString("humi");
                        co2 = time.getString("c02");
                    }

                }

                //Do data vao chart
                theDates.add("Mon");
                theDates.add("Tue");
                theDates.add("Web");
                theDates.add("Thur");
                theDates.add("Fri");
                theDates.add("Sar");
                theDates.add("Sun");


                BarData theData = new BarData(theDates,barDataSet);
                barChart.setData(theData);
                barChart.setTouchEnabled(true);
                barChart.setDragEnabled(true);
                barChart.setScaleEnabled(true);
                barChart.setBackgroundColor(Color.YELLOW);

                ListAdapter adapter = new ListAdapter(
                        ManHinh2.this,
                        R.layout.activity_list_parameter,
                        mangThongSo
                );
                lvThongSo.setAdapter(adapter);
            } catch (JSONException e) {
                e.printStackTrace();
            }
        }
    }

    //Doc noi dung  tu Internet
    private static String docNoiDung_Tu_URL(String theUrl)
    {
        StringBuilder content = new StringBuilder();

        try
        {
            // create a url object
            URL url = new URL(theUrl);

            // create a urlconnection object
            URLConnection urlConnection = url.openConnection();

            // wrap the urlconnection in a bufferedreader
            BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(urlConnection.getInputStream()));

            String line;

            // read from the urlconnection via the bufferedreader
            while ((line = bufferedReader.readLine()) != null)
            {
                content.append(line + "\n");
            }
            bufferedReader.close();
        }
        catch(Exception e)
        {
            e.printStackTrace();
        }
        return content.toString();
    }
}
