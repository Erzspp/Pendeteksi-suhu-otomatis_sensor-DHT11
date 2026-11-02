#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// Definisi Ukuran OLED SSD1306 (128x64 pixel)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 // Reset pin (-1 jika berbagi pin reset dengan ESP)
#define SCREEN_ADDRESS 0x3C // Alamat I2C umum untuk OLED 0.96 inch

// Inisialisasi Objek OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Definisi Pin dan Tipe Sensor DHT
#define DHTPIN D5     // Pin data DHT11 terhubung ke D5 (GPIO 14)
#define DHTTYPE DHT11 // Tipe sensor DHT11

// Inisialisasi Objek DHT
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(115200);

  // Inisialisasi Komunikasi I2C pada Pin D2 (SDA) & D1 (SCL)
  Wire.begin(D2, D1);

  // Inisialisasi Sensor DHT11
  dht.begin();

  // Inisialisasi Layar OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Gagal menginisialisasi OLED SSD1306!"));
    for (;;); // Berhenti di sini jika OLED tidak terdeteksi
  }

  // Tampilan Awal / Splash Screen
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 20);
  display.println(F("Mulai Membaca"));
  display.setCursor(25, 35);
  display.println(F("Sensor Suhu..."));
  display.display();
  delay(2000);
}

void loop() {
  // Pembacaan DHT11 membutuhkan jeda minimal 1-2 detik
  delay(2000);

  // Membaca Kelembapan (%) dan Suhu (Celsius)
  float kelembapan = dht.readHumidity();
  float suhu = dht.readTemperature();

  // Validasi Pembacaan Sensor
  if (isnan(kelembapan) || isnan(suhu)) {
    Serial.println(F("Gagal membaca dari sensor DHT11!"));
    
    // Tampilkan pesan error pada OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 25);
    display.println(F("Error: Sensor DHT11"));
    display.println(F("Tidak Terbaca!"));
    display.display();
    return;
  }

  // Cetak hasil pembacaan ke Serial Monitor
  Serial.print(F("Suhu: "));
  Serial.print(suhu);
  Serial.print(F(" *C | Kelembapan: "));
  Serial.print(kelembapan);
  Serial.println(F(" %"));

  // Tampilkan Hasil Pembacaan pada Layar OLED
  display.clearDisplay();

  // Judul Header
  display.setTextSize(1);
  display.setCursor(20, 0);
  display.println(F("MONITOR SUHU ESP"));
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE); // Garis pemisah

  // Tampilan Suhu
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.print(F("Suhu       : "));
  display.setTextSize(2);
  display.setCursor(0, 30);
  display.print(suhu, 1);
  display.setTextSize(1);
  display.print(F(" "));
  display.write(247); // Simbol derajat (°)
  display.println(F("C"));

  // Tampilan Kelembapan
  display.setTextSize(1);
  display.setCursor(0, 48);
  display.print(F("Kelembapan : "));
  display.print(kelembapan, 1);
  display.println(F(" %"));

  // Perbarui Layar OLED
  display.display();
}