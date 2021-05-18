#ifndef ULTIMEM_H
#define ULTIMEM_H

#define ULTIMEM_CONTROL     ((char *) 0x9ff0)
#define ULTIMEM_CONFIG1     ((char *) 0x9ff1)
#define ULTIMEM_CONFIG2     ((char *) 0x9ff2)
#define ULTIMEM_ID          ((unsigned short *) 0x9ff3)
#define ULTIMEM_RAM         ((unsigned short *) 0x9ff4)
#define ULTIMEM_IO          ((unsigned short *) 0x9ff6)
#define ULTIMEM_BLK1        ((unsigned short *) 0x9ff8)
#define ULTIMEM_BLK2        ((unsigned short *) 0x9ffa)
#define ULTIMEM_BLK3        ((unsigned short *) 0x9ffc)
#define ULTIMEM_BLK5        ((unsigned short *) 0x9ffe)
#define ULTIMEM_BLK5RAM     ((unsigned char *) 0x9ffe)

extern char     ultimem_is_installed (void);
extern unsigned ultimem_get_size (void);

extern void __fastcall__ ultimem_send_command (char);
extern void __fastcall__ ultimem_burn_byte (unsigned short addr, char);
extern void ultimem_erase_chip (void);
extern void __fastcall__ ultimem_erase_block (char);

extern void __fastcall__ ultimem_copy_rom2ram (long src, long dst, unsigned size);

// Map in memory bank a pointer is pointing to in another block config.
extern void * __fastcall__ ultimem_map_ptr (void * destbase, unsigned short * destreg, void * ptr, unsigned short * blockregs);

#endif /* #define ULTIMEM_H */
