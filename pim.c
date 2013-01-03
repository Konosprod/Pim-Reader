#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "pim.h"

int get_pim_header(FILE* in, PIM* p);
int get_pim_info(FILE* in, PIM* p);
int get_pim_data(FILE* in, PIM* p);
int get_pim_palette(FILE* in, PIM* p);
void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

/***************************************************
 * Charge le fichier in dans l'image p
 **************************************************/
int load_pim(FILE* in, PIM* p)
{
    get_pim_header(in, p);
    get_pim_info(in, p);
    get_pim_data(in, p);
    get_pim_palette(in, p);
    
    return 0;
}


/***************************************************
 * Affiche l'image p sur la surface s via la méthode 
 * de fast-blit                      
 * ************************************************/
int blit_pim(SDL_Surface* s, PIM* p)
{
    
    SDL_Surface* surf = NULL;
    SDL_Rect pos;
    
    pos.w = 16;
    pos.h = 8;
    
    surf = SDL_CreateRGBSurface(SDL_HWSURFACE, 16, 8, 32, 0, 0, 0, 0);
    
    for(int i = 0; i < (p->info.width*p->info.height)/0x80; i++)
    {
        SDL_LockSurface(surf);
        for(int j = 0; j < 0x80; j++)
        {
            set_pixel(surf, j%16, j/16, p->colors[p->id[i*0x80+j]]);
        }
        SDL_UnlockSurface(surf);
        
        pos.x = 16*(i%(p->info.width/16));
        pos.y = 8*(i/(p->info.width/16));
        
        SDL_BlitSurface(surf, NULL, s, &pos);
    }
    
    SDL_FreeSurface(surf);
    
    return 0;
}

/***************************************************
 * Récupère la palette du fichier in dans l'image p
 * ************************************************/
int get_pim_palette(FILE* in, PIM* p)
{
    fseek(in, p->info.palette_offset, SEEK_SET);
    
    p->colors = calloc(p->info.number_color, sizeof(char)*p->head.bpc);
    
    if(p->colors == NULL)
    {
        printf("Bad alloc\n");
        free(p->id);
        exit(EXIT_FAILURE);
    }
    
    for(int i = 0; i < p->info.number_color; i++)
    {
        fread(&p->colors[i], sizeof(char), p->head.bpc, in);
        /*printf("Color[%d] : #%X\n", i, p->colors[i]);*/
    }
    
    return 0;
}

/***************************************************
 * Récupère les données des pixels du fichier in 
 * dans l'image p
 * ************************************************/
int get_pim_data(FILE* in, PIM* p)
{
    int size = p->info.width*p->info.height;
    
    //printf("Image data size : 0x%.8X\n", size);
    
    p->id = calloc(size, sizeof(uint8_t));
    
    if(p->id == NULL)
    {
        printf("Bad alloc\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        for(int i = 0; i < size; i++)
        {
            fread(&p->id[i], sizeof(char), 1, in);
        }
    }
    
    return 0;
}


/***************************************************
 * Récupère le header du fichier in dans l'image p
 * ************************************************/
int get_pim_header(FILE* in, PIM* p)
{
    if(fread(&p->head.sign, sizeof(char), 4, in) != 4)
    {
        printf("Failed to read sign\n");
        return -1;
    }
    
    p->head.sign[4] = '\0';
    
    fread(&p->head.bpc, sizeof(char), 2, in);
    fread(&p->head.version, sizeof(char), 2, in);

   /* printf("Signature : %s\n", p->head.sign);
    printf("Byte per color : %d\n", p->head.bpc);
    printf("Version : %.2lf\n", (double)p->head.version);*/
    return 0;
}


/***************************************************
 * Récupère toutes les infos qui ne sont pas des 
 * infos pixels ou de couleur ni le header
 * ************************************************/
int get_pim_info(FILE* in, PIM* p)
{
    fseek(in, 0x10, SEEK_SET);
    
    fread(&p->info.filesize, sizeof(char), 4, in);
    p->info.filesize+=0x10;
    
    fread(&p->info.palette_size, sizeof(char), 4, in);
    fread(&p->info.palette_offset, sizeof(char), 4, in);
    fread(&p->info.size, sizeof(char), 2, in);
    fread(&p->info.number_color, sizeof(char), 2, in);
    
    fseek(in, 0x04, SEEK_CUR);
    
    fread(&p->info.width, sizeof(char), 2, in);
    fread(&p->info.height, sizeof(char), 2, in);
    
    p->info.palette_offset += 0x10 + p->info.size;
    
   // printf("Filesize : 0x%.8X\n", p->info.filesize);
   // printf("Palette size : 0x%.8X\n", p->info.palette_size);
   // printf("Palette offset : 0x%.8X\n", p->info.palette_offset);
   // printf("Info chunk size : 0x%.8X\n", p->info.size);
   // printf("Number color : %d\n", p->info.number_color);
   // printf("Width : 0x%.2X | %d\n", p->info.width, p->info.width);
   // printf("Height : 0x%.2X | %d\n", p->info.height, p->info.height);
    
    fseek(in, p->info.size + 0x10, SEEK_SET);
    
    return 0;
}

/***************************************************
 * Met le pixel de la surface aux coordonnées (x, y)
 * a la couleur pixels
 * ************************************************/
void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int nbOctetsParPixel = surface->format->BytesPerPixel;
    
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;

    switch(nbOctetsParPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}
