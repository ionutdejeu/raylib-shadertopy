#pragma once
#include "../include/raylib.h"

typedef struct {
float x;
float y;
float rotation;
float scaleX;
float scaleY;
} ComponentTransform;
 
typedef struct {
Color color;
int size;
} ComponentDraw;

typedef struct { 
    Sound s;
} ComponentSound;

typedef struct { 
    Texture2D *texture;
    Rectangle drawArea;
} ComponentSprite;

typedef struct {

ComponentTransform *componentPosition;
ComponentDraw *componentDraw;
ComponentSprite *componentSprite;

} Components;

typedef struct {
unsigned int uuid;
Components components;
} Entity;

typedef void (*ecs_system_update_func)(Entity *e);
typedef void (*ecs_entity_distroy)(Entity *e);

typedef struct {
    ecs_system_update_func update;
    Components *components;
    int componentsCount;
}System;



void addComponentPosition(Entity *entity, int x, int y);
void addComponentDraw(Entity *entity, Color color, int size);
void addSpriteComponent(Entity *e,Texture2D *tex,Rectangle rect);
void freeEntity(Entity *entity);

