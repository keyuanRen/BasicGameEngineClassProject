#include "GameControllerHandler.h"

GameControllerHandler::GameControllerHandler()
{
	//see if any joysticks )(joystickm gamepad, other input device) are connected
	int numJoysticks = SDL_NumJoysticks();

	//connected joystrick ids start at 0 and go up
	for (int joystickId = 0; joystickId < numJoysticks; joystickId++)
	{
		//check to see if joystick is classified as a game controller(e.g not GCon gun or steering wheel, but a ps4 or xbox controller)
		if (SDL_IsGameController(joystickId))
		{
			//if it is, good, lets use this
			controller = SDL_GameControllerOpen(joystickId);
			//only using first controller, so we'll exit the loop for now
			break;
			//if you want to have multiple controllers connected, you will need to open them aswell, so this code will need changing
		}
	}
}


GameControllerHandler::~GameControllerHandler()
{
	//close off controller if we have one
	if (controller != NULL)
		SDL_GameControllerClose(controller);
}

void GameControllerHandler::update(SDL_Event* event)
{
	//if controller not null and 'opened'
	if (controller != NULL && SDL_GameControllerGetAttached(controller))
	{
		//make sure we're only checking for this controller // get id of out controller
		SDL_JoystickID joystickId = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));
		//check to see if this sdl_event is with this joystickId
		if (event->cdevice.which == joystickId)
		{
			//if controller removed  :0
			if (event->type == SDL_CONTROLLERDEVICEREMOVED)
			{
				//close controller and set back to null
				SDL_GameControllerClose(controller);
				controller = NULL;
				return; // exit the rest of function, because no controller means no fun
			}

			//if it is, good, lets's check button next 
			if (event->type == SDL_CONTROLLERBUTTONDOWN)
			{
				//check individual buttons
				if (event->cbutton.button == SDL_CONTROLLER_BUTTON_A)
				{
					//A button to reset position
					hero->pos.x = 200;
					hero->pos.y = 200;
				}
				//add B button etc next
			}
		}

		//check left stick x axis
		Sint16 axisX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
		Sint16 axisY = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
		//we should cater for stick 'deadzone', because resting values in the middle wont always be zero
		int deadzone = 10000;

		//check for moving left either by axisX < -deadzone OR we'll also check the DPad left button being held down
		if (axisX < -deadzone || SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT))
			hero->velocity.x = -80;
		//right
		if (axisX > deadzone || SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
			hero->velocity.x = 80;
		//up
		if (axisY < -deadzone || SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP))
			hero->velocity.y = -80;
		//down
		if (axisY > deadzone || SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN))
			hero->velocity.y = 80;

		//check to see if we're not moving left or right
		if (axisX < deadzone && axisX > -deadzone
			&& !SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT)
			&& !SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
		{
			hero->velocity.x = 0;
		}
		//check to see if we're not moving up or down
		if (axisY < deadzone && axisY > -deadzone
			&& !SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP)
			&& !SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN))
		{
			hero->velocity.y = 0;
		}

	}
}
