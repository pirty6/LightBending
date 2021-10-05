import React, { useState, useEffect } from 'react';

const pad = [2, 2];
const url = '/json/pallettes';

const Pallettes = () => {
    const [padConfiguration, setPadConfiguration] = useState({
        padConfiguration1: [...Array(pad[0])].map((x => [...Array(pad[1])])),
             padConfiguration2: [...Array(pad[0])].map((x => [...Array(pad[1])]))
    });







    useEffect(() => {
        fetchFile();
    }, []);

    const fetchFile = () => {
        fetch(url)
          .then(function (response) {
            if (response.status !== 200) {
              throw response.status;
            }
            return response.text();
          })
          .then(function (fileContent) {
            setPadConfiguration(JSON.parse(fileContent));
          })
          .catch(function (status) {
            console.log("Error " + status);
          });
      };
}