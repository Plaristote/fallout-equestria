<rml>
  <head>
    <title>World Map</title>
    <link type="text/template" href="tpl_window.rml" />
    <style>

#main
{
  background-color: black;
  width: 100%;
  height: 80%;
}

#map-frame
{
  width:      60%;
  height:     100%;
  overflow: auto;
}

#clock-frame
{
  width:    100%;
  height:   20%;
  
}

#city-list
{
  width:    100%;
  height:   80%;
}

#main-bar
{
  width:  100%;
  height: 20%;
}

#party-cursor
{
  position: absolute;
  top: 0px;
  left: 0px;
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
.something_button{
	height: 20%;
}
#quest-locator{
	width: 80%;
	height: 65%;
	margin: 10%;

	
}
#map-side{
	width: 32%;
	height: 100%;
}
#rest{
	height: 20%;
	width: 100%;
}
#{RCSS}

    </style>
  </head>


  <body id="body">
    
	  <div id="main" class="char-place-beneath">
	
	    <div id="map-frame" class="char-place-next-to">
	      <div id="party-cursor">-</div>
	      <p id="pworldmap">
	        #{RML}
	      </p>
	    </div>
		 <div id="map-side" class="char-place-next-to">
		    <div id="clock-frame" class="window-background place-beneath">
		      <span class="console-value" id="clock-year">0000</span> <span class="console-value" id="clock-month">01</span> <span id="clock-day" class="console-value">01</span>
		    </div>
		
		    <div id="city-list" class="window-background place-beneath">
		      City list
		    </div>
		  </div>
	  </div>
	  <div id="rest" class="char-place-beneath window-background">
	    <div id="main-bar" class=" char-place-beneath">
	      <button id="button-inventory" class="universal_button" i18n="Inventory">Inventory</button>
	      <button id="button-character" class="universal_button" i18n="Character">Character</button>
	      <button id="button-pipbuck"  class="universal_button" i18n="Pipbuck">Pipbuck</button>
	      <button id="button-menu"     class="universal_button" i18n="Menu">Menu</button>
	    </div>
		 <div id="quest-locator" class="console-bg char-place-beneath">
				 Placeholder for quest tracer
		 </div>
	  </div>

  </body>
</rml>
