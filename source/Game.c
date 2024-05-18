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

#include <VideoPlayerState.h>
#include <VUEngine.h>


//---------------------------------------------------------------------------------------------------------
// 										  GAME'S MAIN LOOP
//---------------------------------------------------------------------------------------------------------

int game(void)
{
   // start the game
	VUEngine::start(VUEngine::getInstance(), GameState::safeCast(VideoPlayerState::getInstance()));

	// end program
	return true;
}
