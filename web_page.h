// web_page.h

#ifndef _WEB_PAGE_h
#define _WEB_PAGE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
char webpage[] PROGMEM = R"=====(
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="cz" lang="cz">
<script src='https://kit.fontawesome.com/a076d05399.js'></script>

<head>
  <title>wifi control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta http-equiv="content-type" content="text/html;charset=utf-8" />
  <meta name="generator" content="Geany 1.30.1" />
  <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/css/bootstrap.min.css"
    integrity="sha384-Vkoo8x4CGsO3+Hhxv8T/Q5PaXtkKtu6ug5TOeNV6gBiFeWPGFN9MuhOf23Q9Ifjh" crossorigin="anonymous">
</head>
<style>
  /* 
  ##Device = Desktops
  ##Screen = 1281px to higher resolution desktops
*/

  @media (min-width: 1281px) {

    body,
    html {
      text-align: center;
    }



    .btn {
      width: 105px;
      height: 100px;
    }

    th,
    input,
    optgroup,
    select,
    textarea {
      font-size: 56px;
    }

    .main {
      width: 800px;

    }

    .space {
      height: 150px;
    }

    .mainTable {
      margin: auto;
    }
  }

  button[type=submit] {
    margin-top: 150px;
    font-size: 30px;
  }

  input[type=radio]+label {
    font-size: 35px;
  }

  input[type=number] {
    width: 140px;
  }

  #tempLabel {
    font-size: 53px;
  }

  /* 
  ##Device = Laptops, Desktops
  ##Screen = B/w 1025px to 1280px
*/

  @media (min-width: 1025px) and (max-width: 1280px) {

    body,
    html {
      text-align: center;
    }

    .main {
      width: 600px;

    }

    .mainTable {
      margin: auto;
    }

    .p {
      text-align: center;
    }

    .space {
      height: 200px;
    }

    .btn {
      width: 135px;
      height: 80px;
    }


    .setTemp {
      height: 50px;
      width: 100px;
      font-size: 34px;

    }

    .setTime {
      height: 49px;
      width: 130px;
      font-size: 34px;
    }

    th,
    input,
    optgroup,
    select,
    textarea {
      font-size: 36px;
    }

    button[type=submit] {
      margin-top: 150px;
      font-size: 30px;
    }
    #tempLabel {
    font-size: 43px;
  }
  }

  /* 
  ##Device = Tablets, Ipads (portrait)
  ##Screen = B/w 768px to 1024px
*/

  @media (min-width: 768px) and (max-width: 1024px) {


    body,
    html {
      text-align: center;
    }

    .btn {
      width: 120px;
      height: 100px;
    }

  }

  /* 
  ##Device = Tablets, Ipads (landscape)
  ##Screen = B/w 768px to 1024px
*/

  @media (min-width: 768px) and (max-width: 1024px) and (orientation: landscape) {

    .btn {
      width: 105px;
      height: 100px;
    }

  }




  /* 
  ##Device = Low Resolution Tablets, Mobiles (Landscape)
  ##Screen = B/w 481px to 767px
*/

  @media (min-width: 481px) and (max-width: 767px) {

    body,
    html {
      height: 100%;
      margin: 0;
      text-align: center;
    }

    .btn {
      width: 120px;
      height: 130px;
    }


    .main {
      width: 100%;
      height: 100%;

    }

    .mainTable {
      margin: auto;
    }

    .space {
      height: 150px;
    }

    th,
    button,
    input,
    optgroup,
    select,
    textarea {
      font-size: 56px;
    }

    .tablink {
      font-size: 30px;
    }

    input[type=radio]+label {
      font-size: 35px;
    }

    input[type=time],
    input[type=number] {
      font-size: 60px;
    }

    button[type=submit] {
      margin-top: 150px;
      font-size: 30px;
    }

    .setTemp {
      width: 140px;
      text-align: center;
    }

    .spc {
      width: 20px;
    }
    p {
      font-size: 25px;
    }
  }

  /* 
  ##Device = Most of the Smartphones Mobiles (Portrait)
  ##Screen = B/w 320px to 479px
*/

  @media (min-width: 320px) and (max-width: 480px) {

    body,
    html {
      height: 100%;
      margin: 0;
      text-align: center;
    }

    .btn {
      width: 60px;
      height: 60px;
      font-size: 15px;
    }

    input[type=radio]+label,
    input[type=checkbox]+label {
      font-size: 18px;
    }

    input[type=time],
    input[type=number] {
      font-size: 30px;
    }

    button[type=submit] {
      margin-top: 30px;
      font-size: 25px;
    }
  }

  /*radio buttons*/
  input[type=radio],
  input[type=checkbox] {
    display: none;
  }

  input[type=radio]+label,
  input[type=checkbox]+label {
    display: inline-block;
    margin: -2px;
    padding: 4px 12px;
    margin-bottom: 0;
    /*font-size: 23px;*/
    line-height: 40px;
    color: #333;
    text-align: center;
    text-shadow: 0 1px 1px rgba(255, 255, 255, 0.75);
    vertical-align: middle;
    cursor: pointer;
    background-color: #f5f5f5;
    background-image: -moz-linear-gradient(top, #fff, #e6e6e6);
    background-image: -webkit-gradient(linear, 0 0, 0 100%, from(#fff), to(#e6e6e6));
    background-image: -webkit-linear-gradient(top, #fff, #e6e6e6);
    background-image: -o-linear-gradient(top, #fff, #e6e6e6);
    background-image: linear-gradient(to bottom, #fff, #e6e6e6);
    background-repeat: repeat-x;
    border: 1px solid #ccc;
    border-color: #e6e6e6 #e6e6e6 #bfbfbf;
    border-color: rgba(0, 0, 0, 0.1) rgba(0, 0, 0, 0.1) rgba(0, 0, 0, 0.25);
    border-bottom-color: #b3b3b3;
    filter: progid: DXImageTransform.Microsoft.gradient(startColorstr='#ffffffff', endColorstr='#ffe6e6e6', GradientType=0) filter: progid: DXImageTransform.Microsoft.gradient(enabled=false);
    -webkit-box-shadow: inset 0 1px 0 rgba(255, 255, 255, 0.2), 0 1px 2px rgba(0, 0, 0, 0.05);
    -moz-box-shadow: inset 0 1px 0 rgba(255, 255, 255, 0.2), 0 1px 2px rgba(0, 0, 0, 0.05);
    box-shadow: inset 0 1px 0 rgba(255, 255, 255, 0.2), 0 1px 2px rgba(0, 0, 0, 0.05);
  }

  input[type=radio]:checked+label,
  input[type=checkbox]:checked+label {
    background-image: none;
    outline: 0;
    -webkit-box-shadow: inset 0 2px 4px rgba(0, 0, 0, 0.15), 0 1px 2px rgba(0, 0, 0, 0.05);
    -moz-box-shadow: inset 0 2px 4px rgba(0, 0, 0, 0.15), 0 1px 2px rgba(0, 0, 0, 0.05);
    box-shadow: inset 0 2px 4px rgba(0, 0, 0, 0.15), 0 1px 2px rgba(0, 0, 0, 0.05);
    background-color: lightgreen;
  }
 .btn-secondary{
   background:yellowgreen ;
 }
  /* Style tab links */

  .tablink {
    background-color: #555;
    color: white;
    float: left;
    border: none;
    outline: none;
    cursor: pointer;
    padding: 14px 16px;
    /*font-size: 17px;*/
    width: 33.33%;
  }

  .tablink:hover {
    background-color: #777;
  }

  /* Style the tab content (and add height:100% for full page content) */

  .tabcontent {
    color: white;
    display: none;
    padding: 100px 20px;
    height: 100%;
  }

  /*.p{
  text-align: center;
}*/

  #termostat {
    background-color: #36bd36;
  }

  #prehled {
    background-color: #3f3ffd;
  }

  #spinacky {
    background-color: #e6be5a;
  }

  /*-----------------------------------------*/
</style>

<script>
  var Socket;
  var termostatLabel, spinackyLabel;
  function init() {
   Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
   Socket.onmessage = function (event) {
   SortData(event.data);
   }
    document.getElementById("defaultOpen").click();
  }


  function SortData(msg) {
    //rozsekat zpravu

    var recInp = msg.substring(msg.findIndex('I') + 2, msg.findIndex('O'));
    var recOutp = msg.substring(msg.findIndex('O') + 2, msg.findIndex('t'));
    var recTstat = msg.substring(msg.findIndex('T') + 2, msg.findIndex('S'));
    var recSpin = msg.substring(msg.findIndex('S') + 2, msg.findIndex('E'));
    //vstupy
    document.getElementById("in1").style.backgroundColor = recInp[0] == '1' ? "#ffc107" : "gray";
    document.getElementById("in2").style.backgroundColor = recInp[1] == '1' ? "#ffc107" : "gray";
    document.getElementById("in3").style.backgroundColor = recInp[2] == '1' ? "#ffc107" : "gray";
    document.getElementById("in4").style.backgroundColor = recInp[3] == '1' ? "#ffc107" : "gray";
    //vystupy
    document.getElementById("out1").style.backgroundColor = recOutp[0] == '1' ? "#ffc107" : "gray";
    document.getElementById("out2").style.backgroundColor = recOutp[1] == '1' ? "#ffc107" : "gray";
    document.getElementById("out3").style.backgroundColor = recOutp[2] == '1' ? "#ffc107" : "gray";
    document.getElementById("out4").style.backgroundColor = recOutp[3] == '1' ? "#ffc107" : "gray";

    //teplota 
    document.getElementById("tempLabel").value = msg.substring(msg.indexOf('t') + 1, msg.indexOf('T'));

    //termostat
    var teploty = document.getElementsByClassName("setTemp");
    var casy = document.getElementsByClassName("setTime");
    for (i = 0; i < teploty.length; i++) {
      teploty[i].value = recTstat.substr(recTstat.indexOf('*') + 1, recTstat.indexOfx('_'));
      casy[i].value = FromMinutes(recTstat.substr(recTstat.indexOf('_') + 1, recTstat.lastIndexOf('')));
    }

    //spinacky
    var casyOn = document.getElementsByClassName("setTimeSpinOn");
    var casyOff = document.getElementsByClassName("setTimeSpinOff");
    for (i = 0; i < casyOn.length; i++) {
      casyOn[i].value = FromMinutes(recSpin.substr(recSpin.indexOf('_') + 1, recSpin.lastIndexOf('')));
      casyOff[i].value = FromMinutes(recSpin.substr(recSpin.indexOf('_') + 1, recSpin.lastIndexOf('')));
    }
  }

  function SaveData() {
    var descripts = ["relé 1", "relé 2", "out 1", "out 2"];
    var message = "&#T*";//zprava k odeslani
    var radioTstat = document.getElementsByName("prepTstat");
    var radioSpin = document.getElementsByName("prepSpin");
    var labels = document.getElementsByClassName("label");


    //odeslani zpravy
    //termostat: teplota format 253(25,3) cas v minutach

    var teploty = document.getElementsByClassName("setTemp");
    var casy = document.getElementsByClassName("setTime");
    for (i = 0; i < teploty.length; i++) {
      var num = teploty[i].valueAsNumber * 10;
      message += num.toString() + '_';
      message += ToMinutes(casy[i].value) + "*";
    }
    var spinaciCasyOn = document.getElementsByClassName("setTimeSpinOn");
    var spinaciCasyOff = document.getElementsByClassName("setTimeSpinOff");
    message += "#S*"
    for (i = 0; i < spinaciCasyOn.length; i++) {

      message += ToMinutes(spinaciCasyOn[i].value) + "_";
      message += ToMinutes(spinaciCasyOff[i].value) + "*";
    }
    const outindex = Array.from(document.getElementsByName("prepTstat"));
    const idx = outindex.findIndex(x => x.checked == true);
    message += "#O*" + idx.toString();
    const outindex1 = Array.from(document.getElementsByName("prepSpin"));
    const idx1 = outindex1.findIndex(x => x.checked == true);
    message += "*" + idx1.toString() + "*";

    Socket.send(message);
  }

  //funkce pro prevod casu na minuty
  function ToMinutes(cas) {
    var pom = parseInt(cas.substr(0, 2));
    var pom2 = parseInt(cas.substr(3, 2));
    return (pom * 60 + pom2).toString();
  }

  //funkce pro prevod minut na HH:MM
  function FromMinutes(cas) {
    var pom = parseInt(cas);
    var pom1 = pom / 60;
    var pom2 = pom % 60;

    return pom1.toString() + ':' + pom2.toString();
  }

  function btn1click() {
    var btn = document.getElementById("out1");
    if (btn.style.backgroundColor == "gray") btn.style.backgroundColor = "green";
    else btn.style.backgroundColor = "gray";
    // else led.style.color = "red";
  }
  function btn2click() {
    var btn = document.getElementById("out2");
    if (btn.style.backgroundColor == "gray") btn.style.backgroundColor = "green";
    else btn.style.backgroundColor = "gray";
  }
  function btn3click() {
    var btn = document.getElementById("out3");
    if (btn.style.backgroundColor == "gray") btn.style.backgroundColor = "green";
    else btn.style.backgroundColor = "gray";
    // else led.style.color = "red";
  }
  function btn4click() {
    var btn = document.getElementById("out4");
    if (btn.style.backgroundColor == "gray") btn.style.backgroundColor = "green";
    else btn.style.backgroundColor = "gray";
  }

  function OutputSelect(selectedOut) {
    document.getElementById("sel1").innerText = "";
    document.getElementById("sel2").innerText = "";
    document.getElementById("sel3").innerText = "";
    document.getElementById("sel4").innerText = "";

    if (selectedOut.name == "prepTtsat") termostatLabel = selectedOut;
    if (selectedOut.name == "prepSpin") spinackyLabel = selectedOut;

    //nastaveni kontrolek
    var radioTstat = document.getElementsByName("prepTstat");
    var radioSpin = document.getElementsByName("prepSpin");
    var labels = document.getElementsByClassName("label");
    var descripts = ["relé 1", "relé 2", "out 1", "out 2"];
    for (i = 0; i < radioTstat.length; i++) {
      if (radioTstat[i].checked == true) {
        labels[i].innerText = "termostat";
        radioSpin[i].disabled = true;
        radioSpin[i].labels[0].innerText = "";

      }
      else {
        radioSpin[i].disabled = false;
        radioSpin[i].labels[0].innerText = descripts[i];
        if (labels[i].innerText != "spinacky") labels[i].innerText = "";
      }

    }
    for (i = 0; i < radioSpin.length; i++) {
      if (radioSpin[i].checked == true) {
        labels[i].innerText = "spínačky";
        radioTstat[i].disabled = true;
        radioTstat[i].labels[0].innerText = "";
      }
      else {
        radioTstat[i].disabled = false;
        radioTstat[i].labels[0].innerText = descripts[i];
        if (labels[i].innerText != "termostat") labels[i].innerText = "";
      }
    }
  }

  function openPage(pageName, elmnt, color) {
    // Hide all elements with class="tabcontent" by default */
    var i, tabcontent, tablinks;
    tabcontent = document.getElementsByClassName("tabcontent");
    for (i = 0; i < tabcontent.length; i++) {
      tabcontent[i].style.display = "none";
    }

    // Remove the background color of all tablinks/buttons
    tablinks = document.getElementsByClassName("tablink");
    for (i = 0; i < tablinks.length; i++) {
      tablinks[i].style.backgroundColor = "";
    }

    // Show the specific tab content
    document.getElementById(pageName).style.display = "block";

    // Add the specific color to the button used to open the tab content
    elmnt.style.backgroundColor = color;
  }
</script>

<body onload="init()">
  <div class="main">
    <button class="tablink" id="defaultOpen" onclick="openPage('prehled', this, 'blue')">Přehled</button>
    <button class="tablink" onclick="openPage('termostat', this, 'green')">Termostat</button>
    <button class="tablink" onclick="openPage('spinacky', this, 'goldenrod')">Spínačky</button>
    <div id=prehled class="tabcontent">
      <div>
        <div class="space"></div>
        <table class="mainTable">
          <tbody>
            <tr>
              <td>
                <button type="button" class="btn btn-secondary btn-lg" onclick="btn1click()" id="out1">RELE 1</button>
              </td>
              <td>
                <button type="button" class="btn btn-secondary btn-lg" onclick="btn2click()" id="out2">RELE 2</button>
              </td>
              <td>
                <button type="button" class="btn btn-secondary btn-lg" onclick="btn3click()" id="out3">OUT 1</button>
              </td>
              <td>
                <button type="button" class="btn btn-secondary btn-lg" onclick="btn4click()" id="out4">OUT 2</button>
              </td>
            </tr>
            <tr>
              <td>
                <p id="sel1" class="label"></p>
              </td>
              <td>
                <p id="sel2" class="label"></p>
              </td>
              <td>
                <p id="sel3" class="label"></p>
              </td>
              <td>
                <p id="sel4" class="label"></p>
              </td>
            </tr>
          </tbody>
        </table>
        <div class="space">
          <div style="margin-top:75px;">
            <span id="tempLabel" style="width:50px;height:30px;margin-top:20px; ">Teplota </span>
            <span id="tempLabel" style="width:50px;height:30px;margin-top:20px;"">23</span>
            <span id="tempLabel" style="width:50px;height:30px;margin-top:20px;""> °C</span>
          </div>
        </div>
        <div style="margin: auto;">
          <button type="button" class="btn btn-warning btn-lg" id="in1">IN 1</button>
          <button type="button" class="btn btn-warning btn-lg" id="in2">IN 2</button>
          <button type="button" class="btn btn-warning btn-lg" id="in3">IN 3</button>
          <button type="button" class="btn btn-warning btn-lg" id="in4">IN 4</button>
        </div>
      </div>
    </div>
    <!--end-->
    <div id="termostat" class="tabcontent" style="margin:auto">
      <table style="margin:auto;">
        <thead>
          <tr>
            <th>teplota</th>
            <th class="spc"></th>
            <th>čas</th>
          </tr>

        </thead>
        <tbody>
          <tr>
            <td>
              <input id="t1" class="setTemp" type="number" min="0" max="50" step="0.5"></input>
            </td>
            <td class="spc"></td>
            <td>
              <input id="tt1" class="setTime" type="time"></input>
            </td>
          </tr>
          <tr>
            <td>
              <input class="setTemp" type="number" min="0" max="50" step="0.5"></input>
            </td>
            <td class="spc"></td>
            <td>
              <input class="setTime" type="time"></input>
            </td>
          </tr>
          <tr>
            <td>
              <input class="setTemp" type="number" min="0" max="50" step="0.5"></input>
            </td>
            <td class="spc"></td>
            <td>
              <input class="setTime" type="time"></input>
            </td>
          </tr>
          <tr>
            <td>
              <input class="setTemp" type="number" min="0" max="50" step="0.5"></input>
            </td>
            <td class="spc"></td>
            <td>
              <input class="setTime" type="time"></input>
            </td>
          </tr>
        </tbody>
      </table>
      <form style="margin-top:50px">
        <fieldset style="border:1;">
          <legend>Výstup termostatu</legend>
          <div style="margin:auto">
            <input type="radio" id="rbtT1" name="prepTstat" onclick="OutputSelect(this)" value="all">
            <label for="rbtT1">relé 1</label>
            <input type="radio" id="rbtT2" name="prepTstat" onclick="OutputSelect(this)" value="false">
            <label for="rbtT2">relé 2</label>
            <input type="radio" id="rbtT3" name="prepTstat" onclick="OutputSelect(this)" value="true">
            <label for="rbtT3">out 1</label>
            <input type="radio" id="rbtT4" name="prepTstat" onclick="OutputSelect(this)" value="true">
            <label for="rbtT4">out 2</label>
          </div>
        </fieldset>
      </form>
      <div>
        <button type="submit" onclick="SaveData()">ULOŽIT</button>
      </div>
    </div>
    <div id="spinacky" class="tabcontent">
      <table style="margin:auto;">
        <thead>
          <tr>
            <th style="text-align:center">on</th>
            <th class="spc"></th>
            <th style="text-align:center">off</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td>
              <input id="sp1on" class="setTimeSpinOn" type="time"></input>
            </td>
            <td class="spc"></td>
            <td>
              <input id="sp1off" class="setTimeSpinOff" type="time"></input>
            </td>
          </tr>
          <tr>
            <td>
              <input id="sp2on" class="setTimeSpinOn" type="time"></input>
            </td>
            <td class="spc"></td>
            <td>
              <input id="sp2off" class="setTimeSpinOff" type="time"></input>
            </td>
          </tr>
          <tr>
            <td>
              <input id="sp3on" class="setTimeSpinOn" type="time"></input>
            </td>
            <td class="spc"></td>
            <td>
              <input id="sp3off" class="setTimeSpinOff" type="time"></input>
            </td>
          </tr>
          <tr>
            <td>
              <input id="sp4on" class="setTimeSpinOn" type="time"></input>
            </td>
            <td class="spc"></td>
            <td>
              <input id="sp4off" class="setTimeSpinOff" type="time"></input>
            </td>
          </tr>
        </tbody>
      </table>
      <form style="margin-top:50px">
        <fieldset style="border:1;">
          <legend>Výstup spínacích hodin</legend>
          <div style="margin:auto">
            <input type="radio" id="rbtS1" name="prepSpin" value="all" onclick="OutputSelect(this)" >
            <label for="rbtS1">relé 1</label>
            <input type="radio" id="rbtS2" name="prepSpin" value="false" onclick="OutputSelect(this)" >
            <label for="rbtS2">relé 2</label>
            <input type="radio" id="rbtS3" name="prepSpin" value="true" onclick="OutputSelect(this)" >
            <label for="rbtS3">out 1</label>
            <input type="radio" id="rbtS4" name="prepSpin" value="true" onclick="OutputSelect(this)" >
            <label for="rbtS4">out 2</label>
          </div>
        </fieldset>
      </form>
      <div>
        <button type="submit" onclick="SaveData()">ULOŽIT</button>
      </div>
    </div>
  </div>
</body>
</html>
)=====";

#endif
