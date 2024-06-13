
# PRÁCTICA 5: Buses de Comunicación I (Introducción y I2C)
## Practica A

### Descripción:

Este programa es un escáner o herramienta de reconocimiento de periféricos conectados a un bus I2C.

Para determinar la cantidad de periféricos conectados a nuestro bus (objetivo del escáner), enviaremos señales a las 128 direcciones posibles (ya que el bus I2C utiliza direcciones de 7 bits para los dispositivos esclavos) con el fin de recibir una respuesta en alguna de estas direcciones. Si recibimos una respuesta, sabremos que hay un dispositivo conectado al bus y también obtendremos su dirección para poder comunicarnos con él. Debido a las resistencias de pull-up que tiene el bus I2C, el circuito normalmente está en alta impedancia, así que si acertamos la dirección de un periférico, lo sabremos porque el periférico alterará esta alta impedancia. En el programa, interpretaremos esta variación como un '0'.

A continuación, explico cómo funciona el programa y la salida que obtenemos por el puerto serie tras su ejecución:

Al inicio del código, como es habitual, incluimos la librería Arduino.h (ya que estamos programando en un entorno tipo Arduino y esto puede prevenir problemas) y, como novedad, incluimos la librería Wire.h. Esta última librería nos permitirá comunicarnos sobre el bus I2C, iniciando y terminando transmisiones en diferentes direcciones.

Más adelante, en el void_setup(), inicializamos el puerto serie (como siempre) y el bus I2C usando la instancia "Wire.begin;". Luego encontramos un bucle while: "while (!Serial)". Este se ejecutará hasta que el puerto serie esté listo; después de esto, escribe "\nI2C Scanner" en el puerto serie, indicándonos que el puerto serie está operativo y que el escáner se está iniciando.

El escáner comienza en el void_loop(). Como mencioné al principio, necesitamos enviar señales a todas las direcciones posibles de dispositivos en el bus y ver si obtenemos respuesta de algún periférico. Esto lo haremos con un bucle for. Para que el bucle funcione, necesitamos declarar una serie de variables: nDevices (tipo int, que define la cantidad de periféricos conectados), error (tipo byte, que indicará si ha encontrado un dispositivo en una dirección dada) y address (tipo byte, donde almacenaremos las direcciones). El bucle terminará cuando haya comprobado todas las direcciones, es decir, empieza comprobando la dirección 00000000 y suma +1 en binario hasta llegar a la última, la 01111111 (el primer 0 se debe a que la variable byte es de 8 bits y las direcciones son de 7 bits).

Enviaremos cada dirección por el bus I2C usando la función .beginTransmission() de Wire.h de la siguiente manera:

``` cpp
Wire.beginTransmission(address);
Después de iniciarla, la cerraremos con la función .endTransmission() de la misma librería, la cual nos devuelve un valor de tipo byte que guardaremos en nuestra variable error:
``` 
``` cpp
error = Wire.endTransmission();
``` 
El valor de error será 0 si la conexión ha sido exitosa. Como mencioné antes, la variación de alta impedancia y, por lo tanto, una respuesta del periférico, la interpretamos como un 0. Entonces, después de tener un valor en error, usamos un if() para preguntar si error vale 0. Si es así, enviamos por el puerto serie la dirección del dispositivo encontrado en formato hexadecimal.

Código del Programa:

```cpp
#include <Arduino.h>
#include <Wire.h>

void setup() {
    Wire.begin();
    Serial.begin(115200);
    while (!Serial) {
        // Leonardo: wait for serial monitor
    }
    Serial.println("\nI2C Scanner");
}

void loop() {
    byte error, address;
    int nDevices = 0;
    Serial.println("Scanning...");

    for (address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0) {
            nDevices++;
            Serial.print("I2C device found at address 0x");
            if (address < 16) Serial.print("0");
            Serial.println(address, HEX);
        }
    }
}
```
## Ejercicio Práctico 2
En este ejercicio, se conectan dos dispositivos al bus I2C: un sensor de temperatura y humedad, y un display OLED. El objetivo es leer la temperatura y la humedad del sensor y mostrarlas en el display.

Código del Programa:

``` cpp
#include <Adafruit_AHTX0.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_AHTX0 aht;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
    Wire.begin();
    Serial.begin(115200);
    Serial.println("Adafruit AHT10/AHT20 demo!");

    if (!aht.begin()) {
        Serial.println("¡No se pudo encontrar el AHT! ¡Verifique la conexión!");
        while (1) delay(10);
    }
    Serial.println("AHT10 o AHT20 encontrado");

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("¡Error al asignar memoria para SSD1306!"));
        for (;;);
    }

    display.display();
    delay(2000);
    display.clearDisplay();
}

void loop() {
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);

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
    Serial.println(" grados C");
    Serial.print("Humedad: ");
    Serial.print(humidity.relative_humidity);
    Serial.println("% HR");

    delay(500);
}
```
Este programa se creó con la ayuda de ChatGPT y se ajustó hasta que funcionó correctamente.

Descripción del Funcionamiento:

Inicialización:

Se incluyen las librerías necesarias: Adafruit_AHTX0.h, Adafruit_GFX.h, Adafruit_SSD1306.h, y Wire.h.
Se definen las constantes para la pantalla OLED.
Se crean instancias para el sensor y el display utilizando las clases de las librerías de Adafruit.
Configuración:

En setup(), se inicializan los buses serie e I2C.
Se comprueba la comunicación con los dispositivos utilizando aht.begin() y display.begin(). Si fallan, se muestra un mensaje de error.
Bucle Principal:

Se obtienen los valores de temperatura y humedad del sensor con aht.getEvent().
Se muestran estos valores en el display OLED y en el monitor serie.
Se actualizan los valores cada 0.5 segundos.

Conectar el sensor y el display al bus I2C, comprobando con el programa del primer ejercicio que se comunican correctamente.
