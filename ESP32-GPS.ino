// READ GPS MODULE DATA WITH ESP32 - BY OldMate6288
// CONFIGURE LCD PINS IN: \Documents\Arduino\libraries\TFT_eSPI\User_Setups\Setup43_ST7735.h

#include <TFT_eSPI.h>
#include <TinyGPS++.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

TFT_eSPI tft = TFT_eSPI();

#define GPS_SERIAL Serial2 // RX 16 TX 17

TinyGPSPlus gps;

AsyncWebServer server(80);

void setup() {
  tft.init();
  tft.setRotation(3);
  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  
  tft.setTextSize(1);
  
  GPS_SERIAL.begin(9600);

  Serial.begin(9600);

  // PGKC START UP COMMANDS/ARGUMENTS:
  // Command: 030 (System reboot command)
  // Arguments: 
  // Arg1: 
  // "1", warm start 
  // "2", warm start 
  // “3”, cold start 
  // "4", full cold start
  // Arg2:
  // “1”, software restart 
  // "2", hardware restart 
  // "3", clear nvram, keep flash reboot

  // Example：
  // Full cold start command: $PGKC030,4,2*2A<CR><LF> 
  // Hot start command: $PGKC030,1,1*2C<CR><LF> 
  
  // Arg2 is set to 1 for both warm start and warm start, and Arg2 is 1, 2, and 3 for full cold start. 
  // In general, the cold start is the full cold start mode, Arg1 is set to 4, Arg2 is set to 2, and the hardware boot mode is not used.

  // THIS IS ASSUMING YOU'RE USING AN EBYTE GPS, REFER TO YOUR GPS DATASHEET FOR MORE INFO ON COMMANDS. SOME GPS MODULES USE MTK COMMANDS INSTEAD SO BE CAREFUL.

  // Enable AGPS by sending the necessary PGKC commands to the GPS module (Hot Start currently does nothing until I figure out how to upload rinex data file to the GPS via the ESP32)
  GPS_SERIAL.println("$PGKC030,1,1*2C<CR><LF>");  // Hot Start (use GPD data if avaliable, uses last GPS position and performs a soft reboot)
  GPS_SERIAL.println("$PGKC115,1,0,0,0*2B<CR><LF>"); // (Set GPS on, set Glonass, Beidou and Galileo off. GK9501 firmware does not support Galileo star search mode.)

  // Set ESP32 as a Wi-Fi hotspot and set yourself a password
  WiFi.softAP("GPS_Hotspot", "password");

  // Print the IP address of the web server to the serial monitor (192.168.4.1)
  Serial.print("Web Server IP address: ");
  Serial.println(WiFi.softAPIP());
  
// HTML webserver with a bit of CSS to make it look nice
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  String html = "<html><head><style>";
  html += "body {";
  html += "  font-family: Arial, sans-serif;";
  html += "  background-color: #f2f2f2;";
  html += "  display: flex;";
  html += "  flex-direction: column;";
  html += "  align-items: center;";
  html += "  justify-content: center;";
  html += "  height: 100vh;";
  html += "}";
  
  html += "h1 {";
  html += "  color: #333333;";
  html += "  font-size: 48px;";
  html += "}";

  html += "p {";
  html += "  color: #666666;";
  html += "  font-size: 18px;";
  html += "}";

  html += "form {";
  html += "  margin-top: 20px;";
  html += "}";

  html += "input[type='text'] {";
  html += "  padding: 8px;";
  html += "  font-size: 16px;";
  html += "  border: 1px solid #999999;";
  html += "}";

  html += "input[type='button'] {";
  html += "  padding: 8px 16px;";
  html += "  font-size: 16px;";
  html += "  background-color: #4CAF50;";
  html += "  color: white;";
  html += "  border: none;";
  html += "  cursor: pointer;";
  html += "}";

  html += "</style><script>";
  html += "function updateData() {";
  html += "  var xhttp = new XMLHttpRequest();";
  html += "  xhttp.onreadystatechange = function() {";
  html += "    if (this.readyState == 4 && this.status == 200) {";
  html += "      var data = JSON.parse(this.responseText);";
  html += "      document.getElementById('latitude').innerHTML = 'Latitude: ' + data.latitude;";
  html += "      document.getElementById('longitude').innerHTML = 'Longitude: ' + data.longitude;";
  html += "      document.getElementById('speed').innerHTML = 'Speed: ' + data.speed;";
  html += "      document.getElementById('altitude').innerHTML = 'Altitude: ' + data.altitude;";
  html += "      document.getElementById('satellites').innerHTML = 'Satellites: ' + data.satellites;";
  html += "      document.getElementById('course').innerHTML = 'Course: ' + data.course;";
  html += "      document.getElementById('hdop').innerHTML = 'HDOP: ' + data.hdop;";
  html += "      document.getElementById('time').innerHTML = 'Time: ' + data.time;";
  html += "      document.getElementById('date').innerHTML = 'Date: ' + data.date;";
  html += "    }";
  html += "  };";
  html += "  xhttp.open('GET', '/data', true);";
  html += "  xhttp.send();";
  html += "}";
  html += "setInterval(updateData, 1000);";
  html += "</script></head><body>";
  html += "<h1>GPS Data from ESP32</h1>";
  html += "<p id='latitude'>Latitude:</p>";
  html += "<p id='longitude'>Longitude:</p>";
  html += "<p id='speed'>Speed:</p>";
  html += "<p id='altitude'>Altitude:</p>";
  html += "<p id='satellites'>Satellites:</p>";
  html += "<p id='course'>Course:</p>";
  html += "<p id='hdop'>HDOP:</p>";
  html += "<p id='time'>Time:</p>";
  html += "<p id='date'>Date:</p>";
  html += "<form id='commandForm'>";
  html += "<input type='text' id='commandInput' placeholder='Enter PGKC command'>";
  html += "<input type='button' value='Send' onclick='sendCommand()'>";
  html += "</form>";
  html += "<p>Created By: <a href='https://github.com/OldMate6288'>OldMate6288</a></p>";
  html += "<script>function sendCommand() {";
  html += "var command = document.getElementById('commandInput').value;";
  html += "var xhttp = new XMLHttpRequest();";
  html += "xhttp.onreadystatechange = function() {";
  html += "if (this.readyState == 4 && this.status == 200) {";
  html += "console.log('Command sent successfully: ' + command);";
  html += "}};";
  html += "xhttp.open('GET', '/send-command?command=' + encodeURIComponent(command), true);";
  html += "xhttp.send();}</script></body></html>";

  request->send(200, "text/html", html);
});

