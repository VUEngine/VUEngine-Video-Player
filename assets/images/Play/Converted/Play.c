//---------------------------------------------------------------------------------------------------------
//
//  Play
//  • 16×16 pixels
//  • 4 tiles, reduced by non-unique and flipped tiles, not compressed
//  • 2×2 map, not compressed
//  • 2 animation frames, individual files, largest frame: 2 tiles
//  Size: 68 + 16 = 84
//
//---------------------------------------------------------------------------------------------------------

const uint32 PlayTiles[17] __attribute__((aligned(4))) =
{
    0x00000000,0x1D000500,0xFD007D00,0xFD00FD00,0xFD00FD00,0x00000000,0x00010000,0x001F0007,
    0x01FF007F,0xFF405540,0xFF40FF40,0xFF40FF40,0xFF40FF40,0x07FD0555,0x07FD07FD,0x07FD07FD,
    0x07FD07FD,
};

const uint16 PlayMap[8] __attribute__((aligned(4))) =
{
    0x0000,0x0001,0x1000,0x1001,0x0000,0x0001,0x1000,0x1001,
};

const uint32 PlayTilesFrameOffsets[2] __attribute__((aligned(4))) =
{
    0x00000001,0x00000009,
};
