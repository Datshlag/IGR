var div = document.getElementById("menu");
var message = document.getElementById("message");

var app = Sammy(function() {

	this.get('#/', function() {

		message.innerHTML = "";
		loadJSON("http://perso.telecom-paristech.fr/~concolat/cours/TP/igr203/mails-inbox.json");

	});

	this.get('#Inbox', function() {

		message.innerHTML = "Vous êtes dans : Inbox";
	});

  	this.get('#Sent', function() {

		message.innerHTML = "Vous êtes dans : Sent";
  	});

  	this.get('#Spam', function() {

		message.innerHTML = "Vous êtes dans : Spam";
  	});

  	this.get('#Archive', function() {

		message.innerHTML = "Vous êtes dans : Archive";
  	});

});

var loadJSON = function(url) {

    var xhr = new XMLHttpRequest();
    
    xhr.onload = function() {

		var status = xhr.status;
		if (status == 200) {

			data = xhr.response;
			console.log(data);
		} 
		else {

			//error occured
		}
    };

    xhr.open('GET', url, true);
    xhr.send();
};

app.run('#/');