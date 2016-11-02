import React, { Component } from 'react';
import { StyleSheet, Text, View } from 'react-native';
import { Actions } from 'react-native-router-flux';
import Header from '../common/Header';
import MapView from 'react-native-maps';

export default class HomePage extends Component {
  render() {
    return (
      <View style={styles.container}>
        <Header />
        <MapView
          ref={ref => { this.map = ref; }}
          initialRegion={{
            latitude: 21.067012,
            longitude: 105.8195148,
            latitudeDelta: 0.0285,
            longitudeDelta: 0.00204,
          }}
          style={styles.map}
        >
          <MapView.Marker
            coordinate={{
              latitude: 21.067012,
              longitude: 105.8195148
            }}
            title='Toong Coworking Space'
            description='Tap for more details'
            onCalloutPress={() => {Actions.record()}}
          />
        </MapView>
      </View>
    )
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1
  },
  map: {
    flex: 9
  }
});