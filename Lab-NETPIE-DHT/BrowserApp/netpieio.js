mqtt_host = "mqtt.netpie.io";
clientID = "d5b5ca3b-4a87-4923-bc2c-c855d1c36f86" 
token = "vyLeUKzqmFQWEfjZvGpzvP4qLKQewvtV";
secret_key = "SGNndKKfhjakV9AJj77QmvkZWjdrHzUx";
topic_pub = "@msg/lab_ict_kps/sensor_data";
topic_sub = "@msg/lab_ict_kps/sensor_data";
client = new Paho.MQTT.Client(mqtt_host, 443, clientID);
client.onMessageArrived = onMessageArrived;

var options = {
    useSSL: true,
    userName: token,
    password: secret_key,
    onSuccess: onConnect,
    onFailure: onError
}
client.connect(options);

function onConnect() {
    client.subscribe(topic_sub);
    document.getElementById('connectState').innerHTML = "Connected...";
    console.log("NetPIE is Connected...")
}

function onError(e) {
    document.getElementById('connectState').innerHTML = "Not Connected!";
    console.log(e);
}

function sendData() {
    element_data = document.getElementById('inputdata').value;
    mqttSend(topic_pub, element_data);
}

function onMessageArrived(message) {
    document.getElementById('showdata').innerHTML = message.payloadString;
    parserPayloadMessage(message.payloadString);
}

function parserPayloadMessage(message) {
    const payload = JSON.parse(message);
    humidityValue = payload.data.humidity;
    temperatureValue = payload.data.temperature;
    console.log("Humidity: " + humidityValue);
    console.log("Temperature: " + temperatureValue);
    document.getElementById("temperatureData").innerHTML = temperatureValue;
    document.getElementById("humidityData").innerHTML = humidityValue;
}

var mqttSend = function(topic, msg) {
    var message = new Paho.MQTT.Message(msg);
    message.destinationName = topic;
    client.send(message);
}