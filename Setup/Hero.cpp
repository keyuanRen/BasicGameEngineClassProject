#include "Hero.h"

Hero::Hero()
{
}

Hero::~Hero()
{
}

void Hero::setAnimation(Animation * animation)
{
	this->animation = animation;
}

void Hero::update(float dt)
{
	//if (faceRight)
	//{
	//	//x += dt*800;
	//	velocity.x = 100;
	//}
	//else
	//{
	//	//x -= dt*800;
	//	velocity.x = -100;
	//}

	//turn around if at edages of our screen (before handler)
	/*if (pos.x >= 640)
		faceRight = false;
	if (pos.x <= 0)
		faceRight = true;*/
	//after using handler   face direction based on velocity.x value
	if (velocity.x > 0)
		faceRight = true;
	if (velocity.x < 0)
		faceRight = false;

	// hit left wall show up at right wall
	if (pos.x < 0) pos.x = 640 - 32;
	//top
	if (pos.y < 0) pos.y = 480- 32;
	//right
	if (pos.x > 640-32) pos.x = 0;
	//button
	if (pos.y > 480-32) pos.y = 0;


	//use basic entity movement to move around
	updateMovement(dt);

	//update animations too
	animation->update(dt);
}

void Hero::draw()
{
	if (animation != NULL)
	{
		if (faceRight)
			animation->draw(pos.x, pos.y);
		else
			animation->draw(pos.x, pos.y, true);
	}
}
