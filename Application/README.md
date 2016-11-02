## Introduction

Airmap Android app using React Native.

## Instruction

1. To use google map api with `react-native-maps`, create `keys.xml` under `android/app/src/main/res/values` with the following content:
   ```xml 
   <resources>
       <string name="map_key">your-google-key</string>
   </resources>
   ```

2. Specific your key in `AndroidManifest.xml`:
   ```xml
   <meta-data
       android:name="com.google.android.geo.API_KEY"
       android:value="@string/map_key" />
   ```
   
3. Copy `src/config.example.js` to `src/config.js` and update `API_ENDPOINT` value.