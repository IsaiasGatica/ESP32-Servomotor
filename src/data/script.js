var websocket;
window.addEventListener("load", onload);

function onload(event) {
  initWebSocket();
}

function initWebSocket() {
  console.log("Trying to open a WebSocket connection…");

  websocket = new WebSocket("ws://" + location.hostname + ":81/");

  websocket.onopen = onOpen;
  websocket.onclose = onClose;
  websocket.onmessage = onMessage;
}

function onOpen(event) {
  console.log("Connection opened");
}

function onClose(event) {
  console.log("Connection closed");
  setTimeout(initWebSocket, 2000);
}

function updateSliderPWM(element) {
  // Actualizar el valor del input de posición
  var positionInput = document.getElementById("positionIndice");
  var sliderValue = document.getElementById(element.id).value;
  positionInput.value = sliderValue + "°";
  sendJson("Indice", sliderValue);
}
function updateSliderPA(element) {
  // Actualizar el valor del input de posición
  var positionInput = document.getElementById("inputPulgarA");
  var sliderValue = document.getElementById(element.id).value;
  positionInput.value = sliderValue + "°";
  sendJson("PulgarA", sliderValue);
}
function updateSliderPB(element) {
  // Actualizar el valor del input de posición
  var positionInput = document.getElementById("inputPulgarB");
  var sliderValue = document.getElementById(element.id).value;
  positionInput.value = sliderValue + "°";
  sendJson("PulgarB", sliderValue);
}

function sendJson(dedo, angulo) {
  var PosicionDedo = {
    dedo,
    angulo,
  };
  var PosicionDedo = JSON.stringify(PosicionDedo);
  websocket.send(PosicionDedo);
}

function onMessage(event) {
  console.log(event.data);
  var myObj = JSON.parse(event.data);
  var keys = Object.keys(myObj);

  for (var i = 0; i < keys.length; i++) {
    var key = keys[i];
    document.getElementById(key).innerHTML = myObj[key];
    document.getElementById("slider" + (i + 1).toString()).value = myObj[key];
  }
}

function updateSliderFromInput(input) {
  // Obtener el valor del input de posición

  var positionValueSlider = input.value.replace("°", "");

  // Actualizar el valor del slider
  var slider = document.getElementById("sliIndice");
  slider.value = positionValueSlider;
}
function updateSliderFromInputPA(input) {
  // Obtener el valor del input de posición

  var positionValueSlider = input.value.replace("°", "");

  // Actualizar el valor del slider
  var slider = document.getElementById("sliderPulgarA");
  slider.value = positionValueSlider;
}
function updateSliderFromInputPB(input) {
  // Obtener el valor del input de posición

  var positionValueSlider = input.value.replace("°", "");

  // Actualizar el valor del slider
  var slider = document.getElementById("sliderPulgarB");
  slider.value = positionValueSlider;
}

var positionIndice = document.getElementById("positionIndice");
addEventos(positionIndice);
var positionPulgarA = document.getElementById("inputPulgarA");
addEventos(positionPulgarA);
var positionPulgarB = document.getElementById("inputPulgarB");
addEventos(positionPulgarB);

function addEventos(id) {
  id.addEventListener("blur", function () {
    updatePositionValue(id);
  });

  // Agregar el evento keydown para detectar la tecla Enter
  id.addEventListener("keydown", function (event) {
    if (event.key === "Enter") {
      updatePositionValue(id);
    }
  });
}

var positionValueWithoutDegreeBU;

function updatePositionValue(id) {
  // Obtener el valor actual del input

  var positionValue = id.value;

  // Verificar si el valor no tiene el símbolo de grados
  if (!positionValue.endsWith("°")) {
    // Agregar el símbolo de grados al valor
    positionValue += "°";

    // Actualizar el valor del input de posición
    id.value = positionValue;
  }

  // Eliminar el símbolo de grados para enviar el valor por websocket
  var positionValueWithoutDegree = positionValue.replace("°", "");

  if (positionValueWithoutDegree != positionValueWithoutDegreeBU) {
    console.log(id.id);
    switch (id.id) {
      case "positionIndice":
        sendJson("Indice", positionValueWithoutDegree.toString());
        console.log("indice");
        break;
      case "inputPulgarA":
        sendJson("PulgarA", positionValueWithoutDegree.toString());
        console.log("pulgar");

        break;
      case "inputPulgarB":
        sendJson("PulgarB", positionValueWithoutDegree.toString());
        break;
    }

    positionValueWithoutDegreeBU = positionValueWithoutDegree;
  }
}
