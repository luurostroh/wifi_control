// web_page.h

#ifndef _WEB_PAGE_h
#define _WEB_PAGE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
char webpage[] PROGMEM = R"=====(
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
	"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<script src='https://kit.fontawesome.com/a076d05399.js'></script>
<head>
	<title>wifi control</title>
	<meta http-equiv="content-type" content="text/html;charset=utf-8" />
	<meta name="generator" content="Geany 1.30.1" />
	<link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/css/bootstrap.min.css" integrity="sha384-Vkoo8x4CGsO3+Hhxv8T/Q5PaXtkKtu6ug5TOeNV6gBiFeWPGFN9MuhOf23Q9Ifjh" crossorigin="anonymous">
</head>
<style>
  
</style>

<script>
    var Socket;
    function init() {
      Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
      Socket.onmessage = function(event){
        var recdata = event.data.split('#');
        var i1 = document.getElementById("in1");
        var i2 = document.getElementById("in2");
        var i3 = document.getElementById("in3");
        var i4 = document.getElementById("in4");
        i1.style.color = recdata[0]=="1"? green : gray;
        i2.style.color = recdata[1]=="1"? green : gray;
        i3.style.color = recdata[2]=="1"? green : gray;
        i4.style.color = recdata[3]=="1"? green : gray;
        document.getElementById("tempDisp").value = recdata[4];
      }
    }

      function btn1click() {
        var led = document.getElementById("in1");
        if (led.style.color == "red") led.style.color = "green";
        else led.style.color = "red";
      }
      function btn2click() {
        var led = document.getElementById("in1");
        if (led.style.color == "red") led.style.color = "green";
        else led.style.color = "red";
      }
</script>
<body onload="javascript:init()">
  <div class="container" style="margin: 50px;" >
<div class="row" style="margin: center">
    <div class="col-sm" ><button onclick="btn1click()" id="r1">RELE 1</button></div>
    <div class="col-sm"><button onclick="btn1click()" id="r2">RELE 2</button></div>
  </div>
  
  <div class="row" style="margin-top: 50px;">
    <div class="col-sm"><i id="in1" class="fas fa-square" style="font-size:48px;color:gray;"></i></div>
    <div class="col-sm"><i id="in2" class="fas fa-square" style="font-size:48px;color:gray;"></i></div>
    <div class="col-sm"><i id="in3" class="fas fa-square" style="font-size:48px;color:gray;"></i></div>
    <div class="col-sm"><i id="in4" class="fas fa-square" style="font-size:48px;color:gray;"></i></div>
  </div>
  </div>
<div>
  <h1 id="tempDisp"></h1>
</div>
</body>

</html>
)=====";

#endif

