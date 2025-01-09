/*
 * VUEngine Video Player
 *
 * © Christian Radke and Marten Reiß
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// INCLUDES
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#include <Actor.h>
#include <BgmapAnimatedSprite.h>

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// DECLARATIONS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

extern uint32 PlayTiles[];
extern uint32 PlayTilesFrameOffsets[];
extern uint16 PlayMap[];

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// DEFINITIONS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

AnimationFunctionROMSpec PlayPlayAnimation =
{
	// Number of frames that the texture supports of this animation function
	1,

	// Frames to play in animation
	{0},

	// Number of cycles a frame of animation is displayed
	1,

	// Whether to play it in loop or not
	false,

	// Callback on animation completion
	NULL,

	// Animation's name
	"Play",
};

AnimationFunctionROMSpec PlayPauseAnimation =
{
	// Number of frames that the texture supports of this animation function
	1,

	// Frames to play in animation
	{1},

	// Number of cycles a frame of animation is displayed
	1,

	// Whether to play it in loop or not
	false,

	// Callback on animation completion
	NULL,

	// Animation's name
	"Pause",
};

// an animation spec
const AnimationFunction* PlayAnimations[] =
{
	(AnimationFunction*)&PlayPlayAnimation,
	(AnimationFunction*)&PlayPauseAnimation,
	NULL,
};

CharSetROMSpec PlayCharset =
{
	// Number of chars in function of the number of frames to load at the same time
	3,

	// Whether it is shared or not
	true,
	
	// Whether the tiles are optimized or not
	true,

	// Tiles array
	PlayTiles,

	// Frame offsets array
	PlayTilesFrameOffsets
};

TextureROMSpec PlayTexture =
{
	// Pointer to the char spec that the texture uses
	(CharSetSpec*)&PlayCharset,

	// Pointer to the map array that defines how to use the tiles from the char set
	PlayMap,

	// Horizontal size in tiles of the texture (max. 64)
	2,

	// Vertical size in tiles of the texture (max. 64)
	2,

	// Padding for affine transformations
	{0, 0},

	// Number of frames that the texture supports, depending on charset's allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*, __NOT_ANIMATED: 1
	// __ANIMATED_MULTI: total number of frames
	1,

	// Palette index to use by the graphical data (0 - 3)
	1,

	// Flag to recyble the texture with a different map
	true,

	// Flag to vertically flip the image
	false,

	// Flag to horizontally flip the image
	false
};

BgmapSpriteROMSpec PlaySpriteSpec =
{
	{
		// Sprite
		{
			// Allocator
			__TYPE(BgmapAnimatedSprite),

			// Component type
			kSpriteComponent
		},

		// Spec for the texture to display
		(TextureSpec*)&PlayTexture,

		// Transparency mode (__TRANSPARENCY_NONE, __TRANSPARENCY_EVEN or __TRANSPARENCY_ODD)
		__TRANSPARENCY_NONE,

		// Displacement added to the sprite's position
		{0, 0, 0, 0},
	},

	// The display mode (__WORLD_BGMAP, __WORLD_AFFINE, __WORLD_OBJECT or __WORLD_HBIAS)
	// make sure to use the proper corresponding sprite type throughout the spec (BgmapSprite or ObjectSprite)
	__WORLD_BGMAP,

	// Pointer to affine / hbias manipulation function
	NULL,

	// Flag to indicate in which display to show the texture (__WORLD_ON, __WORLD_LON or __WORLD_RON)
	__WORLD_ON,
};

BgmapSpriteROMSpec* const PlaySprites[] =
{
	&PlaySpriteSpec,
	NULL
};

ComponentSpec** PlayActorComponentSpecs[] = 
{
	@COMPONENTS@
};

ComponentSpec** PlayActorComponentSpecs[] = 
{
	
/*
* VUEngine Video Player
*
* © Christian Radke and Marten Reiß
*
* For the full copyright and license information, please view the LICENSE file
* that was distributed with this source code.
*/
	NULL
};

ActorROMSpec PlayActor =
{
	{
		// Class allocator
		__TYPE(Actor),

		// Component specs
		(ComponentSpec**)PlayActorComponentSpecs,

		// Children specs
		NULL,

		// Extra info info
		NULL,

		// Size
		// If 0, it is computed from the visual components if any
		{0, 0, 0},

		// Actor's in-game type
		0,

	// Pointer to animation functions array
	(const AnimationFunction**)&PlayAnimations,

	// Animation to play automatically
	"Play"
};
