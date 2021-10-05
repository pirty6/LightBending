var express = require('express');
var path = require('path');
/* var cookieParser = require('cookie-parser'); */
/* var logger = require('morgan'); */
const fs = require('fs');

var app = express();
const port = 5000;

app.use(express.json());
app.use(express.urlencoded());

// view engine setup
app.get('/json/sequences', (req, res) => {
    res.header("Content-Type",'application/json');
    res.sendFile(path.join(__dirname, '/json/sequences.json'));
});
app.post('/json/sequences', (req, res) => {
  try {
    const file = path.join(__dirname, '/json/sequences.json');
    console.log(req.body);
    // read file and make object
    let content = JSON.parse(fs.readFileSync(file, 'utf8'));
    // edit or add property
    content.expiry_date = 999999999999;
    //write file
    fs.writeFileSync(file, JSON.stringify(req.body));
    res.sendStatus(200);
  } catch(e) {
    console.log(e);
  }
});
app.get('/json/pallettes', (req, res) => {
    res.header("Content-Type",'application/json');
    res.sendFile(path.join(__dirname, '/json/pallettes.json'));
});
app.post('/json/pallettes', (req, res) => {
  try {
    const file = path.join(__dirname, '/json/pallettes.json');
    console.log(req.body);
    // read file and make object
    let content = JSON.parse(fs.readFileSync(file, 'utf8'));
    // edit or add property
    content.expiry_date = 999999999999;
    //write file
    fs.writeFileSync(file, JSON.stringify(req.body));
    res.sendStatus(200);
  } catch(e) {
    console.log(e);
  }
});

app.listen(port, () => {
    console.log(`Example app listening at http://localhost:${port}`)
  })