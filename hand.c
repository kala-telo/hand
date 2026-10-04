#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <assert.h>
#include <stdio.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *hand = NULL;
static SDL_Texture *hand_press = NULL;

static bool pressed = false;
static bool alt_pressed = false;

// https://gist.github.com/dele256/901dd1e8f920327fc457a538996f2a29
SDL_HitTestResult hittest(SDL_Window *win, const SDL_Point *area, void *data) {
    if (!alt_pressed) return SDL_HITTEST_NORMAL;
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    const int pad = 10;
    if (area->y < pad)
        if (area->x < pad)
            return SDL_HITTEST_RESIZE_TOPLEFT;
        else if (area->x > w - pad)
            return SDL_HITTEST_RESIZE_TOPRIGHT;
        else
            return SDL_HITTEST_RESIZE_TOP;
    else if (area->y > h - pad)
        if (area->x < pad)
            return SDL_HITTEST_RESIZE_BOTTOMLEFT;
        else if (area->x > w - pad)
            return SDL_HITTEST_RESIZE_BOTTOMRIGHT;
        else
            return SDL_HITTEST_RESIZE_BOTTOM;
    else if (area->x < pad)
        return SDL_HITTEST_RESIZE_LEFT;
    else if (area->x > w - pad)
        return SDL_HITTEST_RESIZE_RIGHT;
    return SDL_HITTEST_DRAGGABLE;
}

SDL_AppResult
SDL_AppInit(void **appstate, int argc, char *argv[])
{
    static uint8_t hand_png[] = {
        #embed "hand.png"
    };
    static uint8_t hand_press_png[] = {
        #embed "hand_press.png"
    };
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("hand", 76, 304, SDL_WINDOW_TRANSPARENT|SDL_WINDOW_RESIZABLE|SDL_WINDOW_ALWAYS_ON_TOP|SDL_WINDOW_BORDERLESS|SDL_WINDOW_UTILITY, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    if(!SDL_SetRenderVSync(renderer, 1))
    {
        SDL_Log("Could not enable VSync! SDL error: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_IOStream *hand_io = SDL_IOFromConstMem(hand_png, sizeof(hand_png));
    SDL_Surface *hand_surf = SDL_LoadPNG_IO(hand_io, true);
    hand = SDL_CreateTextureFromSurface(renderer, hand_surf);
    SDL_DestroySurface(hand_surf);
    SDL_IOStream *hand_press_io = SDL_IOFromConstMem(hand_press_png, sizeof(hand_press_png));
    SDL_Surface *hand_press_surf = SDL_LoadPNG_IO(hand_press_io, true);
    hand_press = SDL_CreateTextureFromSurface(renderer, hand_press_surf);
    SDL_DestroySurface(hand_press_surf);

    SDL_SetWindowHitTest(window, hittest, NULL);

    return SDL_APP_CONTINUE;
}

SDL_AppResult
SDL_AppEvent(void *appstate, SDL_Event *event)
{
    bool bordered;
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        pressed = true;
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        pressed = false;
        break;
    case SDL_EVENT_KEY_DOWN:
        if (event->key.key == SDLK_ESCAPE)
            return SDL_APP_SUCCESS;
        alt_pressed = (event->key.mod & SDL_KMOD_ALT) != 0;
        break;
    case SDL_EVENT_KEY_UP:
        alt_pressed = (event->key.mod & SDL_KMOD_ALT) != 0;
        break;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult
SDL_AppIterate(void *appstate)
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, pressed ? hand_press : hand, NULL, NULL); 
    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

void
SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}

