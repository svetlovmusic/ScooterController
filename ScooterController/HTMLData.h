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
    h2 {
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
   <div>
    <h2>Select Scooter Model</h2>
    <input type="radio" id="gtModel" name="scooterModel" value="0" checked onchange="updateScooterModel(this.value)">
    <label for="gtModel">GT version</label>
    <input type="radio" id="pModel" name="scooterModel" value="1" onchange="updateScooterModel(this.value)">
    <label for="pModel">P version</label>
  </div>
  
  <div id="gtInterface">
    <h2>GT Scooter Controls</h2>
    <p>
      <button onclick="setSpeed('gt', 70, 'race')">Race to 70</button>
      <button onclick="setSpeed('gt', 70, 'sport')">Sport to 70</button>
      <button onclick="setSpeed('gt', 40, 'eco')">Eco to 40</button>
    </p>
    <p>
      <div>
        <label for="gtRaceSlider">Race Max Speed:</label>
        <input type="range" id="gtRaceSlider" min="0" max="70" value="{{SPEED_LIMIT}}" onchange="updateSpeed('gt', 'race', this.value)">
        <span id="gtRaceValue">{{SPEED_LIMIT}}</span>
      </div>
      <div>
        <label for="gtSportSlider">Sport Max Speed:</label>
        <input type="range" id="gtSportSlider" min="0" max="70" value="{{SPEED_LIMIT}}" onchange="updateSpeed('gt', 'sport', this.value)">
        <span id="gtSportValue">{{SPEED_LIMIT}}</span>
      </div>
      <div>
        <label for="gtEcoSlider">Eco Max Speed:</label>
        <input type="range" id="gtEcoSlider" min="0" max="70" value="{{SPEED_LIMIT}}" onchange="updateSpeed('gt', 'eco', this.value)">
        <span id="gtEcoValue">{{SPEED_LIMIT}}</span>
      </div>
    </p>
  </div>

  <div id="pInterface" style="display:none;">
    <h2>P Scooter Controls</h2>
    <p>
      <button onclick="setSpeed('p', 50, 'sport')">Sport to 40</button>
      <button onclick="setSpeed('p', 50, 'drive')">Drive to 40</button>
      <button onclick="setSpeed('p', 30, 'eco')">Eco to 40</button>
    </p>
    <p>
      <div>
        <label for="pSportSlider">Sport Max Speed:</label>
        <input type="range" id="pSportSlider" min="0" max="40" value="{{SPEED_LIMIT}}" onchange="updateSpeed('p', 'sport', this.value)">
        <span id="pRaceValue">{{SPEED_LIMIT}}</span>
      </div>
      <div>
        <label for="pDriveSlider">Drive Max Speed:</label>
        <input type="range" id="pDriveSlider" min="0" max="40" value="{{SPEED_LIMIT}}" onchange="updateSpeed('p', 'drive', this.value)">
        <span id="pSportValue">{{SPEED_LIMIT}}</span>
      </div>
      <div>
        <label for="pEcoSlider">Eco Max Speed:</label>
        <input type="range" id="pEcoSlider" min="0" max="40" value="{{SPEED_LIMIT}}" onchange="updateSpeed('p', eco', this.value)">
        <span id="pEcoValue">{{SPEED_LIMIT}}</span>
      </div>
    </p>
  </div>



  <div>
    <h2>Limit for all modes upon startup</h2>
    <p>
      <div>
        <label for="limitSlider">Speed Limit:</label>
        <input type="input" id="limitSlider" min="0" max="100" value="{{SPEED_LIMIT}}" onchange="updateSpeedLimit(this.value)">
        <span id="limitValue">km/h</span>
      </div>
    </p>
  </div>

  <script>
    var defaultSpeedLimit = {{SPEED_LIMIT}};
    var defaultSeriesId = {{SERIES_ID}};
    
    document.addEventListener("DOMContentLoaded", function() {
      initializeScooterModel();
    });
    
    function updateSpeed(model, mode, value) {
      document.getElementById(model + mode + 'Value').textContent = value;
      setSpeedGT(model, value, mode);
    }

    function setSpeed(model, value, mode) {
      fetch(`/set_speed?speed=${value}&mode=${mode}&model=${model}`)
        .then(response => response.text())
        .then(data => {
          console.log(data);
        })
        .catch(error => {
          console.error("Error setting speed:", error);
        });
    }

    function updateScooterModel(model) {
      if (model === "0") {
        document.getElementById("gtInterface").style.display = "block";
        document.getElementById("pInterface").style.display = "none";
      } else if (model === "1") {
        document.getElementById("gtInterface").style.display = "none";
        document.getElementById("pInterface").style.display = "block";
      }

      fetch(`/set_scooter_model?model=${model}`)
        .then(response => response.text())
        .then(data => {
          console.log(data);
        })
        .catch(error => {
          console.error("Error setting scooter model:", error);
        });
    }

    function updateSpeedLimit(value) {
      document.getElementById('limitValue').textContent = value + " km/h";
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
    
    function initializeScooterModel() {
      var seriesId = defaultSeriesId;
    
      if (seriesId === 0) {
        document.getElementById("gtModel").checked = true;
        updateScooterModel("0");
      } else if (seriesId === 1) {
        document.getElementById("pModel").checked = true;
        updateScooterModel("1");
      }
    }

  </script>
</body>
</html>
)====";
