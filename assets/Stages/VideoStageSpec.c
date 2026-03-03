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

#include <Stage.h>
#include <Fonts.h>
#include <DisplayUnit.h>

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// DECLARATIONS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

extern ActorSpec PlayActor;
extern ActorSpec ProgressBarActor;
extern ActorSpec VideoActor;
extern FontSpec NumberFontSpec;

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// ASSETS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

PositionedActorROMSpec VideoStageChildren[] =
{
	{&PlayActor,			{-192 + 12, 112 - 16, 0}, {0, 0, 0}, {1, 1, 1}, 0, "PlayEnt", NULL, NULL, true},
	{&ProgressBarActor,	{0 - 8, 112 - 16, 0}, {0, 0, 0}, {1, 1, 1}, 0, "ProgrEnt", NULL, NULL, true},
	{&VideoActor,			{0, 0, 0}, {0, 0, 0}, {1, 1, 1}, 1, "VideoEnt", NULL, NULL, true},

	{NULL, {0, 0, 0}, {0, 0, 0}, {1, 1, 1}, 0, NULL, NULL, NULL, false},
};

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// ASSETS LISTS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

FontROMSpec* const VideoStageFonts[] =
{
	&NumberFontSpec,

	NULL
};

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// STAGE DEFINITION
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

StageROMSpec VideoStage =
{
	// Class allocator
	__TYPE(Stage),

	// Timer config
	{
		__TIMER_100US,
		10,
		kMS,
	},

	// General stage's attributes
	{
		// Stage's size in pixels
		{
			// x
			__SCREEN_WIDTH,
			// y
			__SCREEN_HEIGHT,
			// z
			__SCREEN_DEPTH,
		},

		// Camera's initial position inside the stage
   		{
			// x
			0,
			// y
			0,
			// z
			0,
			// p
			0,
		},

		// Camera's frustum
		{
			// x0
			0,
			// y0
			0,
			// z0
			0,
			// x1
			__SCREEN_WIDTH,
			// y1
			__SCREEN_HEIGHT,
			// z1
			__SCREEN_WIDTH,
		}
	},

	// Streaming
	{
		// Padding to be added to camera's frustum when checking if a actor spec
		// describes an actor that is within the camera's range
		40,
		// Padding to be added to camera's frustum when checking if a actor is
		// out of the camera's range
		16,
		// Amount of actor descriptions to check for streaming in actors
		24,
		// If true, actor instantiation is done over time
		false,
	},

	// Rendering
	{
		// Maximum number of texture's rows to write each time the texture writing is active
		64,

		// Maximum number of rows to compute on each call to the affine functions
		16,

		// Color configuration
   		{
			// Background color
			__COLOR_BLACK,

			// Brightness
			// These values times the repeat values specified in the column table (max. 16) make the final
			// brightness values on the respective regions of the screen. maximum brightness is 128.
   			{
				// Dark red
				__BRIGHTNESS_DARK_RED,
				// Medium red
				__BRIGHTNESS_MEDIUM_RED,
				// Bright red
				__BRIGHTNESS_BRIGHT_RED,
			},

			// Brightness repeat
			(BrightnessRepeatSpec*)NULL,
		},

		// Palettes' configuration
   		{
			{
				// Bgmap palette 0
				__BGMAP_PALETTE_0,
				// Bgmap palette 1
				__BGMAP_PALETTE_1,
				// Bgmap palette 2
				__BGMAP_PALETTE_2,
				// Bgmap palette 3
				__BGMAP_PALETTE_3,
			},
			{
				// Object palette 0
				__OBJECT_PALETTE_0,
				// Object palette 1
				__OBJECT_PALETTE_1,
				// Object palette 2
				__OBJECT_PALETTE_2,
				// Object palette 3
				__OBJECT_PALETTE_3,
			},
		},

		// Bgmap segments configuration
		// Number of BGMAP segments reserved for the param
		0,

		// Object Sprite Containers configuration
	   	{
			// __spt0
			{false, 0},
			// __spt1
			{false, 0},
			// __spt2
			{false, 0},
			// __spt3
			{false, 0},
		},

		// Struct defining the optical settings for the stage
	   	{
			// Maximum view distance's power
			__MAXIMUM_X_VIEW_DISTANCE,
			__MAXIMUM_Y_VIEW_DISTANCE,
			// logical distance between the eyes and the screen
			__CAMERA_NEAR_PLANE,
			// Distance from left to right eye (depth sensation)
			__BASE_FACTOR,
			// Horizontal view point center
			__HORIZONTAL_VIEW_POINT_CENTER,
			// Vertical view point center
			__VERTICAL_VIEW_POINT_CENTER,
			// Scaling factor
			__SCALING_MODIFIER_FACTOR,
		},
	},

	// Physical world's properties
	{
		// Gravity
   		{
			__F_TO_FIX10_6(0),
			__F_TO_FIX10_6(0),
			__F_TO_FIX10_6(0),
		},

		// Friction coefficient
		__F_TO_FIX10_6(0),
	},

	// Assets
	{
		// Fonts to preload
		(FontSpec**)VideoStageFonts,

		// CharSets to preload
		(CharSetSpec**)NULL,

		// Textures to preload
		(TextureSpec**)NULL,

		// Sounds to load
		NULL,
	},

	// Actors
	{
		// UI configuration
   		{
			NULL,
			NULL,
		},

		// Stage's children actors
		(PositionedActor*)VideoStageChildren,
	},

	// Post processing effects
	(PostProcessingEffect*)NULL,
};
