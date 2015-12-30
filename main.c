#include <FaNES.h>
#include "imageA.h"
#include "imageB.h"
#include "imageC.h"
#include "imageD.h"

const uchar palette[16] = {
    0x11, 0x00, 0x10, 0x30,
    0x01, 0x21, 0x31, 0x30,
    0x06, 0x16, 0x26, 0x36,
    0x0A, 0x09, 0x19, 0x29
};

static uchar select, sprite0X, sprite0Y;
static uint topScrollX, topScrollY, bottomScrollX, bottomScrollY;
static int topSpeedHorizontal, topSpeedVertical,
    bottomSpeedHorizontal, bottomSpeedVertical;

int main(void) {
    setPaletteBackground(palette);
    setPaletteColor(PALETTE_SPRITE_1 + 2, 0x00);
    setPaletteColor(PALETTE_SPRITE_1 + 3, 0x30);
    unpackNametableRLE(imageA, NAMETABLE_A);
    unpackNametableRLE(imageB, NAMETABLE_B);
    unpackNametableRLE(imageC, NAMETABLE_C);
    unpackNametableRLE(imageD, NAMETABLE_D);
    sprite0X = 0;
    sprite0Y = 120;
    newSprite(sprite0X, sprite0Y, 0x0E, NULL, 0);
    newSprite(sprite0X + 4, sprite0Y - 4, 0x1D, NULL, 4);
    onGraphics();
    while(TRUE) {
        uchar padPo = padPool(PAD_PORT_1);
        uchar padPu = padPush(PAD_PORT_1);
        if (padPo) {
            if(padPu & PAD_BUTTON_SELECT) {
                ++select;
                if (select > 2) select = 0;
                newSprite(sprite0X + 4, sprite0Y - 4, 0x1D + select, NULL, 4);
            }
            switch(select) {
                case 0:
                    if(padPu & PAD_BUTTON_LEFT) --topSpeedHorizontal;
                    if(padPu & PAD_BUTTON_RIGHT) ++topSpeedHorizontal;
                    if(padPu & PAD_BUTTON_UP) --topSpeedVertical;
                    if(padPu & PAD_BUTTON_DOWN) ++topSpeedVertical;
                break;
                case 1:
                    if(padPu & PAD_BUTTON_LEFT) --bottomSpeedHorizontal;
                    if(padPu & PAD_BUTTON_RIGHT) ++bottomSpeedHorizontal;
                    if(padPu & PAD_BUTTON_UP) --bottomSpeedVertical;
                    if(padPu & PAD_BUTTON_DOWN) ++bottomSpeedVertical;
                break;
                case 2:
                    if(padPo & PAD_BUTTON_LEFT && sprite0X > 0) --sprite0X;
                    if(padPo & PAD_BUTTON_RIGHT && sprite0X < 254) ++sprite0X;
                    if(padPo & PAD_BUTTON_UP && sprite0Y > 0) --sprite0Y;
                    if(padPo & PAD_BUTTON_DOWN && sprite0Y < 238) ++sprite0Y;
                    setSpritePosition(sprite0X, sprite0Y, 0);
                    setSpritePosition(sprite0X + 4, sprite0Y - 4, 4);
                break;
            }
        }
        topScrollX += topSpeedHorizontal;
        topScrollY += topSpeedVertical;
        bottomScrollX += bottomSpeedHorizontal;
        bottomScrollY += bottomSpeedVertical;
        setBackgroundScrollI(topScrollX, topScrollY);
        waitFrame();
        setBackgroundScrollEventSprite0HitI(bottomScrollX, bottomScrollY);
    }
}
