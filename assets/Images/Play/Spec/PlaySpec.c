/**
 * VUEngine Video Player
 *
 * © Christian Radke and Marten Reiß
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <AnimatedEntity.h>
#include <BgmapAnimatedSprite.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern uint32 PlayTiles[];
extern uint32 PlayTilesFrameOffsets[];
extern uint16 PlayMap[];


//---------------------------------------------------------------------------------------------------------
//												DEFINITIONS
//---------------------------------------------------------------------------------------------------------

AnimationFunctionROMSpec PlayPlayAnimation =
{
	// number of frames of this animation function
	1,

	// frames to play in animation
	{0},

	// number of cycles a frame of animation is displayed
	1,

	// whether to play it in loop or not
	false,

	// method to call on function completion
	NULL,

	// function's name
	"Play",
};

AnimationFunctionROMSpec PlayPauseAnimation =
{
	// number of frames of this animation function
	1,

	// frames to play in animation
	{1},

	// number of cycles a frame of animation is displayed
	1,

	// whether to play it in loop or not
	false,

	// method to call on function completion
	NULL,

	// function's name
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
	// number of chars in function of the number of frames to load at the same time
	3,

	// whether it is shared or not
	true,
	
	// whether the tiles are optimized or not
	true,

	// char spec
	PlayTiles,

	// pointer to the frames offsets
	PlayTilesFrameOffsets
};

TextureROMSpec PlayTexture =
{
	// charset spec
	(CharSetSpec*)&PlayCharset,

	// bgmap spec
	PlayMap,

	// cols (max 64)
	2,

	// rows (max 64)
	2,

	// padding for affine transformations
	{0, 0},

	// number of frames, depending on charset's allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*, __NOT_ANIMATED: 1
	// __ANIMATED_MULTI: total number of frames
	1,

	// palette number (0-3)
	1,

	// recyclable
	true,

	// vertical flip
	false,

	// horizontal flip
	false
};

BgmapSpriteROMSpec PlaySprite =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture spec
		(TextureSpec*)&PlayTexture,

		// transparency (__TRANSPARENCY_NONE, __TRANSPARENCY_EVEN or __TRANSPARENCY_ODD)
		__TRANSPARENCY_NONE,

		// displacement
		{0, 0, 0, 0},
	},

	// bgmap mode (__WORLD_BGMAP, __WORLD_AFFINE, __WORLD_OBJECT or __WORLD_HBIAS)
	// make sure to use the proper corresponding sprite type throughout the spec (BgmapSprite or ObjectSprite)
	__WORLD_BGMAP,

	// pointer to affine / hbias manipulation function
	NULL,

	// display mode (__WORLD_ON, __WORLD_LON or __WORLD_RON)
	__WORLD_ON,
};

BgmapSpriteROMSpec* const PlaySprites[] =
{
	&PlaySprite,
	NULL
};

AnimatedEntityROMSpec PlayEntity =
{
	{
		// class allocator
		__TYPE(AnimatedEntity),

		// children
		NULL,

		// behaviors 
		NULL,

		// extra info
		NULL,

		// sprites
		(SpriteSpec**)PlaySprites,

		// use z displacement in projection
		false,

		// wireframes
		NULL,

		// colliders
		NULL,

		// size
		// if 0, width and height will be inferred from the first sprite's texture's size
		{0, 0, 0},

		// gameworld's character's type
		0,

		// physical specification
		NULL,
	},

	// pointer to the animation spec for the item
	(const AnimationFunction**)&PlayAnimations,

	// initial animation
	"Play"
};
