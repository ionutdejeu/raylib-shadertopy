/*******************************************************************************************
 *
 *   raylib [core] example - Basic window (adapted for HTML5 platform)
 *
 *   This example is prepared to compile for PLATFORM_WEB, PLATFORM_DESKTOP and PLATFORM_RPI
 *   As you will notice, code structure is slightly diferent to the other examples...
 *   To compile it for PLATFORM_WEB just uncomment #define PLATFORM_WEB at beginning
 *
 *   This example has been created using raylib 1.3 (www.raylib.com)
 *   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
 *
 *   Copyright (c) 2015 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include "../include/raylib.h"
#include "../include/raygui.h"
#include "ecs.h"
#include <stdio.h>
#include <stdlib.h>

//#define PLATFORM_WEB

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

 
//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
int screenWidth = 1000;
int screenHeight = 1000;
Texture2D building;
Texture2D treeTexture; 
Texture2D grassTexture;
Texture2D skyTexture;
// Use default vert shader
Shader shdrSpot;
//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
void UpdateDrawFrame(void); // Update and Draw one frame

// Spot data
typedef struct {
    Vector2 pos;
    Vector2 vel;
    float inner;
    float radius;

    // Shader locations
    unsigned int posLoc;
    unsigned int innerLoc;
    unsigned int radiusLoc;
} Spot;

Entity e;

Spot s;
System sysDraw;
System sysSprite;
Entity *sprites;
Entity *trees; 
Entity grass;
Entity sky; 
Music background_music;
Music cricketMusic;


void DrawSprite(Entity *e){
    DrawTexture(*e->components.componentSprite->texture, 
    e->components.componentPosition->x,
    e->components.componentPosition->y,
    WHITE);
} 
void DrawSystem(Entity *e){
    DrawRectangle(e->components.componentPosition->x,
      e->components.componentPosition->y,100,100,
      e->components.componentDraw->color);
}
//----------------------------------------------------------------------------------
// Main Enry Point
//----------------------------------------------------------------------------------
int main()
{
  // Initialization
  //--------------------------------------------------------------------------------------
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  //SetAudioStreamBufferSizeDefault(MAX_SAN);
  
  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
  InitAudioDevice();

  background_music = LoadMusicStream("../../resources/music.mp3");
  cricketMusic = LoadMusicStream("../../resources/crickets.mp3");

  building = LoadTexture("../../resources/building.png");
  treeTexture = LoadTexture("../../resources/tree.png");
  grassTexture = LoadTexture("../../resources/grass.png");


  shdrSpot = LoadShader(0, "../../resources/spotlight.fs");


  Music music = LoadMusicStream("../../resources/country.mp3");

  PlayMusicStream(music);

  //background_music.looping = true;
  //cricketMusic.looping = true;
  printf("\n Audio Device ready: %d",IsAudioDeviceReady());
  PlayMusicStream(background_music);
  //PlayMusicStream(cricketMusic);
  
  int i = 0;
  char posName[32] = "spots[x].pos\0";
  char innerName[32] = "spots[x].inner\0";
  char radiusName[32] = "spots[x].radius\0";

  posName[6] = '0' + i;
  innerName[6] = '0' + i;
  radiusName[6] = '0' + i;
  
  addComponentPosition(&e,200.01,100.22);
  addComponentDraw(&e,RED,1);


  trees = (Entity*) malloc(4*sizeof(Entity));
  rand();
  for(int i =0;i<4;i++){
      addSpriteComponent(&trees[i],&treeTexture,(Rectangle){0,0,100,100});
      addComponentPosition(&trees[i],GetRandomValue(200,500),GetRandomValue(100,800));
  }
  addSpriteComponent(&grass,&grassTexture,(Rectangle){0,0,100,100});
  addComponentPosition(&grass,0,500);
  

  sysDraw.update = &DrawSystem;
  sysSprite.update = &DrawSprite;

  s.posLoc = GetShaderLocation(shdrSpot, posName);
  s.innerLoc = GetShaderLocation(shdrSpot, innerName);
  s.radiusLoc = GetShaderLocation(shdrSpot, radiusName);

  // Tell the shader how wide the screen is so we can have
  // a pitch black half and a dimly lit half.
  unsigned int wLoc = GetShaderLocation(shdrSpot, "screenWidth");
  float sw = (float)screenWidth;
  SetShaderValue(shdrSpot, wLoc, &sw, SHADER_UNIFORM_FLOAT);
  s.pos.x = (float)GetRandomValue(64, screenWidth - 64);
  s.pos.y = (float)GetRandomValue(64, screenHeight - 64);
  s.vel = (Vector2){ 0, 0 };
  s.inner = 68.0f * (i + 1);
  s.radius = 98.0f * (i + 1);

  SetShaderValue(shdrSpot, s.posLoc, &s.pos.x, SHADER_UNIFORM_VEC2);
  SetShaderValue(shdrSpot, s.innerLoc, &s.inner, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shdrSpot, s.radiusLoc, &s.radius, SHADER_UNIFORM_FLOAT);
   
#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------
  
  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    UpdateDrawFrame();
  }
#endif

  UnloadTexture(building);
  UnloadShader(shdrSpot);
  UnloadMusicStream(background_music);
  UnloadMusicStream(cricketMusic);
  CloseAudioDevice();
  

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
void UpdateDrawFrame(void)
{
  // Update
  //----------------------------------------------------------------------------------
  // TODO: Update your variables here
  //----------------------------------------------------------------------------------

  // Draw
  //----------------------------------------------------------------------------------
  BeginDrawing();

  ClearBackground(GRAY);


  Vector2 mp = GetMousePosition();
  s.pos.x = mp.x;
  s.pos.y = screenHeight - mp.y;

  SetShaderValue(shdrSpot, s.posLoc, &s.pos.x, SHADER_UNIFORM_VEC2);
  
  //GuiGrid((Rectangle){0, 0, screenWidth, screenHeight}, 20.0f, 2); // Draw a fancy grid
  
  sysSprite.update(&grass);
  for(int i =0;i<4;i++){
      sysSprite.update(&trees[i]);
  }

  BeginShaderMode(shdrSpot);
      // Instead of a blank rectangle you could render here
      // a render texture of the full screen used to do screen
      // scaling (slight adjustment to shader would be required
      // to actually pay attention to the colour!)
      DrawRectangle(0, 0, screenWidth, screenHeight, WHITE);
  EndShaderMode();

  //DrawTexture(building, 0, 0, WHITE);
  //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
  //DrawRectangle(100, 100, 100, 300, RED);
  //DrawRectangleV((Vector2){300, 200}, (Vector2){100, 30}, BLUE);
  //sysDraw.update(&e);
  
  EndDrawing();
  //----------------------------------------------------------------------------------
}

