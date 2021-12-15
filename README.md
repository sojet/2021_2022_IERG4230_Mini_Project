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

<h2> ThingSpeak API KEY-VALUE</h2>
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
    <tr>
        <th>Field 5</th>
        <th>Web Command</th>
        <th>Web Command</th>
        <th>int</th>
        <th>1</th>
    </tr>
    <tr>
        <th>Field 6</th>
        <th>Power</th>
        <th>Power</th>
        <th>float</th>
        <th>Current power reading of smart plug</th>
    </tr>
    <tr>
        <th>Field 7</th>
        <th>Air-con Status</th>
        <th>isReplayOn</th>
        <th>bool</th>
        <th>The switch status of smartplug, on = 1; off = 0</th>
    </tr>
</table>
