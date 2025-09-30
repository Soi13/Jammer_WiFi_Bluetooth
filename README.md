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
</table>
