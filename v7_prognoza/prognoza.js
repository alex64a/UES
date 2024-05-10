function api_call() 
{
	var url = "https://api.openweathermap.org/data/2.5/weather?q=" + document.getElementById('fname').value + "&appid=c5d32a3622a064c913548125fb9f6cc5&units=metric";
	console.log(url);
	
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() 
	{	
		if (this.readyState == 4 && this.status == 200)
		{
		   // Typical action to be performed when the document is ready:
		   
			var obj = JSON.parse(xhttp.responseText);
			console.log(xhttp.responseText);
			console.log(obj);
			
			//vremenski podaci				
			document.getElementById("loc").innerHTML = obj["name"] + ", " + obj["sys"]["country"];
			document.getElementById("temp").innerHTML = obj["main"]["temp"] + "&#x2103;";
			document.getElementById("hum").innerHTML = obj["main"]["humidity"] + "%";
			document.getElementById("pres").innerHTML = obj["main"]["pressure"] + "mB";
			
			//ikonica
			document.getElementById("weather_icon").src = "http://openweathermap.org/img/wn/" + obj["weather"][0]["icon"] + "@2x.png";
			document.getElementById("weather_description").innerHTML = obj["weather"][0]["description"];
		   
			document.getElementById("location_form").reset();
			document.getElementById("weather").style.display = "block";
		}
	};
	xhttp.open("GET", url, false);
	xhttp.send();
}
