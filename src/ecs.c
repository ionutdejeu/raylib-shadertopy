#include "ecs.h"
#include <stdlib.h>

void addComponentPosition(Entity *entity, int x, int y) {
entity -> components.componentPosition = malloc(sizeof(ComponentTransform));
entity -> components.componentPosition -> x = x;
entity -> components.componentPosition -> y = y;
entity -> components.componentPosition -> rotation = 0;
entity -> components.componentPosition ->scaleX = 1;
entity -> components.componentPosition ->scaleY = 1;
}
 
void addComponentDraw(Entity *entity, Color color, int size) {
entity -> components.componentDraw = malloc(sizeof(ComponentDraw));
entity -> components.componentDraw -> color = color;
entity -> components.componentDraw -> size = size;
}

void addSpriteComponent(Entity *e,Texture2D *tex,Rectangle rect){
    e -> components.componentSprite = malloc(sizeof(ComponentSprite));
    e -> components.componentSprite ->drawArea = rect;
    e -> components.componentSprite->texture =tex;
}