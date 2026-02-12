#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <driver/i2s.h>
#include <math.h>

// ===== WiFi =====
const char* ssid = "SEU_WIFI";
const char* password = "SUA_SENHA";

// ===== ThingsBoard =====
const char* mqtt_server = "thingsboard.cloud";
const char* token = "SEU_TOKEN_THINGSBOARD";

// ===== I2S ESP32-S3 =====
#define I2S_WS   5
#define I2S_SD   4
#define I2S_SCK  6
#define I2S_PORT I2S_NUM_0

// ===== Configurações de Áudio =====
#define SAMPLE_BLOCK 512    // Quantidade de amostras por leitura
const float OFFSET = 40.0;  // Ajuste este valor para calibrar com um decibelímetro real

WiFiClient espClient;
PubSubClient client(espClient);

void conectar_wifi() {
  if (WiFi.status() == WL_CONNECTED) return;
  Serial.print("Conectando ao WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi Conectado!");
}

void conectar_mqtt() {
  while (!client.connected()) {
    Serial.print("Conectando MQTT...");
    if (client.connect("ESP32S3", token, NULL)) {
      Serial.println(" OK!");
    } else {
      delay(3000);
    }
  }
}

void setup_i2s() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = SAMPLE_BLOCK,
    .use_apll = false
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_PORT, &pin_config);
}

void setup() {
  Serial.begin(115200);
  conectar_wifi();
  client.setServer(mqtt_server, 1883);
  setup_i2s();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) conectar_wifi();
  if (!client.connected()) conectar_mqtt();
  client.loop();

  int16_t sBuffer[SAMPLE_BLOCK];
  size_t bytes_read;
  
  // Lê um bloco inteiro de amostras do microfone
  i2s_read(I2S_PORT, &sBuffer, sizeof(sBuffer), &bytes_read, portMAX_DELAY);

  int samples_count = bytes_read / sizeof(int16_t);
  if (samples_count > 0) {
    float mean_sq = 0;
    
    // Calcula o RMS (Root Mean Square) para estabilidade
    for (int i = 0; i < samples_count; i++) {
      mean_sq += pow(sBuffer[i], 2);
    }
    float rms = sqrt(mean_sq / samples_count);
    
    // Cálculo de dB com proteção contra log de zero
    float decibeis = 20.0 * log10(rms + 1.0) + OFFSET;

    // Envio para o ThingsBoard
    StaticJsonDocument<100> doc;
    doc["db"] = (int)decibeis;

    char payload[100];
    serializeJson(doc, payload);
    
    client.publish("v1/devices/me/telemetry", payload);
    Serial.printf("RMS: %.2f | dB: %.2f\n", rms, decibeis);
  }

  delay(200); // Frequência de atualização (0.2 segundos)
}
