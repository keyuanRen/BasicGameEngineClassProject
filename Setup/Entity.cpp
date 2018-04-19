#include "Entity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
}

void Entity::setRenderer(SDL_Renderer * renderer)
{
	this->renderer = renderer;
}

void Entity::setXY(float x, float y)
{
	this->x = x;
	this->y = y;
}

void Entity::update(float)
{
	//do nothing, unless subclasses do somthing with it
}

void Entity::draw()
{
	//also do nothing for you
}
