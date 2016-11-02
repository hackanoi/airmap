import React, { Component } from 'react';
import { StyleSheet, Text, TouchableOpacity , View } from 'react-native';
import Header from '../common/Header';
import { API_ENDPOINT } from '../../config';
import Chart from 'react-native-chart';

function convertTime(timestamp) {
  let now = new Date(timestamp);

  return now.toLocaleTimeString();
}

export default class RecordPage extends Component {
  constructor(props) {
    super(props);
    this._fetch = this._fetch.bind(this);
    this._refresh = this._refresh.bind(this);
    this.state = {
      data: {},
      chart: []
    }
  }

  _fetch() {
    fetch(API_ENDPOINT)
      .then(response => {
        if (! response.ok) {
          throw Error(response.statusText);
        }
        return response.json();
      })
      .then(response => {
        if (response.status == 'success') {
          let chartData = [];
          for (let i = 0; i < 10; i++) {
            chartData.push([
              convertTime(response.data[i].timestamp),
              response.data[i].aqi
            ]);
          }
          this.setState({chart: chartData});
          this.setState({data: response.data });
        }
      })
      .catch(error => {
        console.log(error);
        alert(error.message || 'Error when fetching data from server');
      });
  }

  _refresh() {
    this._fetch();
  }

  componentDidMount() {
    this._fetch();
  }

  render() {
    if (this.state.data.length)
      return (
        <View style={styles.container}>
          <Header />
          <View style={styles.body}>
            <View style={styles.index}>
              <TouchableOpacity onPress={this._refresh}>
                <Text
                  style={text.aqi}
                >
                  {this.state.data[0].aqi} <Text style={text.small}>AQI</Text>
                </Text>
              </TouchableOpacity>
              <Text style={text.time}>Last Update: {convertTime(this.state.data[0].timestamp)}</Text>
            </View>
            <View style={styles.chart}>
              <Chart
                style={styles.lineChart}
                data={this.state.chart}
                type="line"
                showDataPoint={true}
                showGrid={true}
                gridColor="#e1e1e1"
                axisColor="#e1e1e1"
                axisLabelColor="#606060"
             />
            </View>
            <View style={styles.detail}>
              <View style={styles.subDetail}>
                <Text style={[text.general, text.type]}>CO</Text>
                <Text><Text style={[text.general, text.value]}>{this.state.data[0].co}</Text> ppm</Text>
              </View>
              <View style={styles.subDetailCenter}>
                <Text style={[text.general, text.type]}>SO2</Text>
                <Text><Text style={[text.general, text.value]}>{this.state.data[0].so2}</Text> ppm</Text>
              </View>
              <View style={styles.subDetail}>
                <Text style={[text.general, text.type]}>PM2.5</Text>
                <Text><Text style={[text.general, text.value]}>{this.state.data[0].pm25}</Text> mg/m3</Text>
              </View>
            </View>
          </View>
        </View>
      );
    return (
      <View style={styles.container}>
        <Header />
        <View style={styles.body}>
          <View style={styles.index}>
            <TouchableOpacity onPress={this._refresh}>
              <Text
                style={text.aqi}
              >
                0 <Text style={text.small}>AQI</Text>
              </Text>
            </TouchableOpacity>
            <Text style={text.time}>Last Update: 9.00 am</Text>
          </View>
          <View style={styles.chart}>

          </View>
          <View style={styles.detail}>
            <View style={styles.subDetail}>
              <Text style={[text.general, text.type]}>CO</Text>
              <Text><Text style={[text.general, text.value]}>0</Text> ppm</Text>
            </View>
            <View style={styles.subDetailCenter}>
              <Text style={[text.general, text.type]}>SO2</Text>
              <Text><Text style={[text.general, text.value]}>0</Text> ppm</Text>
            </View>
            <View style={styles.subDetail}>
              <Text style={[text.general, text.type]}>PM2.5</Text>
              <Text><Text style={[text.general, text.value]}>0</Text> mg/m3</Text>
            </View>
          </View>
        </View>
      </View>
    );
  }
}
const text = StyleSheet.create({
  general: {
    color: '#656565'
  },
  type: {
    fontSize: 15,
  },
  value: {
    fontSize: 20
  },
  aqi: {
    color: '#ffffff',
    fontSize: 60
  },
  small: {
    color: '#ffffff',
    fontSize: 30
  },
  time: {
    color: '#ffffff',
    fontSize: 15,
    marginTop: 30
  }
});

const styles = StyleSheet.create({
  container: {
    flex: 1
  },
  body: {
    flex: 9,
  },
  index: {
    flex: 2,
    backgroundColor: '#009bdf',
    alignItems: 'center',
  },
  chart: {
    flex: 2,
    backgroundColor: '#f4f4f4',
    borderBottomWidth: 1,
    borderColor: '#e1e1e1',
    alignItems: 'center',
    justifyContent: 'center',
    padding: 10
  },
  lineChart: {
    width: 300,
    height: 100,
  },
  detail: {
    flex: 1,
    flexDirection: 'row',
    backgroundColor: '#f4f4f4'
  },
  subDetail: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center'
  },
  subDetailCenter: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
    borderColor: '#e1e1e1',
    borderLeftWidth: 1,
    borderRightWidth: 1
  }
})