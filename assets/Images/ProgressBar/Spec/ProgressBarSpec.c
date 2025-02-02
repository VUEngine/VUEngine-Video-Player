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
#include <BgmapSprite.h>

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// DECLARATIONS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

extern uint32 ProgressBarTiles[];
extern uint16 ProgressBarMap[];

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// DEFINITIONS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

CharSetROMSpec ProgressBarCharset =
{
	// Number of chars in function of the number of frames to load at the same time
	5,

	// Whether it is shared or not
	true,
	
	// Whether the tiles are optimized or not
	false,

	// Tiles array
	ProgressBarTiles,

	// Frame offsets array
	NULL,
};

TextureROMSpec ProgressBarTexture =
{
	// Pointer to the char spec that the texture uses
	(CharSetSpec*)&ProgressBarCharset,

	// Pointer to the map array that defines how to use the tiles from the char set
	ProgressBarMap,

	// Horizontal size in tiles of the texture (max. 64)
	40,

	// Vertical size in tiles of the texture (max. 64)
	3,

	// Padding added to the size for affine/hbias transformations (cols, rows)
	{0, 0},

	// Number of frames that the texture supports, depending on charset's allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*, __NOT_ANIMATED: 1
	// __ANIMATED_MULTI: total number of frames
	1,

	// Palette index to use by the graphical data (0 - 3)
	1,

	// Flag to recyble the texture with a different map
	false,

	// Flag to vertically flip the image
	false,

	// Flag to horizontally flip the image
	false
};

BgmapSpriteROMSpec ProgressBarSpriteSpec =
{
	{
		// Sprite
		{
			// Allocator
			__TYPE(BgmapSprite),

			// Component type
			kSpriteComponent
		},

		// Is animated?
		false,

		// Spec for the texture to display
		(TextureSpec*)&ProgressBarTexture,

		// Transparency mode (__TRANSPARENCY_NONE, __TRANSPARENCY_EVEN or __TRANSPARENCY_ODD)
		__TRANSPARENCY_NONE,

		// Displacement added to the sprite's position
		{0, 0, 0, 0},
	},

	// The display mode (__WORLD_BGMAP, __WORLD_AFFINE, __WORLD_OBJECT or __WORLD_HBIAS)
	// make sure to use the proper corresponding sprite type throughout the spec (BgmapSprite or ObjectSprite)
	__WORLD_BGMAP,

	// Pointer to affine/hbias manipulation function
	NULL,

	// Flag to indicate in which display to show the texture (__WORLD_ON, __WORLD_LON or __WORLD_RON)
	__WORLD_ON,
};

BgmapSpriteROMSpec* const ProgressBarSprites[] =
{
	&ProgressBarSprite,
	NULL
};

ComponentSpec** ProgressBarActorComponentSpecs[] = 
{
	@COMPONENTS@
};

ComponentSpec** ProgressBarActorComponentSpecs[] = 
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

ActorROMSpec ProgressBarActor =
{
	// Class allocator
	__TYPE(Actor),

	// Component specs
	(ComponentSpec**)ProgressBarActorComponentSpecs,

	// Children specs
	NULL,

	// Extra info info
	NULL,

	// Size
	// If 0, it is computed from the visual components if any
	{0, 0, 0},

	// Actor's in-game type
	kTypeNone,

	// Pointer to animation functions array
	NULL,

	// Animation to play automatically
	NULL
};

