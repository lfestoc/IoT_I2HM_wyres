#include <stdio.h>
#include "thread.h"
#include "semtech_loramac.h"
#include "xtimer.h"
#include <string.h>

#define SENDING_INTERVAL (30 * US_PER_SEC)  // Envoyer toutes les 30 secondes

static semtech_loramac_t loramac;

const uint8_t deveui[8] = {0xCA, 0xFE, 0xBA, 0xBE, 0xC6, 0xB0, 0xDA, 0xF7};
const uint8_t appeui[12] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t appkey[16] = {0xD6, 0x90, 0x93, 0x71, 0x8B, 0xAA, 0x94, 0x73, 0x93, 0x88, 0xFA, 0x5B, 0x58, 0x66, 0x78, 0xB2};

static char stack[THREAD_STACKSIZE_DEFAULT];

static void *send_thread(void *arg)
{
    (void)arg;

    while (1) {
        printf("Sending data...\n");

        // Message à envoyer
        const char *message = "Hello World";

        // Longueur du message
        uint8_t len = strlen(message);

        // Envoyer le message
        if (semtech_loramac_send(&loramac, (uint8_t *)message, len) != SEMTECH_LORAMAC_TX_DONE) {
            printf("Failed to send message\n");
        }

        // Attendre la fin de la transmission
        semtech_loramac_recv(&loramac);

        printf("Message sent successfully\n");

        xtimer_sleep(1);  // Attendez un certain temps avant d'envoyer à nouveau
    }

    return NULL;
}

int main(void)
{
    // Initialiser LoRaMAC
    semtech_loramac_init(&loramac);

    // Configuration loramac
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);
    semtech_loramac_set_dr(&loramac, 5);
    semtech_loramac_set_tx_power(&loramac, 20);

    // Démarrer le thread d'envoi
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, send_thread, NULL, "send_thread");
    puts("Message envoyé !");

    // Attendre indéfiniment
    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}
