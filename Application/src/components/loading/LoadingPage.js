import React, { Component } from 'react';
import { ActivityIndicator, Image, StyleSheet, Text, View } from 'react-native';
import { Actions } from 'react-native-router-flux';

export default class LoadingPage extends Component {
  componentDidMount() {
    setTimeout(() => {
      Actions.home()
    }, 2500);
  }

  render() {
    return (
      <View style={styles.container}>
        <Image
          source={require('./logo.png')}
          style={styles.image}
          resizeMode="cover"
          resizeMethod="scale"
        />
        <Text style={styles.text}>Airmap</Text>
        <ActivityIndicator animating={true} color='#656565' />
      </View>
    )
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center'
  },
  image: {
    width: 80,
    height: 80
  },
  text: {
    color: '#656565',
    fontSize: 40
  }
});