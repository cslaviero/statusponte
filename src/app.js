var UI = require('ui');
var ajax = require('ajax');
var Vibe = require('ui/vibe');
var Vector2 = require('vector2'); 

var window = new UI.Window({backgroundColor: 'white'});

var imgNit = new UI.Image({ position:new Vector2(8,21),
													size: new Vector2(55,22),
													image:'images/nit2.png'});

var textNit = new UI.Text({ 
															position: new Vector2(18, 44), 
															size: new Vector2(40, 21), 
															font: 'gothic-14',
	color: 'black',
															text:"Niteroi"
});

var imgRj = new UI.Image({position:new Vector2(74,98),size: new Vector2(55,26), image:'images/rj2.png'});
var textRj = new UI.Text({	color: 'black',
															position: new Vector2(78, 127), 
															size: new Vector2(63, 21),
															font: 'gothic-14',	
															text:"R. Janeiro"
});
	
var textTempoNit = new UI.Text({color: 'black',
															position: new Vector2(76,18), 
															size: new Vector2(58, 26),
															font: 'gothic-24-bold',
															text: '?? Min.'
});
var textTempoRj = new UI.Text({	color: 'black',
															position: new Vector2(10, 94), 
															size: new Vector2(58, 26),
															font: 'gothic-24-bold',
															text: '?? Min.'
});
var textUpdate = new UI.Text({	color: 'black',
															position: new Vector2(15, 65), 
															size: new Vector2(118, 22),
															font: 'gothic-18',
															text: '??:??'
});



var imgClock = new UI.Image({position:new Vector2(36,71),size: new Vector2(16,16), image:'images/clock2.png'});

	
window.add(textNit);
window.add(textRj);
window.add(textTempoNit);
window.add(textTempoRj);
window.add(textUpdate);
window.add(imgNit);
window.add(imgRj);
window.add(imgClock);

window.show();	

var textFetch = new UI.Text({	color: 'white',
															position: new Vector2(15, 65), 
															size: new Vector2(118, 22),
															font: 'gothic-18-bold',	
															text: "Fetching...",
															textAlign: 'center',
															backgroundColor:'black'
														});

var url2 = "http://www.ecoponte.com.br/Xml/TempoViagem.xml";

function getDataFromImporIO(data){
		// Success!
    console.log("Successfully fetched bridge data!");
	
	
		var timeNit = data.split(" ")[6].split('"')[1];
		var timeRj = data.split(" ")[8].split('"')[1];
	
	console.log("Tempo RJ:"+timeRj);
	console.log("Tempo NIT: "+timeNit);
	
	// update times and hour
	var time = data.split('"')[5].split(" ")[1];
	
	textTempoNit.text(timeNit+" Min.");
	textTempoRj.text(timeRj+" Min.");
	textUpdate.text(time);
	console.log("time"); 
	
		Vibe.vibrate('short');
		window.remove(textFetch);
	
}

function update()
{
	window.add(textFetch);

	ajax(
  {
    url: url2,
    type: 'xml',
	},getDataFromImporIO,
  function(error) {
    console.log('Failed fetching bridge data: ' + error);
  }
);
	
}

update();

window.on('click', 'select', function() {
	update();
});