// Route for data URL
server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
  String jsonData = "{";
  jsonData += "\"latitude\": " + String(gps.location.lat(), 6) + ",";
  jsonData += "\"longitude\": " + String(gps.location.lng(), 6) + ",";
  jsonData += "\"speed\": " + String(gps.speed.kmph()) + ",";
  jsonData += "\"altitude\": " + String(gps.altitude.meters()) + ",";
  jsonData += "\"satellites\": " + String(gps.satellites.value()) + ",";
  jsonData += "\"course\": " + String(gps.course.deg()) + ",";
  jsonData += "\"hdop\": " + String(gps.hdop.hdop()) + ",";

  // Adjust the GPS time to GMT+8 12hr time format (If you want to remove this comment out my hourGMT8 setup and change the first time string to gps.time.hour())
  int hourGMT8 = (gps.time.hour() + 8) % 12;
  jsonData += "\"time\": \"" + String(hourGMT8) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second()) + "\",";
  jsonData += "\"date\": \"" + String(gps.date.day()) + "/" + String(gps.date.month()) + "/" + String(gps.date.year()) + "\"";
  jsonData += "}";

  request->send(200, "application/json", jsonData);
});
  // Send commands from the webserver to the GPS
  server.on("/send-command", HTTP_GET, [](AsyncWebServerRequest *request){
    String command = request->getParam("command")->value();
    GPS_SERIAL.println(command);
    request->send(200, "text/plain", "Command sent: " + command);
  });

  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404, "text/plain", "Not found");
  });

  
  server.begin();
}

void loop() { // LCD Setup
  // Read data from the GPS module
  while (GPS_SERIAL.available() > 0) {
    if (gps.encode(GPS_SERIAL.read())) {

      tft.fillScreen(TFT_BLACK);

      // Adjust the GPS time to GMT+8 12hr time format, again
      int hourGMT8 = (gps.time.hour() + 8) % 12;
      
      // Display GPS Data to LCD
      tft.setCursor(0, 0);
      tft.println("Latitude: " + String(gps.location.lat(), 6));
      tft.setCursor(0, 10);
      tft.println("Longitude: " + String(gps.location.lng(), 6));
      tft.setCursor(0, 20);
      tft.println("Speed: " + String(gps.speed.kmph()) + " km/h");
      tft.setCursor(0, 30);
      tft.println("Altitude: " + String(gps.altitude.meters()) + " m");
      tft.setCursor(0, 50);
      tft.println("Satellites: " + String(gps.satellites.value()));
      tft.setCursor(0, 40);
      tft.print("Course: " + String(gps.course.deg()) + " degrees");
      tft.setCursor(0, 60);
      tft.print("HDOP: "+ String(gps.hdop.value()));
      tft.setCursor(100, 70);
      tft.println("T:" + String(hourGMT8) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second()));
      tft.setCursor(0, 70);
      tft.println("D:" + String(gps.date.day()) + "/" + String(gps.date.month()) + "/" + String(gps.date.year()));
    }
  }

// Check for incoming Serial data from the computer
  while (Serial.available() > 0) {
    // Read the incoming command
    String command = Serial.readStringUntil('\n');
    
    // Send the PGKC command from the serial monitor to the GPS module
    GPS_SERIAL.println(command);
  }
}
