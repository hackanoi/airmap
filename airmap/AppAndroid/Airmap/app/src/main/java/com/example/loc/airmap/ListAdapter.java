package com.example.loc.airmap;

import android.content.Context;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import java.util.List;

/**
 * Created by LOC on 10/26/2016.
 */


public class ListAdapter extends ArrayAdapter<Parameter> {

    public ListAdapter(Context context, int resource, List<Parameter> items) {
        super(context, resource, items);
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {

        View view = convertView;
        if (view == null) {
            LayoutInflater inflater = LayoutInflater.from(getContext());
            view =  inflater.inflate(R.layout.activity_list_parameter, null);
        }
        Parameter p = getItem(position);
        if (p != null) {
            // Anh xa + Gan gia tri
            TextView txt = (TextView) view.findViewById(R.id.textViewTime);
            txt.setText(p.Time);
            txt.setTextColor(Color.WHITE);
            TextView txt2 = (TextView) view.findViewById(R.id.textViewTemp);
            txt2.setText("Temp "+String.valueOf(p.Temp));
            txt2.setTextColor(Color.WHITE);
            TextView txt3 = (TextView) view.findViewById(R.id.textViewLight);
            txt3.setText("Light "+String.valueOf(p.Light));
            txt3.setTextColor(Color.WHITE);
            TextView txt4 = (TextView) view.findViewById(R.id.textViewHumi);
            txt4.setText("Humi "+String.valueOf(p.Humi));
            txt4.setTextColor(Color.WHITE);
            TextView txt5 = (TextView) view.findViewById(R.id.textViewCO2);
            txt5.setText("CO2 "+String.valueOf(p.CO2));
            txt5.setTextColor(Color.WHITE);

        }
        return view;
    }

}
