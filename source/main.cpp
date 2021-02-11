#include <nds.h>
#include <gl2d.h>
#include <stdio.h>
#include "Input.h"
#include <nf_2d.h>
#include <nf_lib.h>
#include <fstream>

struct Vector2D {
    int X = 0;
    int Y = 0;
};

class Object {
    public:
        Vector2D Position;
        Vector2D Velocity;
        virtual void Init();
        virtual void Update();
        virtual void Draw();
};

class Player : public Object {

    public:
        void Init() {
            fprintf(stderr, "Initializing Player...");
            Position.X = 0;
            Position.Y = 0;
            Velocity.X = 2;
            Velocity.Y = 2;
        }

        void Update() {
            scanKeys();

            if (keysHeld() & KEY_UP) { Position.Y -= Velocity.Y; }
            else if (keysHeld() & KEY_DOWN) { Position.Y += Velocity.Y; }
            
            if (keysHeld() & KEY_LEFT) { Position.X -= Velocity.X; }
            else if (keysHeld() & KEY_RIGHT) { Position.X += Velocity.X; }

            //fprintf(stderr, "In Update (Player).");
        }

        void Draw() {
            glBoxFilled(Position.X, Position.Y, Position.X + 16, Position.Y + 32, RGB15(0, 255, 0));
            //fprintf(stderr, "In Draw (Player).");
        }

};


int main(void) {
    //videoSetMode(MODE_5_3D);
    //consoleDebugInit(DebugDevice_NOCASH);
    consoleDemoInit(); //Console Initializer on Bottom Screen.

    NF_SetRootFolder("NITROFS"); //Setting the root folder to NITROFS for some reason.

    NF_Set2D(0, 0); //Initializing Top Screen.
    NF_Set2D(1, 0); //Initializing Bottom Screen.

    NF_InitTiledBgBuffers(); //Initializing BG buffers.
    NF_InitTiledBgSys(0);    //Initializing BG Sys on top screen.
    NF_InitTiledBgSys(1);    //Initializing BG Sys on bottom screen.

    //Load Background.
    NF_LoadTiledBg("bg/bg_01", "bg_01", 256, 256); //("Path_Of_File_Without_Extension", "Name_Of_BG", "Width_of_BG", "Height_Of_BG") 
    NF_CreateTiledBg(0, 0, "bg_01"); // (Screen (0 - 1), Layer (0 - 3), "Name_Of_BG")
    NF_CreateTiledBg(1, 0, "bg_01"); // (Screen (0 - 1), Layer (0 - 3), "Name_Of_BG")

    //Load Sprite (Trying to figure this out by myself)

    NF_InitSpriteBuffers(); //Initializing Sprite Buffers.
    NF_InitSpriteSys(0);    //Initializing Sprite Sys on top screen.

    //Load the sprite.
    NF_LoadSpriteGfx("spr/spr_player", 0, 16, 32); //("Path_Of_File_Without_Extension", Slot Number (0 - 63), "Width_Of_Sprite", "Height_Of_Sprite") 
    NF_LoadSpritePal("spr/bw", 0); //("Path_Of_File(pal)_Without_Extension", Slot Number (0 - 63))
    
    //Transfer the sprite to VRAM.
    NF_VramSpriteGfx(0, 0, 0, false); //(Screen (0 - 1), RAM Slot (0 - 255), VRAM Slot (0 - 127), Keep Frames? (true - false))
    NF_VramSpritePal(0, 0, 0); //(Screen (0 - 1), RAM Slot of palette (0 - 64), VRAM Slot of palette (0 - 15))

    //Create the sprite.
    NF_CreateSprite(0, 0, 0, 0, 0, 0); //(Screen, Sprite ID, GFX Slot, Palette slot, X pos, Y pos)

    //sprintf("\n Hello World!");

    Player player;

    player.Init();

    while (1) {

        player.Update();
        NF_MoveSprite(0, 0, player.Position.X, player.Position.Y);

        NF_SpriteOamSet(0);
        swiWaitForVBlank();
        oamUpdate(&oamMain);
    }

    return 0;

}