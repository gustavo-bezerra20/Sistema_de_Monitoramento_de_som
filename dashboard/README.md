# 📊 Dashboard - ThingsBoard

## 🎯 Objetivo
O dashboard foi desenvolvido na plataforma ThingsBoard com o objetivo de permitir a visualização em tempo real dos níveis de ruído capturados pelo sistema embarcado.

---

## 🔄 Fluxo de Comunicação

1. O microfone INMP441 capta o sinal sonoro.
2. O ESP32-S3 processa o áudio e calcula o valor RMS.
3. O valor é convertido para decibéis (dB).
4. Os dados são enviados via WiFi utilizando o protocolo MQTT.
5. O ThingsBoard recebe os dados e atualiza os widgets do dashboard.

---

## 📈 Componentes do Dashboard

### 🔹 Gauge (Medidor Analógico)
Exibe o valor instantâneo do nível de ruído em decibéis (dB).

### 🔹 Gráfico de Linha (Time-Series Chart)
Mostra o histórico das medições ao longo do tempo, permitindo análise de variações.

### 🔹 Indicador de Status
Mostra se o dispositivo está:
- Online
- Offline

---

## 📡 Protocolo de Comunicação
A comunicação é realizada via protocolo MQTT utilizando o token único do dispositivo configurado no ThingsBoard.

Broker MQTT:
Porta:
---

## 🧠 Aplicação
O dashboard permite:
- Monitoramento remoto
- Armazenamento histórico
- Análise de níveis de ruído
- Suporte a decisões em ambientes fechados (salas, laboratórios, escritórios)
