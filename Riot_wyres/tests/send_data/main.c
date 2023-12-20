#include <stdio.h>
#include <unistd.h>

#include "semtech_loramac.h"

int main() {
  // Initialise LoraMAC
  semtech_loramac_init();

  // Crée une trame LoRa
  uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
  uint16_t len = sizeof(data);
  struct semtech_loramac_tx_param tx_param;
  tx_param.data = data;
  tx_param.len = len;
  tx_param.port = 1;

  // Envoie la trame
  semtech_loramac_send(&tx_param);

  // Attend la confirmation d'envoi
  while (!semtech_loramac_tx_done()) {
    usleep(10000);
  }

  // Imprime le résultat
  if (semtech_loramac_tx_status() == SEMTECH_LORAMAC_STATUS_OK) {
    printf("Envoi réussi\n");
  } else {
    printf("Envoi échoué\n");
  }

  return 0;
}