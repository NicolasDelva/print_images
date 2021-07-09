//ESP Static IP 
//var gateway = `ws://${window.location.hostname}/ws`;
var gateway = `ws://192.168.1.15/ws`;
var websocket;

//Initilize webSocket connection
function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
    console.log(gateway);
}

//Connection successful
function onOpen(event) {
    console.log('Connection opened');
}

//Connection closed. Happened periodically
function onClose(event) {
    console.log('Connection closed');
    alert("Connection lost !");
    setTimeout(initWebSocket, 2000);
}

/*  Reacts to the incoming of a message. Print image on screen OR retrieve the size info to adapt the size an image
        event.data : http link of an image / name of an image on the computer 
        OR
        event.data : width, left and top to scale the image

*/
function onMessage(event) {
    console.log(event.data);
    var image = document.getElementById('myImage');
    let compute = event.data.split(" ");
    console.log(compute);
    if (compute.length === 2) {
        if (compute[1].includes("http")) {
            image.src = compute[1];
        } else {
            image.src = "file://pzreswalsky/Users/utilisateur/Documents/JDR%20Pathfinder/Personnage%20%E0%20imprimer/" + compute[1];
        }
        
    } else if (compute.length === 6) {
        image.style.width = compute[1] + "%"
        image.style.left = compute[3] + "%"
        image.style.top = compute[5] + "%"
    }
}

//On load, initialize a webSocket connection
window.addEventListener('load', onLoad);
function onLoad(event) {
    initWebSocket();
}