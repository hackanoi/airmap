import React from 'react';
import { StatusBar, StyleSheet, Text, View } from 'react-native';

export default ({title, statusColor}) => (
  <View style={styles.container}>
    <StatusBar backgroundColor={statusColor || '#0092dd'} />
    <Text style={styles.title}>{title || 'Airmap'}</Text>
  </View>
)

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#009bdf',
    alignItems: 'flex-start',
    justifyContent: 'center'
  },
  title: {
    color: '#ffffff',
    marginLeft: 15
  },
})