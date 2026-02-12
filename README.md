# 🔊 Sistema_de_Monitoramento_de_Ruidos_Em_Ambientes_Fechados

 > **Resumo:** Monitora Níveis de Ruídos em determinado e especifico ambientes particulares

---

## 📒 Desenvolvimento do Pojeto
> Este projeto consiste no desenvolvimento de um sistema embarcado capaz de monitorar níveis de ruído em ambientes fechados em tempo real. Utilizando um microfone digital INMP441 integrado ao ESP32-S3 via protocolo I2S, o sistema realiza a captura e processamento do sinal sonoro. O valor eficaz (RMS) é calculado e convertido para decibéis (dB), aplicando um fator de calibração experimental. Os dados são enviados via WiFi utilizando o protocolo MQTT para a plataforma ThingsBoard, onde são exibidos em um dashboard interativo. A solução permite acompanhamento remoto, armazenamento histórico das medições e análise contínua dos níveis de ruído. O projeto demonstra a aplicação de tecnologias IoT de baixo custo voltadas para monitoramento ambiental e saúde ocupacional.
