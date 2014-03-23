<rml>
  <head>
    <title>World Map</title>
    <link type="text/template" href="tpl_mainbar.rml" />
    <style>

#main
{
  background-color: black;
  width: 100%;
  height: 80%;
}

#map-frame, #city-splash
{
  width:      80%;
  height:     100%;
  overflow:   auto;
  float:      left;
}

#city-splash
{
  display: none;
}

#map-side{
  width: 20%;
  height: 100%;
  float: right;
}

#map-side .block-first
{
  width:    100%;
  height:   20%;
  padding-bottom: 20%;
}

#map-side .block-second
{
  width:    100%;
  height:   80%;
}

#clock-frame
{
  padding-top: 25%;
  margin: auto;
  vertical-align: middle;
  padding-left: 5%;
}

#clock-frame .console-value
{
  padding: 5px 5px 5px 5px;
  float: left;
  width: 25%;
  text-align: center;
  vertical-align: middle;
}

#city-container, #entry-zone-container
{
  padding-top: 100px;
  margin: auto;
}

#entry-zone-container
{
  display: none;
}

#city-list, #entry-zones
{
  text-align: center;
}

#main-bar
{
  width:  100%;
  height: 20%;
}

#main-bar button
{
  max-width: 150px;
}

#party-cursor
{
  position: absolute;
  top:      0px;
  left:     0px;
  height:   12px;
  width:    24px;
  z-index:  998;
  background-decorator: image;
  background-image-src: worldmap-cursor.png;
}

#party-cursor:hover
{
  background-image-src: worldmap-cursor-hover.png;
}

#party-cursor:active
{
  background-image-src: worldmap-cursor-active.png;
}

.city-entry, .zone-entry
{
  width: 90%;
  margin-left: 5%;
  min-height: 50px;
}

.city-button, .zone-button
{
  width: 100%;
  height: 100%;
  padding: 10px 10px 10px 10px;
}

.universal_button{
	width: 20%;
	padding-left: 4%;
}
#quest-locator{
	width: 80%;
	height: 65%;
	margin: 10%;

	
}

#rest{
  float: left;
	height: 20%;
	width: 100%;
}
#{RCSS}

    </style>
  </head>


  <body id="body">
    
    <div id="main" class="place-beneath">
      <div id="map-frame">
        <div id="party-cursor"></div>
        <p id="pworldmap">
          #{RML}
        </p>
      </div>
      
      <div id="city-splash">
        <img id="city-splash-image" style="width:100%;height:100%;" />
      </div>
      
      <div id="map-side">
        <div class="block-first window-background">
          <div id="clock-frame">
            <div class="console-value" id="clock-year">0000</div> <div class="console-value" id="clock-month">01</div> <div id="clock-day" class="console-value">01</div>
          </div>
        </div>
        
        <div class="block-second window-background">
          <div id="city-container">
            <div id="city-list">
            </div>
          </div>
          
          <div id="entry-zone-container">
            <div id="entry-zones">
            </div>
              <button class="universal_button" id="button-cancel" style="position:relative;bottom:20px;width:80%;margin:auto;">
                Back to map
              </button>
          </div>
        </div>
      </div>
    </div>

    <div id="rest" class="window-background">
      <div id="main-bar" class="   place-beneath">
        <button id="button-inventory" class="universal_button" i18n="Inventory">Inventory</button>
        <button id="button-character" class="universal_button" i18n="Character">Character</button>
        <button id="button-pipbuck"  class="universal_button" i18n="Pipbuck">Pipbuck</button>
        <button id="button-menu"     class="universal_button" i18n="Menu">Menu</button>
      </div>

      <div id="quest-locator" class="console-bg   place-beneath">
        Placeholder for quest tracer
      </div>
    </div>

  </body>
</rml>
