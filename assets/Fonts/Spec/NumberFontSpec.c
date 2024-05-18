///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                              THIS FILE WAS AUTO-GENERATED - DO NOT EDIT                               //
///////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------------------------
//                                                INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <Printing.h>


//---------------------------------------------------------------------------------------------------------
//                                              DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern uint32 NumberFontTiles[];


//---------------------------------------------------------------------------------------------------------
//                                               DEFINITIONS
//---------------------------------------------------------------------------------------------------------

CharSetROMSpec NumberFontCharSet =
{
	// number of chars in function of the number of frames to load at the same time
	14,

	// whether it is shared or not
	true,
	 
	// whether the tiles are optimized or not
	false,

	// char spec
	NumberFontTiles,

	// pointer to the frames offsets
	NULL,
};

FontROMSpec NumberFontSpec =
{
	// font charset spec pointer
	(CharSetSpec*)&NumberFontCharSet,

	// character number at which the font starts, allows you to skip the control characters for example
	46,

	// number of characters in this font
	14,

	// number of characters per line in charset
	14,

	// size of a single character (in chars) ({width, height})
	{1, 1},

	// font's name
	"Number",
};