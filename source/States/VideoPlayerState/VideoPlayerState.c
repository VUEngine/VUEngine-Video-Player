/**
 * VUEngine Video Player
 *
 * © Christian Radke and Marten Reiß
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */


//---------------------------------------------------------------------------------------------------------
// 												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <string.h>

#include <Camera.h>
#include <CameraEffectManager.h>
#include <KeypadManager.h>
#include <MessageDispatcher.h>
#include <Printing.h>
#include <VideoPlayerState.h>
#include <VIPManager.h>
#include <VUEngine.h>


//---------------------------------------------------------------------------------------------------------
// 												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMSpec VideoStage;


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
	VUEngine::disableKeypad(VUEngine::getInstance());

	// load stage
	GameState::loadStage(GameState::safeCast(this), (StageSpec*)&VideoStage, NULL, true, false);

	// get entities from stage
	this->videoEntity = AnimatedEntity::safeCast(Container::getChildByName(
		Container::safeCast(VUEngine::getStage(VUEngine::getInstance())),
		"VideoEnt",
		false
	));
	this->progressBarEntity = Entity::safeCast(Container::getChildByName(
		Container::safeCast(VUEngine::getStage(VUEngine::getInstance())),
		"ProgrEnt",
		false
	));
	this->playEntity = AnimatedEntity::safeCast(Container::getChildByName(
		Container::safeCast(VUEngine::getStage(VUEngine::getInstance())),
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
	Printing::text(Printing::getInstance(), "...", 44, 25, "Number");
	Printing::text(Printing::getInstance(), "....", 43, 26, "Number");

	// progress bar
	Printing::text(Printing::getInstance(), ".......................................", 3, 25, "Number");
	Printing::text(Printing::getInstance(), ".......................................", 3, 26, "Number");

	this->guiVisible = false;
}

void VideoPlayerState::printFrames()
{
	int currentFrame = AnimatedEntity::getActualFrame(this->videoEntity);
	currentFrame >>= 1;
	currentFrame += 1;

	if(currentFrame >= 100)
	{
		Printing::int32(Printing::getInstance(), currentFrame, 44, 25, "Number");
	}
	else if(currentFrame >= 10)
	{
		Printing::text(Printing::getInstance(), "0", 44, 25, "Number");
		Printing::int32(Printing::getInstance(), currentFrame, 45, 25, "Number");
	}
	else
	{
		Printing::text(Printing::getInstance(), "00", 44, 25, "Number");
		Printing::int32(Printing::getInstance(), currentFrame, 46, 25, "Number");
	}

	if(this->numberOfFrames >= 100)
	{
		Printing::text(Printing::getInstance(), "/", 43, 26, "Number");
		Printing::int32(Printing::getInstance(), this->numberOfFrames, 44, 26, "Number");
	}
	else if(this->numberOfFrames >= 10)
	{
		Printing::text(Printing::getInstance(), "/0", 43, 26, "Number");
		Printing::int32(Printing::getInstance(), this->numberOfFrames, 45, 26, "Number");
	}
	else
	{
		Printing::text(Printing::getInstance(), "/00", 43, 26, "Number");
		Printing::int32(Printing::getInstance(), this->numberOfFrames, 46, 26, "Number");
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
			Printing::text(Printing::getInstance(), ".......................................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ".......................................", 3, 26, "Number");
			break;

		case 2:
			Printing::text(Printing::getInstance(), ";......................................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ";......................................", 3, 26, "Number");
			break;

		case 3:
			Printing::text(Printing::getInstance(), ":......................................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":......................................", 3, 26, "Number");
			break;

		case 4:
			Printing::text(Printing::getInstance(), ":;.....................................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":;.....................................", 3, 26, "Number");
			break;

		case 5:
			Printing::text(Printing::getInstance(), "::.....................................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::.....................................", 3, 26, "Number");
			break;

		case 6:
			Printing::text(Printing::getInstance(), "::;....................................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::;....................................", 3, 26, "Number");
			break;

		case 7:
			Printing::text(Printing::getInstance(), ":::....................................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::....................................", 3, 26, "Number");
			break;

		case 8:
			Printing::text(Printing::getInstance(), ":::;...................................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::;...................................", 3, 26, "Number");
			break;

		case 9:
			Printing::text(Printing::getInstance(), "::::...................................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::...................................", 3, 26, "Number");
			break;

		case 10:
			Printing::text(Printing::getInstance(), "::::;..................................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::;..................................", 3, 26, "Number");
			break;

		case 11:
			Printing::text(Printing::getInstance(), ":::::..................................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::..................................", 3, 26, "Number");
			break;

		case 12:
			Printing::text(Printing::getInstance(), ":::::;.................................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::;.................................", 3, 26, "Number");
			break;

		case 13:
			Printing::text(Printing::getInstance(), "::::::.................................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::.................................", 3, 26, "Number");
			break;

		case 14:
			Printing::text(Printing::getInstance(), "::::::;................................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::;................................", 3, 26, "Number");
			break;

		case 15:
			Printing::text(Printing::getInstance(), ":::::::................................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::................................", 3, 26, "Number");
			break;

		case 16:
			Printing::text(Printing::getInstance(), ":::::::;...............................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::;...............................", 3, 26, "Number");
			break;

		case 17:
			Printing::text(Printing::getInstance(), "::::::::...............................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::...............................", 3, 26, "Number");
			break;

		case 18:
			Printing::text(Printing::getInstance(), "::::::::;..............................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::;..............................", 3, 26, "Number");
			break;

		case 19:
			Printing::text(Printing::getInstance(), ":::::::::..............................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::..............................", 3, 26, "Number");
			break;

		case 20:
			Printing::text(Printing::getInstance(), ":::::::::;.............................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::;.............................", 3, 26, "Number");
			break;

		case 21:
			Printing::text(Printing::getInstance(), "::::::::::.............................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::.............................", 3, 26, "Number");
			break;

		case 22:
			Printing::text(Printing::getInstance(), "::::::::::;............................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::;............................", 3, 26, "Number");
			break;

		case 23:
			Printing::text(Printing::getInstance(), ":::::::::::;...........................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::;...........................", 3, 26, "Number");
			break;

		case 24:
			Printing::text(Printing::getInstance(), "::::::::::::...........................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::...........................", 3, 26, "Number");
			break;

		case 25:
			Printing::text(Printing::getInstance(), "::::::::::::;..........................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::;..........................", 3, 26, "Number");
			break;

		case 26:
			Printing::text(Printing::getInstance(), ":::::::::::::..........................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::..........................", 3, 26, "Number");
			break;

		case 27:
			Printing::text(Printing::getInstance(), ":::::::::::::;.........................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::;.........................", 3, 26, "Number");
			break;

		case 28:
			Printing::text(Printing::getInstance(), "::::::::::::::.........................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::.........................", 3, 26, "Number");
			break;

		case 29:
			Printing::text(Printing::getInstance(), "::::::::::::::;........................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::;........................", 3, 26, "Number");
			break;

		case 30:
			Printing::text(Printing::getInstance(), ":::::::::::::::........................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::........................", 3, 26, "Number");
			break;

		case 31:
			Printing::text(Printing::getInstance(), ":::::::::::::::;.......................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::;.......................", 3, 26, "Number");
			break;

		case 32:
			Printing::text(Printing::getInstance(), "::::::::::::::::.......................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::::.......................", 3, 26, "Number");
			break;

		case 33:
			Printing::text(Printing::getInstance(), "::::::::::::::::;......................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::::;......................", 3, 26, "Number");
			break;

		case 34:
			Printing::text(Printing::getInstance(), ":::::::::::::::::......................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::::......................", 3, 26, "Number");
			break;

		case 35:
			Printing::text(Printing::getInstance(), ":::::::::::::::::;.....................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::::;.....................", 3, 26, "Number");
			break;

		case 36:
			Printing::text(Printing::getInstance(), "::::::::::::::::::.....................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::::::.....................", 3, 26, "Number");
			break;

		case 37:
			Printing::text(Printing::getInstance(), "::::::::::::::::::;....................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::::::;....................", 3, 26, "Number");
			break;

		case 38:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::....................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::::::....................", 3, 26, "Number");
			break;

		case 39:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::;...................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::::::;...................", 3, 26, "Number");
			break;

		case 40:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::...................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::::::::...................", 3, 26, "Number");
			break;

		case 41:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::;..................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::::::::;..................", 3, 26, "Number");
			break;

		case 42:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::..................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::..................", 3, 26, "Number");
			break;

		case 43:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::;.................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::;.................", 3, 26, "Number");
			break;

		case 44:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::.................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::.................", 3, 26, "Number");
			break;

		case 45:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::;................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::;................", 3, 26, "Number");
			break;

		case 46:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::................", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::................", 3, 26, "Number");
			break;

		case 47:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::;...............", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::;...............", 3, 26, "Number");
			break;

		case 48:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::...............", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::...............", 3, 26, "Number");
			break;

		case 49:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::;..............", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::;..............", 3, 26, "Number");
			break;

		case 50:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::..............", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::..............", 3, 26, "Number");
			break;

		case 51:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::;.............", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::;.............", 3, 26, "Number");
			break;

		case 52:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::.............", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::.............", 3, 26, "Number");
			break;

		case 53:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::;............", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::;............", 3, 26, "Number");
			break;

		case 54:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::............", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::............", 3, 26, "Number");
			break;

		case 55:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::;...........", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::;...........", 3, 26, "Number");
			break;

		case 56:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::...........", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::...........", 3, 26, "Number");
			break;

		case 57:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::;..........", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::;..........", 3, 26, "Number");
			break;

		case 58:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::..........", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::..........", 3, 26, "Number");
			break;

		case 59:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::;.........", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::;.........", 3, 26, "Number");
			break;

		case 60:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::.........", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::.........", 3, 26, "Number");
			break;

		case 61:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::;........", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::;........", 3, 26, "Number");
			break;

		case 62:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::........", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::........", 3, 26, "Number");
			break;

		case 63:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::;.......", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::;.......", 3, 26, "Number");
			break;

		case 64:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::.......", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::.......", 3, 26, "Number");
			break;

		case 65:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::;......", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::;......", 3, 26, "Number");
			break;

		case 66:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::......", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::......", 3, 26, "Number");
			break;

		case 67:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::;.....", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::;.....", 3, 26, "Number");
			break;

		case 68:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::::.....", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::::.....", 3, 26, "Number");
			break;

		case 69:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::::;....", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::::;....", 3, 26, "Number");
			break;

		case 70:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::::....", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::::....", 3, 26, "Number");
			break;

		case 71:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::::;...", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::::;...", 3, 26, "Number");
			break;

		case 72:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::::::...", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::::::...", 3, 26, "Number");
			break;

		case 73:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::::::;..", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::::::;..", 3, 26, "Number");
			break;

		case 74:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::::::..", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::::::..", 3, 26, "Number");
			break;

		case 75:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::::::;.", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::::::;.", 3, 26, "Number");
			break;

		case 76:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::::::::.", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::::::::.", 3, 26, "Number");
			break;

		case 77:
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::::::::;", 3, 25, "Number");
			Printing::text(Printing::getInstance(), "::::::::::::::::::::::::::::::::::::::;", 3, 26, "Number");
			break;

		case 78:
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::::::::", 3, 25, "Number");
			Printing::text(Printing::getInstance(), ":::::::::::::::::::::::::::::::::::::::", 3, 26, "Number");
			break;
	}
}

// handle event
void VideoPlayerState::onFadeInComplete(Object eventFirer __attribute__ ((unused)))
{
	// enable user input
	VUEngine::enableKeypad(VUEngine::getInstance());
}
