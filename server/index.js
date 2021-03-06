'use strict';

const twilio = require('twilio');
const accountSid = process.env.TWILIO_ACCOUNT_SID;
const authToken = process.env.TWILIO_AUTH_TOKEN;
const client = new twilio(accountSid, authToken);
const to = process.env.PHONE_NUMBER_TO;
const from = process.env.PHONE_NUMBER_FROM;
const moistureThreshold = 0.5;
const moistureThresholdPercent = moistureThreshold * 100;

exports.run = (req, res) => {
	console.log("Moisture level received");

	const soilMoistureValue = parseFloat(req.query.soilMoistureAmount);
	const soilMoistureValuePercent = soilMoistureValue * 100;

	if (!isSoilMoist(soilMoistureValue)) {
		console.log("Sending Low Moisture Alert");
		var message = `Excuse me, it's your plant. I'm thirsty.\nCurrent moisture level at ${soilMoistureValuePercent}%`;

		sendMessage(message).then(() => {
			// Send the response
			res.status(200).end();
		})
	} else {
		console.log(`Moisture OK: ${soilMoistureValuePercent}`)
		res.status(200).end();
	}

};

function isSoilMoist(value) {
	console.log("Comparing against moistureThreshold")
	return value > moistureThreshold;
}

function sendMessage(body) {
	return client.messages.create({
		body: body,
		to: to,  // Text this number
		from: from // From a valid Twilio number
	})
}
