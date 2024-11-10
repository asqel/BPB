#include <kernel.h>

// Définitions des ports ATA (pour le canal primaire)
#define ATA_PRIMARY_IO_BASE      0x1F0
#define ATA_PRIMARY_CONTROL_BASE 0x3F6

// Offsets des registres principaux
#define ATA_REG_DATA             0x00
#define ATA_REG_ERROR            0x01
#define ATA_REG_SECTOR_COUNT     0x02
#define ATA_REG_LBA_LOW          0x03
#define ATA_REG_LBA_MID          0x04
#define ATA_REG_LBA_HIGH         0x05
#define ATA_REG_DEVICE           0x06
#define ATA_REG_COMMAND          0x07
#define ATA_REG_STATUS           0x07

// Commandes ATA
#define ATA_CMD_READ_SECTORS     0x20

// Masques des états ATA
#define ATA_STATUS_ERR           0x01
#define ATA_STATUS_DRQ           0x08
#define ATA_STATUS_SRV           0x10
#define ATA_STATUS_DF            0x20
#define ATA_STATUS_RDY           0x40
#define ATA_STATUS_BSY           0x80

// Fonction pour attendre que le disque soit prêt
static inline void ata_wait_busy(unsigned short int io_base) {
    while (port_read_u8(io_base + ATA_REG_STATUS) & ATA_STATUS_BSY);
}

// Lecture d'un secteur depuis l'ATA
int ata_read_sector(unsigned int lba, unsigned short int* buffer) {
    unsigned short int io_base = ATA_PRIMARY_IO_BASE;

    // Attente pour que le disque ne soit pas occupé
    ata_wait_busy(io_base);


    // Configurer le registre de périphérique (0xE0 pour disque maître avec LBA)
    port_write_u8(io_base + ATA_REG_DEVICE, 0xE0 | ((lba >> 24) & 0x0F));

    // Écrire LBA et nombre de secteurs (1 ici)
    port_write_u8(io_base + ATA_REG_SECTOR_COUNT, 1);
    port_write_u8(io_base + ATA_REG_LBA_LOW, lba & 0xFF);
    port_write_u8(io_base + ATA_REG_LBA_MID, (lba >> 8) & 0xFF);
    port_write_u8(io_base + ATA_REG_LBA_HIGH, (lba >> 16) & 0xFF);

    // Envoyer la commande de lecture
    port_write_u8(io_base + ATA_REG_COMMAND, ATA_CMD_READ_SECTORS);


    // Attendre que les données soient prêtes (DRQ = 1)
	int timeout = 10000000;
    while (!(port_read_u8(io_base + ATA_REG_STATUS) & ATA_STATUS_DRQ) && (timeout--) > 0){;}
	if (timeout == 0)
		return 0;
    // Lire les 256 mots (512 octets) dans le buffer
    for (int i = 0; i < 256; i++) {
        buffer[i] = port_read_u16(io_base + ATA_REG_DATA);
    }

    // Vérifier les erreurs
    unsigned char status = port_read_u8(io_base + ATA_REG_STATUS);
    if (status & ATA_STATUS_ERR) {
        return 0; // Erreur pendant la lecture
    }
    return 1;
}