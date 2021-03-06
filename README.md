# 2021_2022_IERG4230_Mini_Project
<h2>Hardware Requirements:</h2>
<ol>
    <li>ESP8266 (with or without I/O extended board)</li>
    <li>2.4GHz USB WiFi LAN adapter</li>
    <li>Micro-USB cable</li>
    <li>AM2320 Temperature and Humidity Sensor</li>
    <li>Light Sensor Module</li>
    <li>TP-Link Smart Wi-Fi Plug</li>
</ol>

<h2>Files Explantations:</h2>
<ol>
    <li>AM2320.cpp: library for AM2320</li>
    <li>AM2320.h: Header file for AM2320 library</li>
    <li>TPLink.h: Header file for TP-Link Smart Wi-Fi Plug</li>
</ol>

<h2>How to use the Smart Room ?</h2>
<ol>
    <li>Set the smart plug to the desired wi-fi network</li>
    <li>Find out the ip-address of the smart plug</li>
    <li>Edit the ip-address of the smart plug char* hs110_1 in IERG4230_Smart_Room.ino </li>
    <li>Upload IERG4230_Smart_Room.ino to ESP8266 board</li>
</ol>

<h2> ThingSpeak API KEY-VALUE</h2>
<h3>Channel 1</h3>
<label>Table 1: Channel 1 API Key</lablel>
<table>
    <tr>
        <th>API KEY Type</th>
        <th>Key</th>
    </tr>
    <tr>
        <th>Write API Key</th>
        <th>FIUY5IXW0Y1LDKYZ</th>
    </tr>
        <th>Read API Keys</th>
        <th>HSB377D2S5LD0I0K</th>
    <tr>
    </tr>
</table>

<label>Table 2: Channel 1 API field</label>
<table>
    <tr>
        <th>Field Number</th>
        <th>Name</th>
        <th>Variable in Arduino</th>
        <th>Data Type</th>
        <th>Explanation</th>
    </tr>
    <tr>
        <th>Field 1</th>
        <th>Temperature</th>
        <th>temperature</th>
        <th>float</th>
        <th>Temperature measured from AM2320</th>
    </tr>
    <tr>
        <th>Field 2</th>
        <th>Humidity</th>
        <th>humidity</th>
        <th>float</th>
        <th>Humidity measured from AM2320</th>
    </tr>
    <tr>
        <th>Field 3</th>
        <th>Power</th>
        <th>Power</th>
        <th>float</th>
        <th>Current power reading of smart plug</th>
    </tr>
    <tr>
        <th>Field 4</th>
        <th>Air-con Status</th>
        <th>isReplayOn</th>
        <th>bool</th>
        <th>The switch status of smartplug, on = 1; off = 0</th>
    </tr>
</table>

<h3>Channel 2</h3>
<label>Table 3: Channel 2 API Key</lablel>
<table>
    <tr>
        <th>API KEY Type</th>
        <th>Key</th>
    </tr>
    <tr>
        <th>Write API Key</th>
        <th>4RNEV54YURIE2BUI</th>
    </tr>
        <th>Read API Keys</th>
        <th>1V4WJGNG9YS6LG32</th>
    <tr>
    </tr>
</table>

<label>Table 4: Channel 2 API field</label>
<table>
    <tr>
        <th>Field Number</th>
        <th>Name</th>
        <th>Variable in Arduino</th>
        <th>Data Type</th>
        <th>Explanation</th>
    </tr>
    <tr>
        <th>Field 1</th>
        <th>Web Command</th>
        <th>web_app_command</th>
        <th>int</th>
        <th>
            <br>Set the case to control the air-con</br>
            <br>1: Manual switch on/ off the air-con</br>
            <br>2: Set a temperature to switch on/ off the air-con by web app automatically</br>
            <br>3: Automatically switch on/ off the air-con by programme built-in on ESP8266
            </br>
        </th>
    </tr>
    <tr>
        <th>Field 2</th>
        <th>Turn on/ off Air-con</th>
        <th>web_app_switch_air_con</th>
        <th>bool</th>
        <th>
        <br>Control to turn on/ off the air-con by the web app</br>
        <br>1/ True: Turn on the air-con</br>
        <br>0/ False: Turn off the air-con</br>
        </th>
    </tr>
    <tr>
        <th>Field 3</th>
        <th>Air-Con Control Temperature</th>
        <th>set_temperature</th>
        <th>float</th>
        <th>The temperature set to switch on/ off the air-con by web app automatically</th>
    </tr>
</table>
