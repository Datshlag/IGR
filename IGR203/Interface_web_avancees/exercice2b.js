var div = document.getElementById("menu");
var inbox = document.getElementById("inbox");
var sent = document.getElementById("sent");
var spam = document.getElementById("spam");
var archive = document.getElementById("archive");

var app = Sammy(function() {

	this.get('#/', function() {

		inbox.style="color: #fafafa; text-decoration: none";
        sent.style="color: #1b1e24; text-decoration: none";
        spam.style="color: #1b1e24; text-decoration: none";
        archive.style="color: #1b1e24; text-decoration: none";
		loadJSON("http://perso.telecom-paristech.fr/~concolat/cours/TP/igr203/mails-inbox.json");
	});

	this.get('#Inbox', function() {

		inbox.style="color: #fafafa; text-decoration: none";
        sent.style="color: #1b1e24; text-decoration: none";
        spam.style="color: #1b1e24; text-decoration: none";
        archive.style="color: #1b1e24; text-decoration: none";
		loadJSON("http://perso.telecom-paristech.fr/~concolat/cours/TP/igr203/mails-inbox.json");
	});

  	this.get('#Sent', function() {

		sent.style="color: #fafafa; text-decoration: none";
        inbox.style="color: #1b1e24; text-decoration: none";
        spam.style="color: #1b1e24; text-decoration: none";
        archive.style="color: #1b1e24; text-decoration: none";
		loadJSON("http://perso.telecom-paristech.fr/~concolat/cours/TP/igr203/mails-sent.json");
  	});

  	this.get('#Spam', function() {

		spam.style="color: #fafafa; text-decoration: none";
        inbox.style="color: #1b1e24; text-decoration: none";
        sent.style="color: #1b1e24; text-decoration: none";
        archive.style="color: #1b1e24; text-decoration: none";
		loadJSON("http://perso.telecom-paristech.fr/~concolat/cours/TP/igr203/mails-spam.json");
  	});

  	this.get('#Archive', function() {

		archive.style="color: #fafafa; text-decoration: none";
        inbox.style="color: #1b1e24; text-decoration: none";
        sent.style="color: #1b1e24; text-decoration: none";
        spam.style="color: #1b1e24; text-decoration: none";
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
