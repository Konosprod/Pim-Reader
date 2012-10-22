#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include "pim.h"

void waitAndQuit()
{
    SDL_Event event;

    while ( event.type!= SDL_QUIT)
        SDL_WaitEvent( &event );

    SDL_Quit();
}


void usage(char* name)
{
    printf("%s [FILE]\n", name);
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[])
{
    PIM image;
    FILE* in;
    SDL_Surface* ecran = NULL;
    SDL_Event event;
    
    if(argc != 2)
    {
        usage(argv[0]);
    }
    
    in = fopen(argv[1], "rb");
    
    if(in == NULL)
    {
        printf("Unable to open : %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    
    if(load_pim(in, &image) != 0)
    {
        printf("Unable to load : %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    SDL_Init(SDL_INIT_VIDEO);
    
    ecran = SDL_SetVideoMode(image.info.width, image.info.height,
                             32, SDL_ANYFORMAT | SDL_HWSURFACE );
                             
                             
    blit_pim(ecran, &image);
    
    SDL_Flip(ecran);
    
    while(event.type != SDL_QUIT)
    {
        SDL_WaitEvent(&event);
        
        switch(event.type)
        {
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    break;
                
                if(event.key.keysym.sym == SDLK_p)
                    //convert_bmp(ecran, argv[1]);
           break;
           
           default:
           break;
       }
   }
    
    free(image.id);
    free(image.colors);
    fclose(in);
    
    return 0;
}
