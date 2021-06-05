function parseData(data) {
	console.log(data);

	let hums = data.hums;
	let temps = data.temps;
	let dates = data.dates;

	dates = dates.map((elm) => {
		return elm.split(' ')[4];
	});

	let ctx = document.getElementById('temp-chart').getContext('2d');
		

	let tempChart = new Chart(ctx, {
		type: 'line',
		data: {
			labels: dates,
			datasets: [{
				label: 'Temperature (Celcius)',
				data: temps,
				backgroundColor: [
					'rgba(54, 162, 235, 0.2)',
				],
				borderColor: [
					'rgba(54, 162, 235, 1)',
				],
				borderWidth: 2
			}]
		}
	});

	ctx = document.getElementById('hum-chart').getContext('2d');
		

	let humChart = new Chart(ctx, {
		type: 'line',
		data: {
			labels: dates,
			datasets: [{
				label: 'Humidity',
				data: hums,
				backgroundColor: [
					'rgba(244, 208, 63, 0.2)',
				],
				borderColor: [
					'rgba(244, 208, 63, 1)',
				],
				borderWidth: 2
			}]
		}
	});
}

function getData() {
	$.ajax('api/data', {
		type: 'GET',
		dataType: 'json',
		success: data => parseData(data)
	});
}

function resetData() {
	$.ajax('api/reset', {
		type: 'POST',
		success: data => console.log(data)
	});
}

$(document).ready(() => {
	getData();
	$("#reset").on("click", (e) => {
		e.preventDefault();
		resetData();
		window.location.reload(); 
	});
});