#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define UART_BUFFER_SIZE 256
#define MESSAGE_START_BYTE 0x7E
#define MESSAGE_END_BYTE 0x7F
#define ACK_BYTE 0x06

uint8_t uartBuffer[UART_BUFFER_SIZE];
volatile uint16_t uartWriteIndex = 0;
volatile uint16_t uartReadIndex = 0;

typedef struct {
    uint8_t command;
    int16_t payload[10];
    uint8_t payloadSize;
} Message;

void UART_Init() {
    printf("UART Initialized\n");
}

void UART_SendByte(uint8_t byte) {
    putchar(byte);
}

uint8_t UART_ReadByte() {
    return getchar();
}

void UART_SendMessage(const Message *msg) {
    UART_SendByte(MESSAGE_START_BYTE);
    UART_SendByte(msg->command);
    for (uint8_t i = 0; i < msg->payloadSize; i++) {
        UART_SendByte((msg->payload[i] >> 8) & 0xFF);
        UART_SendByte(msg->payload[i] & 0xFF);
    }
    UART_SendByte(MESSAGE_END_BYTE);
}

void UART_InterruptHandler() {
    uint8_t receivedByte = UART_ReadByte();
    uartBuffer[uartWriteIndex++] = receivedByte;
    if (uartWriteIndex >= UART_BUFFER_SIZE) {
        uartWriteIndex = 0;
    }
}

bool UART_ParseMessage(Message *msg) {
    static uint8_t parseBuffer[UART_BUFFER_SIZE];
    static uint16_t parseIndex = 0;
    bool startFound = false;

    while (uartReadIndex != uartWriteIndex) {
        uint8_t byte = uartBuffer[uartReadIndex++];
        if (uartReadIndex >= UART_BUFFER_SIZE) {
            uartReadIndex = 0;
        }

        if (byte == MESSAGE_START_BYTE) {
            parseIndex = 0;
            startFound = true;
            continue;
        }

        if (byte == MESSAGE_END_BYTE && startFound) {
            msg->command = parseBuffer[0];
            msg->payloadSize = (parseIndex - 1) / 2;

            for (uint8_t i = 0; i < msg->payloadSize; i++) {
                msg->payload[i] = (parseBuffer[1 + i * 2] << 8) | parseBuffer[2 + i * 2];
            }

            UART_SendByte(ACK_BYTE);
            return true;
        }

        parseBuffer[parseIndex++] = byte;
        if (parseIndex >= UART_BUFFER_SIZE) {
            parseIndex = 0;
        }
    }
    return false;
}

int main() {
    UART_Init();

    Message receivedMessage;

    while (1) {
        UART_InterruptHandler();

        if (UART_ParseMessage(&receivedMessage)) {
            switch (receivedMessage.command) {
                case 0x01:
                    printf("Command 0x01 received\n");
                    break;
                default:
                    printf("Unknown command: 0x%02X\n", receivedMessage.command);
                    break;
            }
        }
    }

    return 0;
}
