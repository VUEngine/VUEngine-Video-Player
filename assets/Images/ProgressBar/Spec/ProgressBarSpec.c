/*
 * VUEngine Video Player
 *
 * © Christian Radke and Marten Reiß
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */


//=========================================================================================================
// INCLUDES
//=========================================================================================================

#include <Entity.h>
#include <BgmapSprite.h>


//=========================================================================================================
// DECLARATIONS
//=========================================================================================================

extern uint32 ProgressBarTiles[];
extern uint16 ProgressBarMap[];


//=========================================================================================================
// DEFINITIONS
//=========================================================================================================

CharSetROMSpec ProgressBarCharset =
{
	// number of chars in function of the number of frames to load at the same time
	5,

	// whether it is shared or not
	true,
	
	// whether the tiles are optimized or not
	false,

	// char spec
	ProgressBarTiles,

	// pointer to the frames offsets
	NULL,
};

TextureROMSpec ProgressBarTexture =
{
	// charset spec
	(CharSetSpec*)&ProgressBarCharset,

	// bgmap spec
	ProgressBarMap,

	// cols (max 64)
	40,

	// rows (max 64)
	3,

	// padding for affine/hbias transformations (cols, rows)
	{0, 0},

	// number of frames, depending on charset's allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*, __NOT_ANIMATED: 1
	// __ANIMATED_MULTI: total number of frames
	1,

	// palette number (0-3)
	1,

	// recyclable
	false,

	// vertical flip
	false,

	// horizontal flip
	false
};

BgmapSpriteROMSpec ProgressBarSprite =
{
	{
		// Sprite
		{
		    // Allocator
		    __TYPE(BgmapSprite),

		    // Component type
		    kSpriteComponent
		},

		// texture spec
		(TextureSpec*)&ProgressBarTexture,

		// transparency (__TRANSPARENCY_NONE, __TRANSPARENCY_EVEN or __TRANSPARENCY_ODD)
		__TRANSPARENCY_NONE,

		// displacement
		{0, 0, 0, 0},
	},

	// bgmap mode (__WORLD_BGMAP, __WORLD_AFFINE, __WORLD_OBJECT or __WORLD_HBIAS)
	// make sure to use the proper corresponding sprite type throughout the spec (BgmapSprite or ObjectSprite)
	__WORLD_BGMAP,

	// pointer to affine/hbias manipulation function
	NULL,

	// display mode (__WORLD_ON, __WORLD_LON or __WORLD_RON)
	__WORLD_ON,
};

BgmapSpriteROMSpec* const ProgressBarSprites[] =
{
	&ProgressBarSprite,
	NULL
};



ComponentSpec** ProgressBarEntityComponentSpecs[] = 
{
    @COMPONENTS@
};

ComponentSpec** ProgressBarEntityComponentSpecs[] = 
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

EntityROMSpec ProgressBarEntity =
{
	// Class allocator	
	__TYPE(Entity),

	// Component specs
	(ComponentSpec**)ProgressBarEntityComponentSpecs,

	// Children specs
	NULL,

	// Extra info info
	NULL,

	// Size
	// If 0, it is computed from the visual components if any
	{0, 0, 0},

	// In-game entity's type
	kTypeNone
};
