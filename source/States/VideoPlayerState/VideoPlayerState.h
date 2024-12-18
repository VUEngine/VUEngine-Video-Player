/*
 * VUEngine Video Player
 *
 * © Christian Radke and Marten Reiß
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef VIDEO_PLAYER_STATE_H_
#define VIDEO_PLAYER_STATE_H_


//---------------------------------------------------------------------------------------------------------
// 												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <GameState.h>
#include <AnimatedEntity.h>


//---------------------------------------------------------------------------------------------------------
// 											CLASS'S DECLARATION
//---------------------------------------------------------------------------------------------------------

singleton class VideoPlayerState : GameState
{
	AnimatedEntity videoEntity;
	Entity progressBarEntity;
	AnimatedEntity playEntity;
	int numberOfFrames;
	bool videoPlaying;
	bool guiVisible;
	bool hiColorMode;

	static VideoPlayerState getInstance();
	override void enter(void* owner);
	override void execute(void* owner);
	override void processUserInput(UserInput userInput);
}


#endif
