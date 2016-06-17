#ifndef _BITMAP_PACK_H_
#define _BITBAP_PACK_H_

#include <vector>
#include <algorithm>

class BitmapPack
{
public:
    struct Bitmap
    {
		Bitmap() : data(0) {}
        unsigned char* data;
        unsigned int w, h;
        unsigned char bpp;
        float uv_left, uv_bottom;
        float uv_right, uv_top;
		unsigned int orig_index;
    };

	struct Slot
    {
        unsigned int w, h;
        unsigned int x, y;
    };

    unsigned int Add(unsigned char* data, unsigned int w, unsigned int h, unsigned char bpp);
    Bitmap Pack(unsigned char bpp = 0);
    Bitmap* Get(unsigned int index);
private:
    void Insert(Bitmap* bitmap);
    bool Fit(Bitmap* bitmap, unsigned int slot_index);
    unsigned int Expand(Bitmap* bitmap);

	void BitmapBlit(Bitmap& to, Bitmap from);

	static bool BitmapCompareMaxSide(const Bitmap& a, const Bitmap& b);

    std::vector<Bitmap> bitmaps;
    Bitmap bitmap_packed;
    std::vector<Slot> free_slots;
};

#endif