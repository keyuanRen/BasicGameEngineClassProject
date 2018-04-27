#pragma once

#include <SDL.h>
#include"Vector.h"

class Entity
{
protected:
	SDL_Renderer * renderer;
	

public:
	Entity();
	~Entity();

	Vector pos;//our entities x y position
	Vector velocity; //how fast to move in my direction

	void setRenderer(SDL_Renderer* renderer);
	
	//void setX(float x);
	//void setY(float y);
	//void setXY(float x, float y);

	Vector getPosition();
	void setPosition(Vector pos);

	Vector getVelocity();
	void setVelocity(Vector velocity);

	virtual void update(float);
	virtual void updateMovement(float dt);
	virtual void draw();
};

