#pragma once

const char PROGMEM HTML_DATA[] = R"====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
  <meta charset="UTF-8">
  <title>Scooter Controller</title>
  <style>
    html {
      font-family: Helvetica;
      display: inline-block;
      margin: 0px auto;
      text-align: center;
    }
    body {
      margin-top: 50px;
    }
    h1 {
      color: #444444;
      margin: 50px auto 30px;
    }
    h3 {
      color: #444444;
      margin-bottom: 50px;
    }
    .button {
      display: block;
      width: 80px;
      background-color: #f48100;
      border: none;
      color: white;
      padding: 13px 10px;
      text-decoration: none;
      font-size: 20px;
      margin: 0px auto 15px;
      cursor: pointer;
      border-radius: 4px;
    }
  </style>
</head>
<body>
  <button onclick="setSpeed(70, 'race')">Race to 70</button>
  <button onclick="setSpeed(70, 'sport')">Sport to 70</button>
  <button onclick="setSpeed(40, 'eco')">Eco to 40</button>

  <div>
    <label for="raceSlider">Race Speed:</label>
    <input type="range" id="raceSlider" min="0" max="70" value="22" onchange="updateSpeed('race', this.value)">
    <span id="raceValue">22</span>
  </div>

  <div>
    <label for="sportSlider">Sport Speed:</label>
    <input type="range" id="sportSlider" min="0" max="70" value="22" onchange="updateSpeed('sport', this.value)">
    <span id="sportValue">22</span>
  </div>

  <div>
    <label for="ecoSlider">Eco Speed:</label>
    <input type="range" id="ecoSlider" min="0" max="40" value="22" onchange="updateSpeed('eco', this.value)">
    <span id="ecoValue">22</span>
  </div>

  <h2>Speed limitation for all scooter modes upon startup</h2>

  <div>
    <label for="limitSlider">Limit:</label>
    <input type="range" id="limitSlider" min="0" max="70" value="0" onchange="updateSpeedLimit(this.value)">
    <span id="limitValue">0</span>
  </div>

  <script>
    function updateSpeed(mode, value) {
      document.getElementById(mode + 'Value').textContent = value;
      setSpeed(value, mode);
    }

    function setSpeed(value, mode) {
      fetch(`/set_speed?speed=${value}&mode=${mode}`)
        .then(response => response.text())
        .then(data => {
          console.log(data);
        })
        .catch(error => {
          console.error("Error setting speed:", error);
        });
    }
    
    function updateSpeedLimit(value) {
      document.getElementById('limitValue').textContent = value;
      setSpeedLimit(value);
    }
    
    function setSpeedLimit(value) {
      fetch(`/set_speed_limit?speedLimit=${value}`)
        .then(response => response.text())
        .then(data => {
          console.log(data);
        })
        .catch(error => {
          console.error("Error setting speed limit:", error);
        });
    }
  </script>
</body>
</html>
)====";
