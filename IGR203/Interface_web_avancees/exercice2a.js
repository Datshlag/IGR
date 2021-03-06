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

			data = JSON.parse(xhr.response);
			var s = '<table>\
                    	<tr>\
                        	<th>Expéditeur</th>\
                        	<th>Destinataire</th>\
                        	<th>Date</th>\
                        	<th>Sujet</th>\
                    	</tr>';

        	for(i = 0; i < data['mails'].length; i++) {

        		var s_ = '<tr>';
                    s_ += '<td>' + data['mails'][i]['from'] + '</td>';
                    s_ += '<td>' + data['mails'][i]['to'] + '</td>';
                    s_ += '<td>' + data['mails'][i]['date'] + '</td>';
                    s_ += '<td>' + data['mails'][i]['subject'] + '</td>';
                    s_ += '</tr>';
                    s  += s_;
                }

            s += '</table>';
            $('#mails').html(s);
		}
		else {

			//error occured
		}
    };

    xhr.open('GET', url, true);
    xhr.send();
};

app.run('#/');
