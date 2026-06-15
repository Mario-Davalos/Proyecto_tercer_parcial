#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Jugador.h"
#include "Enemigo.h"
#include "Bala.h"
#include <iostream>

enum EstadoJuego { MENU, JUGANDO, JEFE, VICTORIA };

SDL_Texture* cargarTextura(const char* ruta, SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(ruta);
    if (!surface) { std::cout << "Error: " << ruta << " - " << IMG_GetError() << std::endl; return nullptr; }
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 255, 255));
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    SDL_FreeSurface(surface);
    return tex;
}

int main()
{
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    SDL_Window* ventana = SDL_CreateWindow("MANDATO FINAL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1376, 768, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* texMapa    = cargarTextura("gallery/mapa-1.png",    renderer);
    SDL_Texture* texMillon  = cargarTextura("gallery/Millonario.png", renderer);
    SDL_Texture* texBuchona = cargarTextura("gallery/Buchona.png",    renderer);
    SDL_Texture* texAlucin  = cargarTextura("gallery/Alucin.png",     renderer);
    SDL_Texture* texPelucho = cargarTextura("gallery/Peluchon.png",   renderer);
    SDL_Texture* texEnemigo = cargarTextura("gallery/Enemigo.png",    renderer);
    SDL_Texture* texJefe    = cargarTextura("gallery/Jefe.png",       renderer);

    EstadoJuego estado = MENU;
    int personaje = 0;
    float jugX = 150, jugY = 580, velY = 0;
    bool enSuelo = true, miraDerecha = true;
    int vidaJugador = 5;
    float balaX = -100, balaY = -100;
    bool balaActiva = false, balaVaDerecha = true;

    const int N = 10;
    float enX[N] = {350,500,650,800,950,400,580,720,880,1050};
    float enY[N] = {580,580,580,580,580,480,480,480,480,480};
    bool enVivo[N];
    for (int i = 0; i < N; i++) enVivo[i] = true;
    int eliminados = 0;

    float jefeX = 900, jefeY = 500;
    int vidaJefe = 10, jefeDir = -1;
    float jefeVelX = 2.5f;

    const int MBJ = 5;
    float bjX[MBJ], bjY[MBJ], bjVelX[MBJ];
    bool bjActiva[MBJ];
    for (int i = 0; i < MBJ; i++) bjActiva[i] = false;
    int timerJefe = 0, cooldownDano = 0, tick = 0;
    bool corriendo = true;
    SDL_Event evento;

    while (corriendo)
    {
        tick++;
        while (SDL_PollEvent(&evento))
        {
            if (evento.type == SDL_QUIT) corriendo = false;
            if (evento.type == SDL_KEYDOWN)
            {
                if (estado == MENU)
                {
                    if (evento.key.keysym.sym == SDLK_RETURN) estado = JUGANDO;
                    if (evento.key.keysym.sym == SDLK_RIGHT) { personaje++; if (personaje>3) personaje=0; }
                    if (evento.key.keysym.sym == SDLK_LEFT)  { personaje--; if (personaje<0) personaje=3; }
                }
                if (estado == JUGANDO || estado == JEFE)
                {
                    if (evento.key.keysym.sym == SDLK_SPACE && !balaActiva)
                    {
                        balaActiva = true; balaVaDerecha = miraDerecha;
                        balaX = jugX + (miraDerecha ? 110 : -20); balaY = jugY + 35;
                    }
                }
                if (estado == VICTORIA && evento.key.keysym.sym == SDLK_ESCAPE) corriendo = false;
            }
        }

        const Uint8* keys = SDL_GetKeyboardState(NULL);
        if (estado == JUGANDO || estado == JEFE)
        {
            if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])  { jugX -= 5; miraDerecha = false; }
            if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) { jugX += 5; miraDerecha = true; }
            if ((keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP]) && enSuelo) { velY = -17; enSuelo = false; }
            velY += 0.7f; jugY += velY;
            if (jugY >= 580) { jugY = 580; velY = 0; enSuelo = true; }
            if (jugX < 0) jugX = 0;
            if (jugX > 1260) jugX = 1260;

            if (balaActiva)
            {
                balaX += balaVaDerecha ? 15 : -15;
                if (balaX > 1400 || balaX < -50) balaActiva = false;
                if (estado == JUGANDO)
                {
                    for (int i = 0; i < N; i++)
                    {
                        if (!enVivo[i]) continue;
                        if (balaX > enX[i] && balaX < enX[i]+70 && balaY > enY[i] && balaY < enY[i]+100)
                        { enVivo[i] = false; balaActiva = false; eliminados++; }
                    }
                }
                if (estado == JEFE)
                {
                    if (balaX > jefeX && balaX < jefeX+130 && balaY > jefeY && balaY < jefeY+160)
                    { balaActiva = false; vidaJefe--; if (vidaJefe <= 0) estado = VICTORIA; }
                }
            }
            if (estado == JUGANDO && eliminados >= N) { estado = JEFE; jefeX = 900; jefeY = 500; }
        }

        if (estado == JEFE)
        {
            jefeX += jefeVelX * jefeDir;
            if (jefeX > 1100) jefeDir = -1;
            if (jefeX < 600)  jefeDir =  1;
            timerJefe++;
            if (timerJefe >= 90)
            {
                timerJefe = 0;
                for (int i = 0; i < MBJ; i++)
                {
                    if (!bjActiva[i]) { bjX[i]=jefeX+20; bjY[i]=620; bjVelX[i]=(jefeX>jugX)?-9.0f:9.0f; bjActiva[i]=true; break; }
                }
            }
            if (cooldownDano > 0) cooldownDano--;
            for (int i = 0; i < MBJ; i++)
            {
                if (!bjActiva[i]) continue;
                bjX[i] += bjVelX[i];
                if (bjX[i] < -50 || bjX[i] > 1450) { bjActiva[i]=false; continue; }
                if (bjX[i]>jugX && bjX[i]<jugX+110 && 620>jugY+20 && 620<jugY+130 && cooldownDano==0)
                { vidaJugador--; cooldownDano=90; bjActiva[i]=false; if(vidaJugador<=0){vidaJugador=5;jugX=150;jugY=580;} }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (estado == MENU)
        {
            SDL_SetRenderDrawColor(renderer, 15, 10, 30, 255); SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 220, 180, 0, 255);
            SDL_Rect t1={388,80,600,12}, t2={388,160,600,12};
            SDL_RenderFillRect(renderer,&t1); SDL_RenderFillRect(renderer,&t2);
            SDL_SetRenderDrawColor(renderer, 255, 220, 0, 255);
            for (int i=0;i<13;i++){SDL_Rect b={420+i*40,100,28,50};SDL_RenderFillRect(renderer,&b);}
            SDL_Texture* pj[4]={texMillon,texBuchona,texAlucin,texPelucho};
            for (int i=0;i<4;i++)
            {
                SDL_Rect r={150+i*270,250,160,220};
                if (i==personaje && (tick/15)%2==0){SDL_SetRenderDrawColor(renderer,255,220,0,255);SDL_Rect m={r.x-6,r.y-6,r.w+12,r.h+12};SDL_RenderFillRect(renderer,&m);}
                if (pj[i]) SDL_RenderCopy(renderer,pj[i],nullptr,&r);
                else{SDL_SetRenderDrawColor(renderer,80,80,200,255);SDL_RenderFillRect(renderer,&r);}
            }
            if ((tick/20)%2==0){SDL_SetRenderDrawColor(renderer,200,200,200,255);SDL_Rect i1={488,560,400,8},i2={488,580,400,8};SDL_RenderFillRect(renderer,&i1);SDL_RenderFillRect(renderer,&i2);}
        }

        if (estado == JUGANDO || estado == JEFE)
        {
            if (texMapa) SDL_RenderCopy(renderer,texMapa,nullptr,nullptr);
            else{SDL_SetRenderDrawColor(renderer,30,60,30,255);SDL_RenderClear(renderer);}
            SDL_SetRenderDrawColor(renderer,60,40,20,255);
            SDL_Rect piso={0,670,1376,100}; SDL_RenderFillRect(renderer,&piso);

            SDL_Texture* texJug=texMillon;
            if(personaje==1)texJug=texBuchona;
            if(personaje==2)texJug=texAlucin;
            if(personaje==3)texJug=texPelucho;
            bool renderJug=!(cooldownDano>0&&(tick/5)%2==0);
            if(renderJug){
                SDL_Rect rj={(int)jugX,(int)jugY,110,130};
                if(texJug) SDL_RenderCopyEx(renderer,texJug,nullptr,&rj,0,nullptr,miraDerecha?SDL_FLIP_NONE:SDL_FLIP_HORIZONTAL);
                else{SDL_SetRenderDrawColor(renderer,0,150,255,255);SDL_RenderFillRect(renderer,&rj);}
            }
            if(balaActiva){
                SDL_SetRenderDrawColor(renderer,255,255,100,255);SDL_Rect rb={(int)balaX,(int)balaY,20,8};SDL_RenderFillRect(renderer,&rb);
                SDL_SetRenderDrawColor(renderer,255,180,0,255);SDL_Rect rb2={(int)balaX+2,(int)balaY+2,14,4};SDL_RenderFillRect(renderer,&rb2);
            }
            if(estado==JUGANDO){
                for(int i=0;i<N;i++){
                    if(!enVivo[i])continue;
                    SDL_Rect re={(int)enX[i],(int)enY[i],70,100};
                    if(texEnemigo)SDL_RenderCopy(renderer,texEnemigo,nullptr,&re);
                    else{SDL_SetRenderDrawColor(renderer,180,30,30,255);SDL_RenderFillRect(renderer,&re);}
                }
                for(int i=0;i<N;i++){
                    SDL_Rect ind={10+i*32,10,26,26};
                    SDL_SetRenderDrawColor(renderer,enVivo[i]?220:40,enVivo[i]?40:200,40,255);
                    SDL_RenderFillRect(renderer,&ind);
                    SDL_SetRenderDrawColor(renderer,255,255,255,255);SDL_RenderDrawRect(renderer,&ind);
                }
                SDL_SetRenderDrawColor(renderer,50,50,50,255);SDL_Rect bf={10,44,320,12};SDL_RenderFillRect(renderer,&bf);
                SDL_SetRenderDrawColor(renderer,255,200,0,255);SDL_Rect bp={10,44,(int)(320.0f*eliminados/N),12};SDL_RenderFillRect(renderer,&bp);
            }
            if(estado==JEFE){
                SDL_Rect rjefe={(int)jefeX,(int)jefeY,130,160};
                if(texJefe)SDL_RenderCopyEx(renderer,texJefe,nullptr,&rjefe,0,nullptr,jefeDir>0?SDL_FLIP_NONE:SDL_FLIP_HORIZONTAL);
                else{SDL_SetRenderDrawColor(renderer,200,0,200,255);SDL_RenderFillRect(renderer,&rjefe);}
                for(int i=0;i<MBJ;i++){
                    if(!bjActiva[i])continue;
                    SDL_SetRenderDrawColor(renderer,255,50,50,255);SDL_Rect rb={(int)bjX[i],615,18,8};SDL_RenderFillRect(renderer,&rb);
                    SDL_SetRenderDrawColor(renderer,255,150,0,255);SDL_Rect rb2={(int)bjX[i]+2,617,12,4};SDL_RenderFillRect(renderer,&rb2);
                }
                SDL_SetRenderDrawColor(renderer,80,0,0,255);SDL_Rect vf={400,20,576,20};SDL_RenderFillRect(renderer,&vf);
                SDL_SetRenderDrawColor(renderer,220,30,30,255);SDL_Rect vb={400,20,(int)(576.0f*vidaJefe/10),20};SDL_RenderFillRect(renderer,&vb);
                SDL_SetRenderDrawColor(renderer,255,100,100,255);SDL_RenderDrawRect(renderer,&vf);
                for(int i=0;i<5;i++){
                    SDL_SetRenderDrawColor(renderer,i<vidaJugador?255:60,i<vidaJugador?50:60,i<vidaJugador?50:60,255);
                    SDL_Rect c={10+i*36,10,28,28};SDL_RenderFillRect(renderer,&c);
                    SDL_SetRenderDrawColor(renderer,255,255,255,255);SDL_RenderDrawRect(renderer,&c);
                }
            }
        }

        if(estado==VICTORIA){
            SDL_SetRenderDrawColor(renderer,0,60,0,255);SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer,220,180,0,255);
            SDL_Rect m1={100,100,1176,568},m2={110,110,1156,548};SDL_RenderDrawRect(renderer,&m1);SDL_RenderDrawRect(renderer,&m2);
            SDL_SetRenderDrawColor(renderer,255,220,0,255);
            SDL_Rect y1={450,200,20,80};SDL_RenderFillRect(renderer,&y1);
            SDL_Rect y2={490,200,20,80};SDL_RenderFillRect(renderer,&y2);
            SDL_Rect y3={463,260,30,20};SDL_RenderFillRect(renderer,&y3);
            SDL_Rect y4={463,280,20,80};SDL_RenderFillRect(renderer,&y4);
            SDL_Rect o1={540,200,60,20};SDL_RenderFillRect(renderer,&o1);
            SDL_Rect o2={540,340,60,20};SDL_RenderFillRect(renderer,&o2);
            SDL_Rect o3={540,200,20,160};SDL_RenderFillRect(renderer,&o3);
            SDL_Rect o4={580,200,20,160};SDL_RenderFillRect(renderer,&o4);
            SDL_Rect u1={630,200,20,160};SDL_RenderFillRect(renderer,&u1);
            SDL_Rect u2={670,200,20,160};SDL_RenderFillRect(renderer,&u2);
            SDL_Rect u3={630,340,60,20};SDL_RenderFillRect(renderer,&u3);
            SDL_Rect w1={730,200,20,160};SDL_RenderFillRect(renderer,&w1);
            SDL_Rect w2={820,200,20,160};SDL_RenderFillRect(renderer,&w2);
            SDL_Rect w3={760,300,20,60};SDL_RenderFillRect(renderer,&w3);
            SDL_Rect w4={790,300,20,60};SDL_RenderFillRect(renderer,&w4);
            SDL_Rect ii={860,200,20,160};SDL_RenderFillRect(renderer,&ii);
            SDL_Rect n1={900,200,20,160};SDL_RenderFillRect(renderer,&n1);
            SDL_Rect n2={960,200,20,160};SDL_RenderFillRect(renderer,&n2);
            SDL_Rect n3={900,200,80,20};SDL_RenderFillRect(renderer,&n3);
            if((tick/25)%2==0){SDL_SetRenderDrawColor(renderer,180,180,180,255);SDL_Rect inst={488,500,400,8};SDL_RenderFillRect(renderer,&inst);}
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    if(texMapa)    SDL_DestroyTexture(texMapa);
    if(texMillon)  SDL_DestroyTexture(texMillon);
    if(texBuchona) SDL_DestroyTexture(texBuchona);
    if(texAlucin)  SDL_DestroyTexture(texAlucin);
    if(texPelucho) SDL_DestroyTexture(texPelucho);
    if(texEnemigo) SDL_DestroyTexture(texEnemigo);
    if(texJefe)    SDL_DestroyTexture(texJefe);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
