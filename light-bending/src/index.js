import React from 'react';
import ReactDOM from 'react-dom';
import './index.css';
import reportWebVitals from './reportWebVitals';
import 'bootstrap/dist/css/bootstrap.min.css';
import {
  BrowserRouter as Router,
  Switch,
  Route
} from "react-router-dom";

import Tabs from './components/tabs';
import Sequences from './components/sequences';
import { initializeIcons } from '@fluentui/font-icons-mdl2';

initializeIcons();

ReactDOM.render(
  <React.StrictMode>
    <Tabs />
    <Router>
      <Switch>
        <Route path='/pallettes'>
        </Route>
        <Route path='/'>
          <Sequences/>
        </Route>
      </Switch>
    </Router>
  </React.StrictMode>,
  document.getElementById('root')
);

// If you want to start measuring performance in your app, pass a function
// to log results (for example: reportWebVitals(console.log))
// or send to an analytics endpoint. Learn more: https://bit.ly/CRA-vitals
reportWebVitals();
