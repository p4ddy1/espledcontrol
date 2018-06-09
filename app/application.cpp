#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <Adafruit_NeoPixel/Adafruit_NeoPixel.h>

#define WIFI_SSID "ENTER_SSID_HERE"
#define WIFI_PWD "ENTER_PASSWORD_HERE"

#define PIXELCOUNT 60
#define STRIPPIN 13

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELCOUNT,STRIPPIN, NEO_GRB + NEO_KHZ800);

HttpServer server;
int StripColor = 0;

void setStripColor(){
    for(int i = 0; i<strip.numPixels(); i++){
        strip.setPixelColor(i, StripColor);
        strip.show();
    }
}

void onIndex(HttpRequest &request, HttpResponse &response){
    response.setContentType("text/html");
    response.sendString("usage: /color?r=?&g=?&b=?");
}

void onColor(HttpRequest &request, HttpResponse &response){
    int r = request.getQueryParameter("r").toInt();
    int g = request.getQueryParameter("g").toInt();
    int b = request.getQueryParameter("b").toInt();

    StripColor = strip.Color(r,g,b);
    setStripColor();
    response.setContentType("text/html");
    response.sendString("ok");
}

void gotIp(IPAddress ip, IPAddress netmask, IPAddress gateway){
    Serial.println("Connected!");
    Serial.println("IP: ");
    Serial.print(ip);
    Serial.println("");
    server.listen(80);
    server.addPath("/", onIndex);
    server.addPath("/color", onColor);
}

void init()
{
    Serial.begin(115200);

    Serial.println("Connecting to wifi...");

    WifiStation.enable(true);
    WifiStation.config(WIFI_SSID,WIFI_PWD);
    WifiAccessPoint.enable(false);
    WifiEvents.onStationGotIP(gotIp);

    strip.begin();
    setStripColor();
}
