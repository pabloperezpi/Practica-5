#include <Adafruit_AHTX0.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128 // Ancho de la pantalla OLED
#define SCREEN_HEIGHT 32 // Alto de la pantalla OLED

#define OLED_RESET -1 // Pin de reset (o -1 si se comparte el pin de reset del Arduino)
#define SCREEN_ADDRESS 0x3C ///< Ver la hoja de datos para la dirección; 0x3D para 128x64, 0x3C para 128x32

Adafruit_AHTX0 aht;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
 Wire.begin();
 // Wire1.begin(OLED_SDA, OLED_SCL);
 Serial.begin(115200);
 Serial.println("Adafruit AHT10/AHT20 demostration!");

 if (!aht.begin()) {
   Serial.println("¡No se pudo encontrar el AHT! ¡Verifique la conexión!");
   while (1) delay(10);
 }
 Serial.println("AHT10 o AHT20 encontrado");

 if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
   Serial.println(F("¡Error al asignar memoria para SSD1306!"));
   for(;;);
 }

 display.display();
 delay(2000);
 display.clearDisplay();
}

void loop() {
 sensors_event_t humidity, temp;
 aht.getEvent(&humidity, &temp); // Obtener los objetos de temperatura y humedad con datos actualizados

 display.clearDisplay();
 display.setTextSize(1);
 display.setTextColor(SSD1306_WHITE);
 display.setCursor(0, 0);
 display.print("Temperatura: ");
 display.println(temp.temperature);
 display.print("Humedad: ");
 display.println(humidity.relative_humidity);

 display.display();
 Serial.print("Temperatura: ");
 Serial.print(temp.temperature);
 Serial.println(" Grados °C");
 Serial.print("Humedad: ");
 Serial.print(humidity.relative_humidity);
 Serial.println("% HR");

 delay(500);
}