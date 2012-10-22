#ifndef PIM_H_INCLUDED
#define PIM_H_INCLUDED

#include <stdint.h>

typedef struct Header
{
    char sign[5];
    uint16_t bpc;
    uint16_t version;
}Header;

typedef struct BUW
{
    
}BUW;

typedef struct Info
{
    uint32_t filesize;
    uint32_t palette_size;
    uint32_t palette_offset;
    uint16_t size;
    uint16_t number_color;
    uint16_t width;
    uint16_t height;
    BUW buw;
}Info;

typedef struct PIM
{
    Header head;
    Info info;
    uint8_t* id;
    uint32_t* colors;
}PIM;

int load_pim(FILE* in, PIM* p);
int blit_pim(SDL_Surface* s, PIM* p);

#endif
