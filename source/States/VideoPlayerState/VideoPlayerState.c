/*
 * VUEngine Video Player
 *
 * © Christian Radke and Marten Reiß
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// INCLUDES
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#include <Singleton.h>
#include <string.h>

#include <Camera.h>
#include <CameraEffectManager.h>
#include <KeypadManager.h>
#include <MessageDispatcher.h>
#include <Printer.h>
#include <VideoPlayerState.h>
#include <VIPManager.h>
#include <VUEngine.h>

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// DECLARATIONS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

extern StageROMSpec VideoStage;

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS'S METHODS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void VideoPlayerState::constructor()
{
	// Always explicitly call the base's constructor 
	Base::constructor();

	// Init members
	this->videoActor = NULL;
	this->progressBarActor = NULL;
	this->playActor = NULL;
	this->videoPlaying = true;
	this->guiVisible = false;
	this->numberOfFrames = 0;
	this->hiColorMode = true;
}

void VideoPlayerState::destructor()
{
	// Always explicitly call the base's destructor 
	Base::destructor();
}

void VideoPlayerState::enter(void* owner __attribute__ ((unused)))
{
	Base::enter(this, owner);

	// Disable user input
	KeypadManager::enable();

	// Load stage
	GameState::configureStage(GameState::safeCast(this), (StageSpec*)&VideoStage, NULL);

	// Get actors from stage
	this->videoActor = Actor::safeCast(Container::getChildByName
	(
		Container::safeCast(this->stage),
		"VideoEnt",
		false
	));

	this->progressBarActor = Actor::safeCast(Container::getChildByName
	(
		Container::safeCast(this->stage),
		"ProgrEnt",
		false
	));
	
	this->playActor = Actor::safeCast(Container::getChildByName
	(
		Container::safeCast(this->stage),
		"PlayEnt",
		false
	));

	this->numberOfFrames = 0;//Actor::getNumberOfFrames(this->videoActor);
	this->numberOfFrames >>= 1;

	// Initially hide gui
	VideoPlayerState::hideGui(this);

	// Start clocks to start animations
	GameState::startClocks(GameState::safeCast(this));

	// Start fade in effect
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
	PRINT_TIME(1, 1);
return;
	// Refresh GUI if the video is playing
	if(this->guiVisible && this->videoPlaying)
	{
		VideoPlayerState::printFrames(this);
		VideoPlayerState::printProgress(this);
	}

	// If the video is paused in hicolor mode, make sure to alternate the two frames that make the hi-color image
	if(!this->videoPlaying && this->hiColorMode)
	{
		int currentFrame = Actor::getActualFrame(this->videoActor);

		if(currentFrame & 1)
		{
			Actor::previousFrame(Actor::safeCast(this->videoActor));
		}
		else
		{
			Actor::nextFrame(Actor::safeCast(this->videoActor));
		}
	}
}

void VideoPlayerState::processUserInput(UserInput userInput)
{
	if
	(
		(userInput.pressedKey & K_LR) || ((userInput.holdKey & K_LR) && (userInput.holdKeyDuration > 12)) ||
		(userInput.pressedKey & K_RR) || ((userInput.holdKey & K_RR) && (userInput.holdKeyDuration > 12)) ||
		(userInput.pressedKey & K_RT) || ((userInput.holdKey & K_RT) && (userInput.holdKeyDuration > 12))
	)
	{
		// Pause animation
		if(this->videoPlaying)
		{
			VideoPlayerState::pauseVideo(this);
			Actor::playAnimation(Actor::safeCast(this->playActor), "Pause");
			this->videoPlaying = false;
			VideoPlayerState::showGui(this);
		}

		// Show next frame
		Actor::nextFrame(Actor::safeCast(this->videoActor));
		Actor::nextFrame(Actor::safeCast(this->videoActor));

		VideoPlayerState::printFrames(this);
		VideoPlayerState::printProgress(this);
	}
	else if
	(
		(userInput.pressedKey & K_LL) || ((userInput.holdKey & K_LL) && (userInput.holdKeyDuration > 12)) ||
		(userInput.pressedKey & K_RL) || ((userInput.holdKey & K_RL) && (userInput.holdKeyDuration > 12)) ||
		(userInput.pressedKey & K_LT) || ((userInput.holdKey & K_LT) && (userInput.holdKeyDuration > 12))
	)
	{
		// Pause animation
		if(this->videoPlaying)
		{
			VideoPlayerState::pauseVideo(this);
			Actor::playAnimation(Actor::safeCast(this->playActor), "Pause");
			this->videoPlaying = false;
			VideoPlayerState::showGui(this);
		}

		// Show previous frame
		Actor::previousFrame(Actor::safeCast(this->videoActor));
		Actor::previousFrame(Actor::safeCast(this->videoActor));

		VideoPlayerState::printFrames(this);
		VideoPlayerState::printProgress(this);
	}
	else if(userInput.pressedKey & K_STA)
	{
		// Pause/resume animation
		if(this->videoPlaying)
		{
			VideoPlayerState::pauseVideo(this);
		}
		else
		{
			Actor::pauseAnimation(Actor::safeCast(this->videoActor), false);
		}
		this->videoPlaying = !this->videoPlaying;
		if(this->videoPlaying)
		{
			Actor::playAnimation(Actor::safeCast(this->playActor), "Play");
			VideoPlayerState::hideGui(this);
		}
		else
		{
			Actor::playAnimation(Actor::safeCast(this->playActor), "Pause");
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
		int currentFrame = Actor::getActualFrame(this->videoActor);
		Actor::playAnimation(Actor::safeCast(this->videoActor), this->hiColorMode ? "HiColor" : "4Color");
		Actor::setActualFrame(this->videoActor, currentFrame);
		Actor::pauseAnimation(Actor::safeCast(this->videoActor), !this->videoPlaying);
	}
}

void VideoPlayerState::showGui()
{
	// Actors
	Actor::show(this->progressBarActor);
	Actor::show(this->playActor);

	// Frame counter
	VideoPlayerState::printFrames(this);

	// Progress bar
	VideoPlayerState::printProgress(this);

	this->guiVisible = true;
}

void VideoPlayerState::hideGui()
{
	// Actors
	Actor::hide(this->progressBarActor);
	Actor::hide(this->playActor);

	// Frame counter
	Printer::text("...", 44, 25, "Number");
	Printer::text("....", 43, 26, "Number");

	// Progress bar
	Printer::text(".......................................", 3, 25, "Number");
	Printer::text(".......................................", 3, 26, "Number");

	this->guiVisible = false;
}

void VideoPlayerState::printFrames()
{
	int currentFrame = Actor::getActualFrame(this->videoActor);
	currentFrame >>= 1;
	currentFrame += 1;

	if(currentFrame >= 100)
	{
		Printer::int32(currentFrame, 44, 25, "Number");
	}
	else if(currentFrame >= 10)
	{
		Printer::text("0", 44, 25, "Number");
		Printer::int32(currentFrame, 45, 25, "Number");
	}
	else
	{
		Printer::text("00", 44, 25, "Number");
		Printer::int32(currentFrame, 46, 25, "Number");
	}

	if(this->numberOfFrames >= 100)
	{
		Printer::text("/", 43, 26, "Number");
		Printer::int32(this->numberOfFrames, 44, 26, "Number");
	}
	else if(this->numberOfFrames >= 10)
	{
		Printer::text("/0", 43, 26, "Number");
		Printer::int32(this->numberOfFrames, 45, 26, "Number");
	}
	else
	{
		Printer::text("/00", 43, 26, "Number");
		Printer::int32(this->numberOfFrames, 46, 26, "Number");
	}
}

void VideoPlayerState::pauseVideo()
{
	Actor::pauseAnimation(Actor::safeCast(this->videoActor), true);

	int currentFrame = Actor::getActualFrame(this->videoActor);

	// If currentFrame is odd, go to previous frame (4 color frame)
	if(currentFrame & 1)
	{
		Actor::previousFrame(Actor::safeCast(this->videoActor));
	}
}

void VideoPlayerState::printProgress()
{
	int currentFrame = Actor::getActualFrame(this->videoActor);
	currentFrame >>= 1;

	int progress = (int)((float)currentFrame / (float)this->numberOfFrames * 78);

	switch(progress)
	{
		case 0:
		case 1:
			Printer::text(".......................................", 3, 25, "Number");
			Printer::text(".......................................", 3, 26, "Number");
			break;

		case 2:
			Printer::text(";......................................", 3, 25, "Number");
			Printer::text(";......................................", 3, 26, "Number");
			break;

		case 3:
			Printer::text(":......................................", 3, 25, "Number");
			Printer::text(":......................................", 3, 26, "Number");
			break;

		case 4:
			Printer::text(":;.....................................", 3, 25, "Number");
			Printer::text(":;.....................................", 3, 26, "Number");
			break;

		case 5:
			Printer::text("::.....................................", 3, 25, "Number");
			Printer::text("::.....................................", 3, 26, "Number");
			break;

		case 6:
			Printer::text("::;....................................", 3, 25, "Number");
			Printer::text("::;....................................", 3, 26, "Number");
			break;

		case 7:
			Printer::text(":::....................................", 3, 25, "Number");
			Printer::text(":::....................................", 3, 26, "Number");
			break;

		case 8:
			Printer::text(":::;...................................", 3, 25, "Number");
			Printer::text(":::;...................................", 3, 26, "Number");
			break;

		case 9:
			Printer::text("::::...................................", 3, 25, "Number");
			Printer::text("::::...................................", 3, 26, "Number");
			break;

		case 10:
			Printer::text("::::;..................................", 3, 25, "Number");
			Printer::text("::::;..................................", 3, 26, "Number");
			break;

		case 11:
			Printer::text(":::::..................................", 3, 25, "Number");
			Printer::text(":::::..................................", 3, 26, "Number");
			break;

		case 12:
			Printer::text(":::::;.................................", 3, 25, "Number");
			Printer::text(":::::;.................................", 3, 26, "Number");
			break;

		case 13:
			Printer::text("::::::.................................", 3, 25, "Number");
			Printer::text("::::::.................................", 3, 26, "Number");
			break;

		case 14:
			Printer::text("::::::;................................", 3, 25, "Number");
			Printer::text("::::::;................................", 3, 26, "Number");
			break;

		case 15:
			Printer::text(":::::::................................", 3, 25, "Number");
			Printer::text(":::::::................................", 3, 26, "Number");
			break;

		case 16:
			Printer::text(":::::::;...............................", 3, 25, "Number");
			Printer::text(":::::::;...............................", 3, 26, "Number");
			break;

		case 17:
			Printer::text("::::::::...............................", 3, 25, "Number");
			Printer::text("::::::::...............................", 3, 26, "Number");
			break;

		case 18:
			Printer::text("::::::::;..............................", 3, 25, "Number");
			Printer::text("::::::::;..............................", 3, 26, "Number");
			break;

		case 19:
			Printer::text(":::::::::..............................", 3, 25, "Number");
			Printer::text(":::::::::..............................", 3, 26, "Number");
			break;

		case 20:
			Printer::text(":::::::::;.............................", 3, 25, "Number");
			Printer::text(":::::::::;.............................", 3, 26, "Number");
			break;

		case 21:
			Printer::text("::::::::::.............................", 3, 25, "Number");
			Printer::text("::::::::::.............................", 3, 26, "Number");
			break;

		case 22:
			Printer::text("::::::::::;............................", 3, 25, "Number");
			Printer::text("::::::::::;............................", 3, 26, "Number");
			break;

		case 23:
			Printer::text(":::::::::::;...........................", 3, 25, "Number");
			Printer::text(":::::::::::;...........................", 3, 26, "Number");
			break;

		case 24:
			Printer::text("::::::::::::...........................", 3, 25, "Number");
			Printer::text("::::::::::::...........................", 3, 26, "Number");
			break;

		case 25:
			Printer::text("::::::::::::;..........................", 3, 25, "Number");
			Printer::text("::::::::::::;..........................", 3, 26, "Number");
			break;

		case 26:
			Printer::text(":::::::::::::..........................", 3, 25, "Number");
			Printer::text(":::::::::::::..........................", 3, 26, "Number");
			break;

		case 27:
			Printer::text(":::::::::::::;.........................", 3, 25, "Number");
			Printer::text(":::::::::::::;.........................", 3, 26, "Number");
			break;

		case 28:
			Printer::text("::::::::::::::.........................", 3, 25, "Number");
			Printer::text("::::::::::::::.........................", 3, 26, "Number");
			break;

		case 29:
			Printer::text("::::::::::::::;........................", 3, 25, "Number");
			Printer::text("::::::::::::::;........................", 3, 26, "Number");
			break;

		case 30:
			Printer::text(":::::::::::::::........................", 3, 25, "Number");
			Printer::text(":::::::::::::::........................", 3, 26, "Number");
			break;

		case 31:
			Printer::text(":::::::::::::::;.......................", 3, 25, "Number");
			Printer::text(":::::::::::::::;.......................", 3, 26, "Number");
			break;

		case 32:
			Printer::text("::::::::::::::::.......................", 3, 25, "Number");
			Printer::text("::::::::::::::::.......................", 3, 26, "Number");
			break;

		case 33:
			Printer::text("::::::::::::::::;......................", 3, 25, "Number");
			Printer::text("::::::::::::::::;......................", 3, 26, "Number");
			break;

		case 34:
			Printer::text(":::::::::::::::::......................", 3, 25, "Number");
			Printer::text(":::::::::::::::::......................", 3, 26, "Number");
			break;

		case 35:
			Printer::text(":::::::::::::::::;.....................", 3, 25, "Number");
			Printer::text(":::::::::::::::::;.....................", 3, 26, "Number");
			break;

		case 36:
			Printer::text("::::::::::::::::::.....................", 3, 25, "Number");
			Printer::text("::::::::::::::::::.....................", 3, 26, "Number");
			break;

		case 37:
			Printer::text("::::::::::::::::::;....................", 3, 25, "Number");
			Printer::text("::::::::::::::::::;....................", 3, 26, "Number");
			break;

		case 38:
			Printer::text(":::::::::::::::::::....................", 3, 25, "Number");
			Printer::text(":::::::::::::::::::....................", 3, 26, "Number");
			break;

		case 39:
			Printer::text(":::::::::::::::::::;...................", 3, 25, "Number");
			Printer::text(":::::::::::::::::::;...................", 3, 26, "Number");
			break;

		case 40:
			Printer::text("::::::::::::::::::::...................", 3, 25, "Number");
			Printer::text("::::::::::::::::::::...................", 3, 26, "Number");
			break;

		case 41:
			Printer::text("::::::::::::::::::::;..................", 3, 25, "Number");
			Printer::text("::::::::::::::::::::;..................", 3, 26, "Number");
			break;

		case 42:
			Printer::text(":::::::::::::::::::::..................", 3, 25, "Number");
			Printer::text(":::::::::::::::::::::..................", 3, 26, "Number");
			break;

		case 43:
			Printer::text(":::::::::::::::::::::;.................", 3, 25, "Number");
			Printer::text(":::::::::::::::::::::;.................", 3, 26, "Number");
			break;

		case 44:
			Printer::text("::::::::::::::::::::::.................", 3, 25, "Number");
			Printer::text("::::::::::::::::::::::.................", 3, 26, "Number");
			break;

		case 45:
			Printer::text("::::::::::::::::::::::;................", 3, 25, "Number");
			Printer::text("::::::::::::::::::::::;................", 3, 26, "Number");
			break;

		case 46:
			Printer::text(":::::::::::::::::::::::................", 3, 25, "Number");
			Printer::text(":::::::::::::::::::::::................", 3, 26, "Number");
			break;

		case 47:
			Printer::text(":::::::::::::::::::::::;...............", 3, 25, "Number");
			Printer::text(":::::::::::::::::::::::;...............", 3, 26, "Number");
			break;

		case 48:
			Printer::text("::::::::::::::::::::::::...............", 3, 25, "Number");
			Printer::text("::::::::::::::::::::::::...............", 3, 26, "Number");
			break;

		case 49:
			Printer::text("::::::::::::::::::::::::;..............", 3, 25, "Number");
			Printer::text("::::::::::::::::::::::::;..............", 3, 26, "Number");
			break;

		case 50:
			Printer::text(":::::::::::::::::::::::::..............", 3, 25, "Number");
			Printer::text(":::::::::::::::::::::::::..............", 3, 26, "Number");
			break;

		case 51:
			Printer::text(":::::::::::::::::::::::::;.............", 3, 25, "Number");
			Printer::text(":::::::::::::::::::::::::;.............", 3, 26, "Number");
			break;

		case 52:
			Printer::text("::::::::::::::::::::::::::.............", 3, 25, "Number");
			Printer::text("::::::::::::::::::::::::::.............", 3, 26, "Number");
			break;

		case 53:
			Printer::text("::::::::::::::::::::::::::;............", 3, 25, "Number");
			Printer::text("::::::::::::::::::::::::::;............", 3, 26, "Number");
			break;

		case 54:
			Printer::text(":::::::::::::::::::::::::::............", 3, 25, "Number");
			Printer::text(":::::::::::::::::::::::::::............", 3, 26, "Number");
			break;

		case 55:
			Printer::text(":::::::::::::::::::::::::::;...........", 3, 25, "Number");
			Printer::text(":::::::::::::::::::::::::::;...........", 3, 26, "Number");
			break;

		case 56:
			Printer::text("::::::::::::::::::::::::::::...........", 3, 25, "Number");
			Printer::text("::::::::::::::::::::::::::::...........", 3, 26, "Number");
			break;

		case 57:
			Printer::text("::::::::::::::::::::::::::::;..........", 3, 25, "Number");
			Printer::text("::::::::::::::::::::::::::::;..........", 3, 26, "Number");
			break;

		case 58:
			Printer::text(":::::::::::::::::::::::::::::..........", 3, 25, "Number");
			Printer::text(":::::::::::::::::::::::::::::..........", 3, 26, "Number");
			break;

		case 59:
			Printer::text(":::::::::::::::::::::::::::::;.........", 3, 25, "Number");
			Printer::text(":::::::::::::::::::::::::::::;.........", 3, 26, "Number");
			break;

		case 60:
			Printer::text("::::::::::::::::::::::::::::::.........", 3, 25, "Number");
			Printer::text("::::::::::::::::::::::::::::::.........", 3, 26, "Number");
			break;

		case 61:
			Printer::text("::::::::::::::::::::::::::::::;........", 3, 25, "Number");
			Printer::text("::::::::::::::::::::::::::::::;........", 3, 26, "Number");
			break;

		case 62:
			Printer::text(":::::::::::::::::::::::::::::::........", 3, 25, "Number");
			Printer::text(":::::::::::::::::::::::::::::::........", 3, 26, "Number");
			break;

		case 63:
			Printer::text(":::::::::::::::::::::::::::::::;.......", 3, 25, "Number");
			Printer::text(":::::::::::::::::::::::::::::::;.......", 3, 26, "Number");
			break;

		case 64:
			Printer::text("::::::::::::::::::::::::::::::::.......", 3, 25, "Number");
			Printer::text("::::::::::::::::::::::::::::::::.......", 3, 26, "Number");
			break;

		case 65:
			Printer::text("::::::::::::::::::::::::::::::::;......", 3, 25, "Number");
			Printer::text("::::::::::::::::::::::::::::::::;......", 3, 26, "Number");
			break;

		case 66:
			Printer::text(":::::::::::::::::::::::::::::::::......", 3, 25, "Number");
			Printer::text(":::::::::::::::::::::::::::::::::......", 3, 26, "Number");
			break;

		case 67:
			Printer::text(":::::::::::::::::::::::::::::::::;.....", 3, 25, "Number");
			Printer::text(":::::::::::::::::::::::::::::::::;.....", 3, 26, "Number");
			break;

		case 68:
			Printer::text("::::::::::::::::::::::::::::::::::.....", 3, 25, "Number");
			Printer::text("::::::::::::::::::::::::::::::::::.....", 3, 26, "Number");
			break;

		case 69:
			Printer::text("::::::::::::::::::::::::::::::::::;....", 3, 25, "Number");
			Printer::text("::::::::::::::::::::::::::::::::::;....", 3, 26, "Number");
			break;

		case 70:
			Printer::text(":::::::::::::::::::::::::::::::::::....", 3, 25, "Number");
			Printer::text(":::::::::::::::::::::::::::::::::::....", 3, 26, "Number");
			break;

		case 71:
			Printer::text(":::::::::::::::::::::::::::::::::::;...", 3, 25, "Number");
			Printer::text(":::::::::::::::::::::::::::::::::::;...", 3, 26, "Number");
			break;

		case 72:
			Printer::text("::::::::::::::::::::::::::::::::::::...", 3, 25, "Number");
			Printer::text("::::::::::::::::::::::::::::::::::::...", 3, 26, "Number");
			break;

		case 73:
			Printer::text("::::::::::::::::::::::::::::::::::::;..", 3, 25, "Number");
			Printer::text("::::::::::::::::::::::::::::::::::::;..", 3, 26, "Number");
			break;

		case 74:
			Printer::text(":::::::::::::::::::::::::::::::::::::..", 3, 25, "Number");
			Printer::text(":::::::::::::::::::::::::::::::::::::..", 3, 26, "Number");
			break;

		case 75:
			Printer::text(":::::::::::::::::::::::::::::::::::::;.", 3, 25, "Number");
			Printer::text(":::::::::::::::::::::::::::::::::::::;.", 3, 26, "Number");
			break;

		case 76:
			Printer::text("::::::::::::::::::::::::::::::::::::::.", 3, 25, "Number");
			Printer::text("::::::::::::::::::::::::::::::::::::::.", 3, 26, "Number");
			break;

		case 77:
			Printer::text("::::::::::::::::::::::::::::::::::::::;", 3, 25, "Number");
			Printer::text("::::::::::::::::::::::::::::::::::::::;", 3, 26, "Number");
			break;

		case 78:
			Printer::text(":::::::::::::::::::::::::::::::::::::::", 3, 25, "Number");
			Printer::text(":::::::::::::::::::::::::::::::::::::::", 3, 26, "Number");
			break;
	}
}

// handle event
void VideoPlayerState::onFadeInComplete(Object eventFirer __attribute__ ((unused)))
{
	// Enable user input
	KeypadManager::enable();
}
