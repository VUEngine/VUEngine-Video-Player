/* VUEngine - Virtual Utopia Engine <http://vuengine.planetvb.com/>
 * A universal game engine for the Nintendo Virtual Boy
 *
 * Copyright (C) 2007, 2018 by Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <chris@vr32.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
 * NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


//---------------------------------------------------------------------------------------------------------
// 												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <string.h>

#include <Game.h>
#include <Camera.h>
#include <Printing.h>
#include <VIPManager.h>
#include <VideoPlayerState.h>
#include <KeypadManager.h>
#include <MessageDispatcher.h>


//---------------------------------------------------------------------------------------------------------
// 												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMDef VIDEO_ST;


//---------------------------------------------------------------------------------------------------------
// 												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

void VideoPlayerState::constructor()
{
	Base::constructor();

	// init members
	this->videoEntity = NULL;
	this->progressBarEntity = NULL;
	this->playEntity = NULL;
	this->videoPlaying = true;
	this->guiVisible = false;
	this->numberOfFrames = 0;
	this->hiColorMode = true;
}

void VideoPlayerState::destructor()
{
	// destroy base
	Base::destructor();
}

void VideoPlayerState::enter(void* owner __attribute__ ((unused)))
{
	// call base
	Base::enter(this, owner);

	// disable user input
    Game::disableKeypad(Game::getInstance());

	// load stage
	GameState::loadStage(GameState::safeCast(this), (StageDefinition*)&VIDEO_ST, NULL, true);

	// get entities from stage
	this->videoEntity = AnimatedEntity::safeCast(Container::getChildByName(
		Container::safeCast(Game::getStage(Game::getInstance())),
		"VideoEnt",
		false
	));
	this->progressBarEntity = Entity::safeCast(Container::getChildByName(
		Container::safeCast(Game::getStage(Game::getInstance())),
		"ProgrEnt",
		false
	));
	this->playEntity = AnimatedEntity::safeCast(Container::getChildByName(
		Container::safeCast(Game::getStage(Game::getInstance())),
		"PlayEnt",
		false
	));

	this->numberOfFrames = AnimatedEntity::getNumberOfFrames(this->videoEntity);
	this->numberOfFrames >>= 1;

	// initially hide gui
	VideoPlayerState::hideGui(this);

	// start clocks to start animations
	GameState::startClocks(GameState::safeCast(this));

	// start fade in effect
	Camera::startEffect(Camera::getInstance(),
		kFadeTo, // effect type
		0, // initial delay (in ms)
		NULL, // target brightness
		0, // delay between fading steps (in ms)
		(void (*)(Object, Object))VideoPlayerState::onFadeInComplete, // callback function
		Object::safeCast(this) // callback scope
	);
}

void VideoPlayerState::execute(void* owner)
{
	Base::execute(this, owner);

	// refresh GUI if the video is playing
	if(this->guiVisible && this->videoPlaying)
	{
		VideoPlayerState::printFrames(this);
		VideoPlayerState::printProgress(this);
	}

	// if the video is paused in hicolor mode, make sure to alternate the two frames that make the hi-color image
	if(!this->videoPlaying && this->hiColorMode)
	{
		int currentFrame = AnimatedEntity::getActualFrame(this->videoEntity);

		if(currentFrame & 1)
		{
			AnimatedEntity::previousFrame(AnimatedEntity::safeCast(this->videoEntity));
		}
		else
		{
			AnimatedEntity::nextFrame(AnimatedEntity::safeCast(this->videoEntity));
		}
	}
}

void VideoPlayerState::processUserInput(UserInput userInput)
{
	if(
		(userInput.pressedKey & K_LR) || ((userInput.holdKey & K_LR) && (userInput.holdKeyDuration > 12)) ||
		(userInput.pressedKey & K_RR) || ((userInput.holdKey & K_RR) && (userInput.holdKeyDuration > 12)) ||
		(userInput.pressedKey & K_RT) || ((userInput.holdKey & K_RT) && (userInput.holdKeyDuration > 12))
	)
	{
		// pause animation
		if(this->videoPlaying)
		{
			VideoPlayerState::pauseVideo(this);
			AnimatedEntity::playAnimation(AnimatedEntity::safeCast(this->playEntity), "Pause");
			this->videoPlaying = false;
			VideoPlayerState::showGui(this);
		}

		// show next frame
		AnimatedEntity::nextFrame(AnimatedEntity::safeCast(this->videoEntity));
		AnimatedEntity::nextFrame(AnimatedEntity::safeCast(this->videoEntity));

		VideoPlayerState::printFrames(this);
		VideoPlayerState::printProgress(this);
	}
	else if(
		(userInput.pressedKey & K_LL) || ((userInput.holdKey & K_LL) && (userInput.holdKeyDuration > 12)) ||
		(userInput.pressedKey & K_RL) || ((userInput.holdKey & K_RL) && (userInput.holdKeyDuration > 12)) ||
		(userInput.pressedKey & K_LT) || ((userInput.holdKey & K_LT) && (userInput.holdKeyDuration > 12))
	)
	{
		// pause animation
		if(this->videoPlaying)
		{
			VideoPlayerState::pauseVideo(this);
			AnimatedEntity::playAnimation(AnimatedEntity::safeCast(this->playEntity), "Pause");
			this->videoPlaying = false;
			VideoPlayerState::showGui(this);
		}

		// show previous frame
		AnimatedEntity::previousFrame(AnimatedEntity::safeCast(this->videoEntity));
		AnimatedEntity::previousFrame(AnimatedEntity::safeCast(this->videoEntity));

		VideoPlayerState::printFrames(this);
		VideoPlayerState::printProgress(this);
	}
	else if(userInput.pressedKey & K_STA)
	{
		// pause/resume animation
		if(this->videoPlaying)
		{
			VideoPlayerState::pauseVideo(this);
		}
		else
		{
			AnimatedEntity::pauseAnimation(AnimatedEntity::safeCast(this->videoEntity), false);
		}
		this->videoPlaying = !this->videoPlaying;
		if(this->videoPlaying)
		{
			AnimatedEntity::playAnimation(AnimatedEntity::safeCast(this->playEntity), "Play");
			VideoPlayerState::hideGui(this);
		}
		else
		{
			AnimatedEntity::playAnimation(AnimatedEntity::safeCast(this->playEntity), "Pause");
			VideoPlayerState::showGui(this);
		}
	}
	else if(userInput.pressedKey & K_SEL)
	{
		if(this->videoPlaying)
		{
			if(this->guiVisible)
			{
				VideoPlayerState::hideGui(this);
			}
			else
			{
				VideoPlayerState::showGui(this);
			}
		}
	}
	else if(userInput.pressedKey & K_A)
	{
		this->hiColorMode = !this->hiColorMode;
		int currentFrame = AnimatedEntity::getActualFrame(this->videoEntity);
		AnimatedEntity::playAnimation(AnimatedEntity::safeCast(this->videoEntity), this->hiColorMode ? "HiColor" : "4Color");
		AnimatedEntity::setActualFrame(this->videoEntity, currentFrame);
		AnimatedEntity::pauseAnimation(AnimatedEntity::safeCast(this->videoEntity), !this->videoPlaying);
	}
}

void VideoPlayerState::showGui()
{
	// entities
	Entity::show(this->progressBarEntity);
	Entity::show(this->playEntity);

	// frame counter
	VideoPlayerState::printFrames(this);

	// progress bar
	VideoPlayerState::printProgress(this);

	this->guiVisible = true;
}

void VideoPlayerState::hideGui()
{
	// entities
	Entity::hide(this->progressBarEntity);
	Entity::hide(this->playEntity);

	// frame counter
	Printing::text(Printing::getInstance(), "...", 44, 25, NULL);
	Printing::text(Printing::getInstance(), "....", 43, 26, NULL);

	// progress bar
	Printing::text(Printing::getInstance(), ".......................................", 3, 25, NULL);
	Printing::text(Printing::getInstance(), ".......................................", 3, 26, NULL);

	this->guiVisible = false;
}

void VideoPlayerState::printFrames()
{
	int currentFrame = AnimatedEntity::getActualFrame(this->videoEntity);
	currentFrame >>= 1;
	currentFrame += 1;

	if(currentFrame >= 100)
	{
		Printing::int(Printing::getInstance(), currentFrame, 44, 25, NULL);
	}
	else if(currentFrame >= 10)
	{
		Printing::text(Printing::getInstance(), "0", 44, 25, NULL);
		Printing::int(Printing::getInstance(), currentFrame, 45, 25, NULL);
	}
	else
	{
		Printing::text(Printing::getInstance(), "00", 44, 25, NULL);
		Printing::int(Printing::getInstance(), currentFrame, 46, 25, NULL);
	}

	if(this->numberOfFrames >= 100)
	{
		Printing::text(Printing::getInstance(), "/", 43, 26, NULL);
		Printing::int(Printing::getInstance(), this->numberOfFrames, 44, 26, NULL);
	}
	else if(this->numberOfFrames >= 10)
	{
		Printing::text(Printing::getInstance(), "/0", 43, 26, NULL);
		Printing::int(Printing::getInstance(), this->numberOfFrames, 45, 26, NULL);
	}
	else
	{
		Printing::text(Printing::getInstance(), "/00", 43, 26, NULL);
		Printing::int(Printing::getInstance(), this->numberOfFrames, 46, 26, NULL);
	}
}

void VideoPlayerState::pauseVideo()
{
	AnimatedEntity::pauseAnimation(AnimatedEntity::safeCast(this->videoEntity), true);

	int currentFrame = AnimatedEntity::getActualFrame(this->videoEntity);

	// if currentFrame is odd, go to previous frame (4 color frame)
	if(currentFrame & 1)
	{
		AnimatedEntity::previousFrame(AnimatedEntity::safeCast(this->videoEntity));
	}
}

void VideoPlayerState::printProgress()
{
	int currentFrame = AnimatedEntity::getActualFrame(this->videoEntity);
	currentFrame >>= 1;

	int progress = (int)((float)currentFrame / (float)this->numberOfFrames * 78);

	switch(progress)
	{
		case 0:
		case 1:
			Printing::text(Printing::getInstance(), ".......................................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ".......................................", 3, 26, NULL);
			break;

		case 2:
			Printing::text(Printing::getInstance(), ";......................................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ";......................................", 3, 26, NULL);
			break;

		case 3:
			Printing::text(Printing::getInstance(), ":......................................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":......................................", 3, 26, NULL);
			break;

		case 4:
			Printing::text(Printing::getInstance(), ":;.....................................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":;.....................................", 3, 26, NULL);
			break;

		case 5:
			Printing::text(Printing::getInstance(), "::.....................................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::.....................................", 3, 26, NULL);
			break;

		case 6:
			Printing::text(Printing::getInstance(), "::;....................................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::;....................................", 3, 26, NULL);
			break;

		case 7:
			Printing::text(Printing::getInstance(), ":::....................................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::....................................", 3, 26, NULL);
			break;

		case 8:
			Printing::text(Printing::getInstance(), ":::;...................................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::;...................................", 3, 26, NULL);
			break;

		case 9:
			Printing::text(Printing::getInstance(), "::::...................................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::...................................", 3, 26, NULL);
			break;

		case 10:
			Printing::text(Printing::getInstance(), "::::;..................................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::;..................................", 3, 26, NULL);
			break;

		case 11:
			Printing::text(Printing::getInstance(), ":::::..................................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::..................................", 3, 26, NULL);
			break;

		case 12:
			Printing::text(Printing::getInstance(), ":::::;.................................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::;.................................", 3, 26, NULL);
			break;

		case 13:
			Printing::text(Printing::getInstance(), "::::::.................................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::.................................", 3, 26, NULL);
			break;

		case 14:
			Printing::text(Printing::getInstance(), "::::::;................................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::;................................", 3, 26, NULL);
			break;

		case 15:
			Printing::text(Printing::getInstance(), ":::::::................................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::................................", 3, 26, NULL);
			break;

		case 16:
			Printing::text(Printing::getInstance(), ":::::::;...............................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::;...............................", 3, 26, NULL);
			break;

		case 17:
			Printing::text(Printing::getInstance(), "::::::::...............................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::...............................", 3, 26, NULL);
			break;

		case 18:
			Printing::text(Printing::getInstance(), "::::::::;..............................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::;..............................", 3, 26, NULL);
			break;

		case 19:
			Printing::text(Printing::getInstance(), ":::::::::..............................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::..............................", 3, 26, NULL);
			break;

		case 20:
			Printing::text(Printing::getInstance(), ":::::::::;.............................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::;.............................", 3, 26, NULL);
			break;

		case 21:
			Printing::text(Printing::getInstance(), "::::::::::.............................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::.............................", 3, 26, NULL);
			break;

		case 22:
			Printing::text(Printing::getInstance(), "::::::::::;............................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::;............................", 3, 26, NULL);
			break;

		case 23:
			Printing::text(Printing::getInstance(), ":::::::::::;...........................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::;...........................", 3, 26, NULL);
			break;

		case 24:
			Printing::text(Printing::getInstance(), "::::::::::::...........................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::...........................", 3, 26, NULL);
			break;

		case 25:
			Printing::text(Printing::getInstance(), "::::::::::::;..........................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::;..........................", 3, 26, NULL);
			break;

		case 26:
			Printing::text(Printing::getInstance(), ":::::::::::::..........................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::..........................", 3, 26, NULL);
			break;

		case 27:
			Printing::text(Printing::getInstance(), ":::::::::::::;.........................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::;.........................", 3, 26, NULL);
			break;

		case 28:
			Printing::text(Printing::getInstance(), "::::::::::::::.........................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::.........................", 3, 26, NULL);
			break;

		case 29:
			Printing::text(Printing::getInstance(), "::::::::::::::;........................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::;........................", 3, 26, NULL);
			break;

		case 30:
			Printing::text(Printing::getInstance(), ":::::::::::::::........................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::........................", 3, 26, NULL);
			break;

		case 31:
			Printing::text(Printing::getInstance(), ":::::::::::::::;.......................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::;.......................", 3, 26, NULL);
			break;

		case 32:
			Printing::text(Printing::getInstance(), "::::::::::::::::.......................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::::.......................", 3, 26, NULL);
			break;

		case 33:
			Printing::text(Printing::getInstance(), "::::::::::::::::;......................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::::;......................", 3, 26, NULL);
			break;

		case 34:
			Printing::text(Printing::getInstance(), ":::::::::::::::::......................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::::......................", 3, 26, NULL);
			break;

		case 35:
			Printing::text(Printing::getInstance(), ":::::::::::::::::;.....................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::::;.....................", 3, 26, NULL);
			break;

		case 36:
			Printing::text(Printing::getInstance(), "::::::::::::::::::.....................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::::::.....................", 3, 26, NULL);
			break;

		case 37:
			Printing::text(Printing::getInstance(), "::::::::::::::::::;....................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::::::;....................", 3, 26, NULL);
			break;

		case 38:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::....................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::::::....................", 3, 26, NULL);
			break;

		case 39:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::;...................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::::::;...................", 3, 26, NULL);
			break;

		case 40:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::...................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::::::::...................", 3, 26, NULL);
			break;

		case 41:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::;..................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::::::::;..................", 3, 26, NULL);
			break;

		case 42:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::..................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::..................", 3, 26, NULL);
			break;

		case 43:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::;.................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::;.................", 3, 26, NULL);
			break;

		case 44:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::.................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::.................", 3, 26, NULL);
			break;

		case 45:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::;................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::;................", 3, 26, NULL);
			break;

		case 46:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::................", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::................", 3, 26, NULL);
			break;

		case 47:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::;...............", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::;...............", 3, 26, NULL);
			break;

		case 48:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::...............", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::...............", 3, 26, NULL);
			break;

		case 49:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::;..............", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::;..............", 3, 26, NULL);
			break;

		case 50:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::..............", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::..............", 3, 26, NULL);
			break;

		case 51:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::;.............", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::;.............", 3, 26, NULL);
			break;

		case 52:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::.............", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::.............", 3, 26, NULL);
			break;

		case 53:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::;............", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::;............", 3, 26, NULL);
			break;

		case 54:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::............", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::............", 3, 26, NULL);
			break;

		case 55:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::;...........", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::;...........", 3, 26, NULL);
			break;

		case 56:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::...........", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::...........", 3, 26, NULL);
			break;

		case 57:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::;..........", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::;..........", 3, 26, NULL);
			break;

		case 58:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::..........", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::..........", 3, 26, NULL);
			break;

		case 59:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::;.........", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::;.........", 3, 26, NULL);
			break;

		case 60:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::.........", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::.........", 3, 26, NULL);
			break;

		case 61:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::;........", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::;........", 3, 26, NULL);
			break;

		case 62:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::........", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::........", 3, 26, NULL);
			break;

		case 63:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::;.......", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::;.......", 3, 26, NULL);
			break;

		case 64:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::.......", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::.......", 3, 26, NULL);
			break;

		case 65:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::;......", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::;......", 3, 26, NULL);
			break;

		case 66:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::......", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::......", 3, 26, NULL);
			break;

		case 67:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::;.....", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::;.....", 3, 26, NULL);
			break;

		case 68:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::::.....", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::::.....", 3, 26, NULL);
			break;

		case 69:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::::;....", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::::;....", 3, 26, NULL);
			break;

		case 70:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::::....", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::::....", 3, 26, NULL);
			break;

		case 71:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::::;...", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::::;...", 3, 26, NULL);
			break;

		case 72:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::::::...", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::::::...", 3, 26, NULL);
			break;

		case 73:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::::::;..", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::::::;..", 3, 26, NULL);
			break;

		case 74:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::::::..", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::::::..", 3, 26, NULL);
			break;

		case 75:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::::::;.", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::::::;.", 3, 26, NULL);
			break;

		case 76:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::::::::.", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::::::::.", 3, 26, NULL);
			break;

		case 77:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::::::::;", 3, 25, NULL);
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::::::::;", 3, 26, NULL);
			break;

		case 78:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::::::::", 3, 25, NULL);
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::::::::", 3, 26, NULL);
			break;
	}
}

// handle event
void VideoPlayerState::onFadeInComplete(Object eventFirer __attribute__ ((unused)))
{
	// enable user input
	Game::enableKeypad(Game::getInstance());
}
