var div = document.getElementById("menu");
var message = document.getElementById("message");

var app = Sammy(function() {

	this.get('#/', function() {

		message.innerHTML = "";
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

app.run('#/');