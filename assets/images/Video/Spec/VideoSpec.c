/* VUEngine - Virtual Utopia Engine <http://vuengine.planetvb.com/>
 * A universal game engine for the Nintendo Virtual Boy
 *
 * Copyright (C) 2007, 2018 by Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <chris@vr32.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
 * NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <AnimatedEntity.h>
#include <BgmapAnimatedSprite.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern BYTE VideoLTiles[];
extern BYTE VideoLMap[];
extern BYTE VideoRTiles[];
extern BYTE VideoRMap[];


//---------------------------------------------------------------------------------------------------------
//												DEFINITIONS
//---------------------------------------------------------------------------------------------------------

AnimationFunctionROMSpec VIDEO_HI_COLOR_ANIM =
{
	// number of frames of this animation function
	944,

	// frames to play in animation
	{
		  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
		 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
		 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
		 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
		120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,
		150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,
		180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,
		210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
		240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,
		270,271,272,273,274,275,276,277,278,279,280,281,282,283,284,285,286,287,288,289,290,291,292,293,294,295,296,297,298,299,
		300,301,302,303,304,305,306,307,308,309,310,311,312,313,314,315,316,317,318,319,320,321,322,323,324,325,326,327,328,329,
		330,331,332,333,334,335,336,337,338,339,340,341,342,343,344,345,346,347,348,349,350,351,352,353,354,355,356,357,358,359,
		360,361,362,363,364,365,366,367,368,369,370,371,372,373,374,375,376,377,378,379,380,381,382,383,384,385,386,387,388,389,
		390,391,392,393,394,395,396,397,398,399,400,401,402,403,404,405,406,407,408,409,410,411,412,413,414,415,416,417,418,419,
		420,421,422,423,424,425,426,427,428,429,430,431,432,433,434,435,436,437,438,439,440,441,442,443,444,445,446,447,448,449,
		450,451,452,453,454,455,456,457,458,459,460,461,462,463,464,465,466,467,468,469,470,471,472,473,474,475,476,477,478,479,
		480,481,482,483,484,485,486,487,488,489,490,491,492,493,494,495,496,497,498,499,500,501,502,503,504,505,506,507,508,509,
		510,511,512,513,514,515,516,517,518,519,520,521,522,523,524,525,526,527,528,529,530,531,532,533,534,535,536,537,538,539,
		540,541,542,543,544,545,546,547,548,549,550,551,552,553,554,555,556,557,558,559,560,561,562,563,564,565,566,567,568,569,
		570,571,572,573,574,575,576,577,578,579,580,581,582,583,584,585,586,587,588,589,590,591,592,593,594,595,596,597,598,599,
		600,601,602,603,604,605,606,607,608,609,610,611,612,613,614,615,616,617,618,619,620,621,622,623,624,625,626,627,628,629,
		630,631,632,633,634,635,636,637,638,639,640,641,642,643,644,645,646,647,648,649,650,651,652,653,654,655,656,657,658,659,
		660,661,662,663,664,665,666,667,668,669,670,671,672,673,674,675,676,677,678,679,680,681,682,683,684,685,686,687,688,689,
		690,691,692,693,694,695,696,697,698,699,700,701,702,703,704,705,706,707,708,709,710,711,712,713,714,715,716,717,718,719,
		720,721,722,723,724,725,726,727,728,729,730,731,732,733,734,735,736,737,738,739,740,741,742,743,744,745,746,747,748,749,
		750,751,752,753,754,755,756,757,758,759,760,761,762,763,765,766,767,768,769,770,771,772,773,774,775,776,777,778,779,780,
		781,782,783,784,785,786,787,788,789,790,791,792,793,794,795,796,797,798,799,800,801,802,803,804,805,806,807,808,809,810,
		811,812,813,814,815,816,817,818,819,820,821,822,823,824,825,826,827,828,829,830,831,832,833,834,835,836,837,838,839,840,
		841,842,843,844,845,846,847,848,849,850,851,852,853,854,855,856,857,858,859,860,861,862,863,864,865,866,867,868,869,870,
		871,872,873,874,875,876,877,878,879,880,881,882,883,884,885,886,887,888,889,890,891,892,893,894,895,896,897,898,899,900,
		901,902,903,904,905,906,907,908,909,910,911,912,913,914,915,916,917,918,919,920,921,922,923,924,925,926,927,928,929,930,
		931,932,933,934,935,936,937,938,939,940,941,942,943
	},

	// number of cycles a frame of animation is displayed
	1,

	// whether to play it in loop or not
	true,

	// method to call on function completion
	NULL,

	// function's name
	"HiColor",
};

AnimationFunctionROMSpec VIDEO_4_COLOR_ANIM =
{
	// number of frames of this animation function
	944,

	// frames to play in animation
	{
		  0,  0,  2,  2,  4,  4,  6,  6,  8,  8, 10, 10, 12, 12, 14, 14, 16, 16, 18, 18, 20, 20, 22, 22, 24, 24, 26, 26, 28, 28,
		 30, 30, 32, 32, 34, 34, 36, 36, 38, 38, 40, 40, 42, 42, 44, 44, 46, 46, 48, 48, 50, 50, 52, 52, 54, 54, 56, 56, 58, 58,
		 60, 60, 62, 62, 64, 64, 66, 66, 68, 68, 70, 70, 72, 72, 74, 74, 76, 76, 78, 78, 80, 80, 82, 82, 84, 84, 86, 86, 88, 88,
		 90, 90, 92, 92, 94, 94, 96, 96, 98, 98,100,100,102,102,104,104,106,106,108,108,110,110,112,112,114,114,116,116,118,118,
		120,120,122,122,124,124,126,126,128,128,130,130,132,132,134,134,136,136,138,138,140,140,142,142,144,144,146,146,148,148,
		150,150,152,152,154,154,156,156,158,158,160,160,162,162,164,164,166,166,168,168,170,170,172,172,174,174,176,176,178,178,
		180,180,182,182,184,184,186,186,188,188,190,190,192,192,194,194,196,196,198,198,200,200,202,202,204,204,206,206,208,208,
		210,210,212,212,214,214,216,216,218,218,220,220,222,222,224,224,226,226,228,228,230,230,232,232,234,234,236,236,238,238,
		240,240,242,242,244,244,246,246,248,248,250,250,252,252,254,254,256,256,258,258,260,260,262,262,264,264,266,266,268,268,
		270,270,272,272,274,274,276,276,278,278,280,280,282,282,284,284,286,286,288,288,290,290,292,292,294,294,296,296,298,298,
		300,300,302,302,304,304,306,306,308,308,310,310,312,312,314,314,316,316,318,318,320,320,322,322,324,324,326,326,328,328,
		330,330,332,332,334,334,336,336,338,338,340,340,342,342,344,344,346,346,348,348,350,350,352,352,354,354,356,356,358,358,
		360,360,362,362,364,364,366,366,368,368,370,370,372,372,374,374,376,376,378,378,380,380,382,382,384,384,386,386,388,388,
		390,390,392,392,394,394,396,396,398,398,400,400,402,402,404,404,406,406,408,408,410,410,412,412,414,414,416,416,418,418,
		420,420,422,422,424,424,426,426,428,428,430,430,432,432,434,434,436,436,438,438,440,440,442,442,444,444,446,446,448,448,
		450,450,452,452,454,454,456,456,458,458,460,460,462,462,464,464,466,466,468,468,470,470,472,472,474,474,476,476,478,478,
		480,480,482,482,484,484,486,486,488,488,490,490,492,492,494,494,496,496,498,498,500,500,502,502,504,504,506,506,508,508,
		510,510,512,512,514,514,516,516,518,518,520,520,522,522,524,524,526,526,528,528,530,530,532,532,534,534,536,536,538,538,
		540,540,542,542,544,544,546,546,548,548,550,550,552,552,554,554,556,556,558,558,560,560,562,562,564,564,566,566,568,568,
		570,570,572,572,574,574,576,576,578,578,580,580,582,582,584,584,586,586,588,588,590,590,592,592,594,594,596,596,598,598,
		600,600,602,602,604,604,606,606,608,608,610,610,612,612,614,614,616,616,618,618,620,620,622,622,624,624,626,626,628,628,
		630,630,632,632,634,634,636,636,638,638,640,640,642,642,644,644,646,646,648,648,650,650,652,652,654,654,656,656,658,658,
		660,660,662,662,664,664,666,666,668,668,670,670,672,672,674,674,676,676,678,678,680,680,682,682,684,684,686,686,688,688,
		690,690,692,692,694,694,696,696,698,698,700,700,702,702,704,704,706,706,708,708,710,710,712,712,714,714,716,716,718,718,
		720,720,722,722,724,724,726,726,728,728,730,730,732,732,734,734,736,736,738,738,740,740,742,742,744,744,746,746,748,748,
		750,750,752,752,754,754,756,756,758,758,760,760,762,762,765,765,767,767,769,769,770,770,772,772,774,774,776,776,778,778,
		780,780,782,782,784,784,786,786,788,788,790,790,792,792,794,794,796,796,798,798,800,800,802,802,804,804,806,806,808,808,
		810,810,812,812,814,814,816,816,818,818,820,820,822,822,824,824,826,826,828,828,830,830,832,832,834,834,836,836,838,838,
		840,840,842,842,844,844,846,846,848,848,850,850,852,852,854,854,856,856,858,858,860,860,862,862,864,864,866,866,868,868,
		870,870,872,872,874,874,876,876,878,878,880,880,882,882,884,884,886,886,888,888,890,890,892,892,894,894,896,896,898,898,
		900,900,902,902,904,904,906,906,908,908,910,910,912,912,914,914,916,916,918,918,920,920,922,922,924,924,926,926,928,928,
		930,930,932,932,934,934,936,936,938,938,940,940,942,942
	},

	// number of cycles a frame of animation is displayed
	1,

	// whether to play it in loop or not
	true,

	// method to call on function completion
	NULL,

	// function's name
	"4Color",
};

// an animation spec
AnimationDescriptionROMSpec VIDEO_ANIM =
{
	// animation functions
	{
		(AnimationFunction*)&VIDEO_HI_COLOR_ANIM,
		(AnimationFunction*)&VIDEO_4_COLOR_ANIM,

		NULL,
	}
};

CharSetROMSpec VIDEO_L_CH =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	783,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__ANIMATED_SINGLE_OPTIMIZED,

	// char spec
	VideoLTiles,
};

TextureROMSpec VIDEO_L_TX =
{
	// charset spec
	(CharSetSpec*)&VIDEO_L_CH,

	// bgmap spec
	VideoLMap,

	// cols (max 64)
	48,

	// rows (max 64)
	28,

	// padding for affine transformations
	{0, 0},

	// number of frames, depending on charset's allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*, __NOT_ANIMATED: 1
	// __ANIMATED_MULTI: total number of frames
	1,

	// palette number (0-3)
	0,

	// recyclable
	true,
};

BgmapSpriteROMSpec VIDEO_L_SPRITE =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture spec
		(TextureSpec*)&VIDEO_L_TX,

		// transparent (__TRANSPARENCY_NONE, __TRANSPARENCY_EVEN or __TRANSPARENCY_ODD)
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
	__WORLD_LON,
};

CharSetROMSpec VIDEO_R_CH =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	748,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__ANIMATED_SINGLE_OPTIMIZED,

	// char spec
	VideoRTiles,
};

TextureROMSpec VIDEO_R_TX =
{
	// charset spec
	(CharSetSpec*)&VIDEO_R_CH,

	// bgmap spec
	VideoRMap,

	// cols (max 64)
	48,

	// rows (max 64)
	28,

	// padding for affine transformations
	{0, 0},

	// number of frames, depending on charset's allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*, __NOT_ANIMATED: 1
	// __ANIMATED_MULTI: total number of frames
	1,

	// palette number (0-3)
	0,

	// recyclable
	true,
};

BgmapSpriteROMSpec VIDEO_R_SPRITE =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture spec
		(TextureSpec*)&VIDEO_R_TX,

		// transparent (__TRANSPARENCY_NONE, __TRANSPARENCY_EVEN or __TRANSPARENCY_ODD)
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
	__WORLD_RON,
};

BgmapSpriteROMSpec* const VIDEO_SPRITES[] =
{
	&VIDEO_L_SPRITE,
	&VIDEO_R_SPRITE,
	NULL
};

AnimatedEntityROMSpec VIDEO_EN =
{
	{
		// class allocator
		__TYPE(AnimatedEntity),

		// sprites
		(SpriteROMSpec**)VIDEO_SPRITES,

		// collision shapes
		(ShapeSpec*)NULL,

		// size
		// if 0, width and height will be inferred from the first sprite's texture's size
		{0, 0, 0},

		// gameworld's character's type
		0,

		// physical specification
		(PhysicalSpecification*)NULL,
	},

	// pointer to the animation spec for the item
	(AnimationDescription*)&VIDEO_ANIM,

	// initial animation
	"HiColor"
};