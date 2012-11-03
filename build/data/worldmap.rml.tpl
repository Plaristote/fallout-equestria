<rml>
  <head>
    <title>World Map</title>
    <link type="text/template" href="tpl_mainbar.rml" />
    <style>

#main
{
  background-color: black;
}

#map-frame
{
  position:   absolute;
  top:        0px;
  left:       0px;
  width:      80%;
  height:     80%;
  overflow: scroll;
}

#clock-frame
{
  position: absolute;
  top:      0px;
  right:    0px;
  width:    20%;
  height:   10%;
  padding-top:  15px;
  padding-left: 15px;
}

#city-list
{
  position: absolute;
  top:      10%;
  right:    0px;
  width:    20%;
  height:   70%;
}

#main-bar
{
  position: absolute;
  bottom: 0px;
  left:   0px;
  width:  100%;
  height: 20%;
}

#party-cursor
{
  position: absolute;
  top:      0px;
  left:     0px;
  height:   10px;
  width:    10px;
  background-color: red;
  z-index:  998;
}

.city-name
{
  margin-left:    5px;
  padding-left:   5px;
  padding-top:    20px;
  background-color: blue;
  color: white;
  text-align: center;
  font-size: 20px;
}

#{RCSS}

    </style>
  </head>


  <body id="body">

  <div id="main" style="width:100%; height:100%;">

    <div id="map-frame">
      <div id="party-cursor">-</div>
      <p id="pworldmap">
        #{RML}
      </p>
    </div>

    <div id="clock-frame" class="window-background">
      <span class="console-value" id="clock-year">0000</span> <span class="console-value" id="clock-month">01</span> <span id="clock-day" class="console-value">01</span>
    </div>

    <div id="city-list" class="window-background">
      City list
    </div>

    <div id="main-bar" class="window-background">
      <button id="button-inventory" i18n="Inventory">Inventory</button>
      <button id="button-character" i18n="Character">Character</button>
      <button id="button-pipbuck"   i18n="Pipbuck">Pipbuck</button>
      <button id="button-menu"      i18n="Menu">Menu</button>
    </div>

  </div>

  </body>
</rml>
