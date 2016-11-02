import React, { Component } from 'react';
import { Scene, Router, ActionConst } from 'react-native-router-flux';
import HomePage from './components/home/HomePage';
import RecordPage from './components/record/RecordPage';
import LoadingPage from './components/loading/LoadingPage';

export default class App extends Component {
  render() {
    return (
      <Router>
        <Scene key="root" hideNavBar={true}>
          <Scene key="loading" component={LoadingPage} initial={true} />
          <Scene key="home" component={HomePage} type={ActionConst.REPLACE} />
          <Scene key="record" component={RecordPage} />
        </Scene>
      </Router>
    );
  }
}