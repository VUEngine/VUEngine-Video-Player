//======================================================================
//
//	Play
//  Stacked tiles and map
// 	• Number of frames: 2
// 	• Chars per frame:  3
//  Total size: 96 + 16 = 112
//
//======================================================================

const uint32 PlayTiles[24] __attribute__((aligned(4)))=
{
    // (1/2) Play-000.c

    0x00000000,0x00000000,0x00000000,0x00000000,0x1D000500,0xFD007D00,0xFD00FD00,0xFD00FD00,
    0x00000000,0x00010000,0x001F0007,0x01FF007F,
    
    // (2/2) Play-001.c

    0x00000000,0x00000000,0x00000000,0x00000000,0xFF405540,0xFF40FF40,0xFF40FF40,0xFF40FF40,
    0x07FD0555,0x07FD07FD,0x07FD07FD,0x07FD07FD,
    
};

const uint16 PlayMap[8] __attribute__((aligned(4)))=
{
    // (1/2) Play-000.c

    0x0001,0x0002,0x1001,0x1002,
    
    // (2/2) Play-001.c

    0x0001,0x0002,0x1001,0x1002,
    
};