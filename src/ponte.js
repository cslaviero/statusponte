
var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function locationSuccess(pos) {

  // Send request to OpenWeatherMap
		var url2 = "http://www.ecoponte.com.br/Xml/TempoViagem.xml";
  xhrRequest(url2, 'GET', 
   function(responseText) {
      // responseText contains a JSON object with weather info

			var data = responseText;
			
			var timeNit = data.split(" ")[6].split('"')[1];
			var timeRj = data.split(" ")[8].split('"')[1];

			console.log("Tempo RJ:"+timeRj);
			console.log("Tempo NIT: "+timeNit);

			// update times and hour
			var time = data.split('"')[5].split(" ")[1];
			console.log("Tempo: "+time);
				
			// Assemble dictionary using our keys
			var dictionary = {
				'KEY_NIT': timeNit,
				'KEY_RJ': timeRj,
				'KEY_AGUARDE':time
			};
			
			// Send to Pebble
			Pebble.sendAppMessage(dictionary,
				function(e) {
					console.log('Weather info sent to Pebble successfully!');
				},
				function(e) {
					console.log('Error sending weather info to Pebble!: ');
					console.log(e.error);
				}
			);
    }   
  );

}

function locationError(err) {
  console.log('Error requesting location!');
}

function getWeather() {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
		getWeather();
  }                     
);


// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log('PebbleKit JS ready!');

    // Get the initial weather
    getWeather();
  }
);




