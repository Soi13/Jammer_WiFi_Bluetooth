<H1>Wifi & Bluetooth jammer</H1>
<p>The device suppress wifi and bluetooth signals.As a base I took code and circuit of [@system-linux] [FazJammer](https://github.com/system-linux/FazJammer) and simplified it a little by deleting graphics and some functions. Also insted of ESP8266 I used Arduino Nano, additionally I ussed in circuit AMS1117 voltage regulator which that takes a higher voltage (like 5V) and outputs a stable 3.3V. </p>
<p>It necessary since the nRF24L01 can spike up to 115 mA when transmitting. The Arduino Nano’s onboard 3.3V pin can only supply ~50 mA reliably, so powering directly from Nano causes brownouts - USB disconnects. With AMS1117-3.3, we give the nRF24L01 its own stable 3.3V.</p>
<h3>Components</h3>
<ul>
<li>Arduino Nano or any other arduino based controller (ESP8266, ESP32, etc)</li>
<li>NRF24L01+PA+LNA module</li>
<li>0.96" OLED SSD1306 display</li>
<li>Push Button</li>
<li>AMS1117-3.3 voltage regulator</li>
</ul>
<h3>Hardware connections</h3>
<table>
  <tr>
    <th>Arduino Nano</th>
    <th>NRF24L01+PA+LNA</th>
  </tr>
  <tr>
    <td>D9</td>
    <td>CE</td>
  </tr>
  <tr>
    <td>D10</td>
    <td>CSN</td>
  </tr>
  <tr>
    <td>D11</td>
    <td>MOSI</td>
  </tr>
  <tr>
    <td>D12</td>
    <td>MISO</td>
  </tr>
  <tr>
    <td>D13</td>
    <td>SCK</td>
  </tr>
  <tr>
    <td>GND</td>
    <td>GND</td>
  </tr>
</table><br/>

<table>
  <tr>
    <th>Arduino Nano</th>
    <th>AMS1117-3.3</th>
  </tr>
  <tr>
    <td>5V</td>
    <td>VIN</td>
  </tr>
  <tr>
    <td>GND</td>
    <td>GND</td>
  </tr>
</table><br/>

<table>
  <tr>
    <th>NRF24L01+PA+LNA</th>
    <th>AMS1117-3.3</th>
  </tr>
  <tr>
    <td>VCC</td>
    <td>VOUT (3.3V)</td>
  </tr>
</table><br/>

<table>
  <tr>
    <th>Arduino Nano</th>
    <th>OLED SSD1306</th>
  </tr>
  <tr>
    <td>5V</td>
    <td>VCC</td>
  </tr>
  <tr>
    <td>GND</td>
    <td>GND</td>
  </tr>
  <tr>
    <td>A5</td>
    <td>SCL</td>
  </tr>
  <tr>
    <td>A4</td>
    <td>SDA</td>
  </tr>
</table><br/>

<table>
  <tr>
    <th>Arduino Nano</th>
    <th>Button</th>
  </tr>
  <tr>
    <td>GND</td>
    <td>One Pin of button on GND</td>
  </tr>
  <tr>
    <td>D3</td>
    <td>Another Pin of button</td>
  </tr>
</table><br/>
