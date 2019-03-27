var mqtt;
var flag = true;
var clientID = Math.random().toString(36).substring(2, 5) + Math.random().toString(36).substring(2, 5);
var reconnectTimeout = 2000;
var host = "69.61.176.71"; //change this
var port = 1884;


/*const urlParams = new URLSearchParams(window.location.search);
const myParam = urlParams.get('nfc');
getParameterByName()
*/
MQTTconnect();

function pub(value, topic) {
   message = new Paho.MQTT.Message(value);
   message.destinationName = topic;
   message.retained = true;
   mqtt.send(message);
}


function onConnect() {
   // Once a connection has been made, make a subscription and send a message.
   console.log("Connected ");
   mqtt.subscribe('home/light_power');
   mqtt.subscribe('home/temperature_power');
   mqtt.subscribe('home/security_power');
   mqtt.subscribe('home/light_intensity');
   mqtt.subscribe('home/blind_status');
   mqtt.subscribe('home/temperature_set');
   mqtt.subscribe('home/last_motion_detected');
   mqtt.subscribe('home/temperature_sensor');
   mqtt.subscribe('home/security_triggered');
   mqtt.subscribe('home/brightness');
}

function MQTTconnect() {
   console.log("connecting to " + host + " " + port);
   mqtt = new Paho.MQTT.Client(host, port, "/home", clientID);
   mqtt.onMessageArrived = onMessageArrived;
   mqtt.onConnectionLost = onConnectionLost;
   var options = {
      timeout: 3,
      onSuccess: onConnect,

   };
   mqtt.connect(options);
}

function onConnectionLost(responseObject) {
   alert('connection lost');
   if (responseObject.errorCode !== 0) {
      console.log("onConnectionLost:" + responseObject.errorMessage);
   }
   MQTTconnect();
}

function onMessageArrived(message) {
   var new_message = message.payloadString;
   var topic = message.destinationName;
   switch (topic) {
      case 'home/light_power':
         if (new_message == 'on') {
            document.getElementById('lights-on-audio').play();
            flag = false;
            $('#light_slider').prop('checked', true).change();
            flag = true;
         }
         else {
            document.getElementById('lights-off-audio').play();
            flag = false;
            $('#light_slider').prop('checked', false).change();
            flag = true;
         }
         break;
      case 'home/light_intensity':
         document.getElementById("light-number").innerHTML = new_message;
         break;
      case 'home/temperature_sensor':
         document.getElementById("temp-number").innerHTML = new_message;
         break;
      case 'home/temperature_power':
         if (new_message == 'on') {
            flag = false;
            $('#hvac_slider').prop('checked', true).change();
            flag = true;
         }
         break;
      case 'home/temperature_set':
         document.getElementById("set-temp-number").innerHTML = new_message;
         break;
      case 'home/security_power':
         if (new_message == 'on') {
            document.getElementById('security-on-audio').play();
            flag = false;
            $('#security_slider').prop('checked', true).change();
            flag = true;
         }
         else{
            document.getElementById('security-off-audio').play();
            flag = false;
            $('#security_slider').prop('checked', false).change();
            flag = true;
         }
         break;
      case 'home/blind_status':
         if (new_message == 'open') {
            document.getElementById('blinds-open-audio').play();

            flag = false;
            $('#blind_slider').prop('checked', true).change();
            flag = true;
         }
         else {
            flag = false;
            document.getElementById('blinds-close-audio').play();
            $('#blind_slider').prop('checked', false).change();
            flag = true;
         }
         break;

      case 'home/last_motion_detected':
         document.getElementById('last_motion').innerHTML = new_message;
         break;
      case 'home/security_triggered':
         if (new_message == 'red') {
            document.getElementById('security-alert-audio').play();
            $('.btn-square').css("background-color", "#FF0000");
            $('.circled-number').css("border-color", "#FF0000");
            $('header').css("color", "#FF0000");
         }
         else {
            $('.btn-square').css("background-color", "#979FAA");
            $('.circled-number').css("border-color", "#3CB371  ");
            $('header').css("color", "#3CB371");

         }
         break;
      case 'home/brightness':
         document.getElementById('brightness-audio').play();
         flag = false;
         slider = document.getElementById("myRange");
         slider.value = new_message;
         flag = true;
         break;
   }
}

function plus() {
   var val = document.getElementById("set-temp-number").innerHTML;
   var new_val = parseInt(document.getElementById("set-temp-number").innerHTML) + 1;
   document.getElementById("set-temp-number").innerHTML = new_val;
   pub(new_val.toString(), 'home/temperature_set');

}

function minus() {
   var val = document.getElementById("set-temp-number").innerHTML;
   var new_val = parseInt(document.getElementById("set-temp-number").innerHTML) - 1;
   document.getElementById("set-temp-number").innerHTML = new_val;
   pub(new_val.toString(), 'home/temperature_set');
}

function hvac_toggle() {
   if (flag) {
      var val = $('#hvac_slider').prop("checked");
      if (val) {
         pub('on', 'home/temperature_power');
      }
      else {
         pub('off', 'home/temperature_power');
      }
   }
}

function light_toggle() {
   if (flag) {
      var val = $('#light_slider').prop("checked");
      if (val) {

         pub('on', 'home/light_power');
      }
      else {
         pub('off', 'home/light_power');
      }
   }
}

function security_toggle() {
   //pub('123', 'home/home_motion'); //This was a test to activate motion and see if the lights work
   if (flag) {
      var val = $('#security_slider').prop("checked");
      if (val) {
         pub('on', 'home/security_power');
      }
      else {
         pub('off', 'home/security_power');
      }
   }
}

function blind_toggle() {
   if (flag) {
      var val = $('#blind_slider').prop("checked");
      if (val) {
         pub('open', 'home/blind_status');
      }
      else {
         pub('closed', 'home/blind_status');
      }
   }
}

function brightness_slider() {
   if (flag) {
      var slider = document.getElementById("myRange");
      pub(slider.value.toString(), 'home/brightness')
   }
}



$(document).ready(function () {
   $("button").click(function () {
      if ($(this).hasClass("btn-shrink")) {
         $(this).toggleClass("btn-shrink");
      }
      else {
         $(".background.btn-shrink").toggleClass("btn-shrink");
         $(".collapse.in").removeClass("in");
         $(this).toggleClass("btn-shrink");
         $('html,body').animate({
            scrollTop: ($(this).offset().top)
         }, 500);
      }
   });
});

function getParameterByName(name, url) {
    if (!url) url = window.location.href;
    name = name.replace(/[\[\]]/g, '\\$&');
    var regex = new RegExp('[?&]' + name + '(=([^&#]*)|&|#|$)'),
        results = regex.exec(url);
    if (!results) return null;
    if (!results[2]) return '';
    return decodeURIComponent(results[2].replace(/\+/g, ' '));
}

$(window).on('load', function(){ 
   /*
   setTimeout(function(){
     var param = getParameterByName('nfc');
   switch(param){
      case 'security_on':
         pub('on','security_power');
      break;
      case 'security_off':
         pub('off','security_power');
      break;
      case 'off':

      break;
      case 'reset':
         pub('off','home/light_power')
         pub('off','home/fan')
         pub('closed','home/blind_status')
         pub('off','home/security_power')
   }
   window.close();
   }, 1000);
*/
});

