var div = document.getElementById("menu");
var message = document.getElementById("message");

var app = Sammy(function() {

	this.get('#/', function() {

		message.innerHTML = "";
		loadJSON("http://perso.telecom-paristech.fr/~concolat/cours/TP/igr203/mails-inbox.json");
	});

	this.get('#Inbox', function() {

		message.innerHTML = "Vous êtes dans : Inbox";
		loadJSON("http://perso.telecom-paristech.fr/~concolat/cours/TP/igr203/mails-inbox.json");
	});

  	this.get('#Sent', function() {

		message.innerHTML = "Vous êtes dans : Sent";
		loadJSON("http://perso.telecom-paristech.fr/~concolat/cours/TP/igr203/mails-sent.json");
  	});

  	this.get('#Spam', function() {

		message.innerHTML = "Vous êtes dans : Spam";
		loadJSON("http://perso.telecom-paristech.fr/~concolat/cours/TP/igr203/mails-spam.json");
  	});

  	this.get('#Archive', function() {

		message.innerHTML = "Vous êtes dans : Archive";
		loadJSON("http://perso.telecom-paristech.fr/~concolat/cours/TP/igr203/mails-archives.json");
  	});

});


var loadJSON = function(url) {

    var xhr = new XMLHttpRequest();

    xhr.onload = function() {

		var status = xhr.status;
		if (status == 200) {

            $.get("exercice2b.template", function(template) {

    			data = JSON.parse(xhr.response);
                var html = Mustache.to_html(template, data);
                document.getElementById("mails").innerHTML = html;
            });
		}
		else {

        }
    };

    xhr.open('GET', url, true);
    xhr.send();
};  

app.run('#/');
