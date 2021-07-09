//ESP Static IP
//var gateway = `ws://${window.location.hostname}/ws`;
var gateway = `ws://192.168.1.15/ws`;
var websocket;

//Send the URL or name of an image
function Button() { 
    let inputVal = document.getElementById("inputId").value;
    websocket.send("img "+inputVal);
}

//Initialize a webSocket connection
function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
    console.log(gateway);
}


function onOpen(event) {
    console.log('Connection opened');
}

//If the webSocket connection is closed, try to reconnect after 2 seconds
function onClose(event) {
    console.log('Connection closed');
    alert("Trying to reconnect");
    setTimeout(initWebSocket, 2000);
}

//The ESP does not check who send the datas and send it back to everyone for now, this is an echo of every messages send to the ESP. 
function onMessage(event) {
    console.log(event.data);
}

//On load, initialize a webSocket connection and put the current input values. 
window.addEventListener('load', onLoad);
function onLoad(event) {
    initWebSocket();
    adjustSize();
}

//Retrieve the current settings to the image display and send it to clients (this allow to zoom on a map, for exemple)
function adjustSize() {
    let inputWidth = document.getElementById("width");
    let inputTop = document.getElementById("top");
    let inputHeight = document.getElementById("height");
    let textWidth = document.getElementById("widthText");
    let textTop = document.getElementById("topText");
    let textHeight = document.getElementById("heightText");
    textWidth.innerHTML = inputWidth.value;
    textTop.innerHTML = inputTop.value;
    textHeight.innerHTML = inputHeight.value;
    websocket.send("width " + inputWidth.value + " height " + inputHeight.value + " top " + inputTop.value);
}