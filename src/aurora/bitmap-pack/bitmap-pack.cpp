#include "bitmap-pack.h"

bool BitmapPack::BitmapCompareMaxSide(const Bitmap& a, const Bitmap& b)
{
	return (a.w > a.h ? a.w : a.h) > (b.w > b.h ? b.w : b.h);
}

unsigned int BitmapPack::Add(unsigned char* data, unsigned int w, unsigned int h, unsigned char bpp)
{
	if (w == 0)
		return 0;
	if (h == 0)
		return 0;
	if (bpp == 0)
		return 0;
	if (data == 0)
		return 0;

    BitmapPack::Bitmap bmp;
    bmp.data = new unsigned char[w * h * bpp];
    memcpy(bmp.data, data, w * h * bpp);
    bmp.w = w;
    bmp.h = h;
    bmp.bpp = bpp;
	

    unsigned int index = bitmaps.size();
	bmp.orig_index = index;
    bitmaps.push_back(bmp);
    return index;
}

BitmapPack::Bitmap BitmapPack::Pack(unsigned char bpp)
{
	free_slots.clear();

    if(bitmaps.size() == 0)
        return bitmap_packed;

	std::vector<Bitmap> bitmaps = this->bitmaps;
    std::sort(bitmaps.begin(), bitmaps.end(), BitmapPack::BitmapCompareMaxSide);

    bitmap_packed.w = bitmaps[0].w;
    bitmap_packed.h = bitmaps[0].h;
	bitmap_packed.bpp = bitmaps[0].bpp;
    Slot slot;
    slot.w = bitmaps[0].w;
    slot.h = bitmaps[0].h;
    slot.x = 0;
    slot.y = 0;
    free_slots.push_back(slot);

    for(unsigned int i = 0; i < bitmaps.size(); ++i)
    {
        Insert(&this->bitmaps[bitmaps[i].orig_index]);
    }

	if (bpp > 0)
		bitmap_packed.bpp = bpp;

	unsigned int pot_w = bitmap_packed.w;
	unsigned int pot_h = bitmap_packed.h;
	pot_w--;
	pot_w |= pot_w >> 1;
	pot_w |= pot_w >> 2;
	pot_w |= pot_w >> 4;
	pot_w |= pot_w >> 8;
	pot_w |= pot_w >> 16;
	pot_w++;
	pot_h--;
	pot_h |= pot_h >> 1;
	pot_h |= pot_h >> 2;
	pot_h |= pot_h >> 4;
	pot_h |= pot_h >> 8;
	pot_h |= pot_h >> 16;
	pot_h++;

	bitmap_packed.w = pot_w;
	bitmap_packed.h = pot_h;

	if (bitmap_packed.data)
		delete[] bitmap_packed.data;
	bitmap_packed.data = new unsigned char[bitmap_packed.w * bitmap_packed.h * bitmap_packed.bpp];
	memset(bitmap_packed.data, 0, bitmap_packed.w * bitmap_packed.h * bitmap_packed.bpp);

	for (unsigned int i = 0; i < bitmaps.size(); ++i)
	{
		BitmapBlit(bitmap_packed, this->bitmaps[bitmaps[i].orig_index]);
	}
	/*
	unsigned char* img_flipped = new unsigned char[bitmap_packed.w * bitmap_packed.h * bitmap_packed.bpp];
	for (int i = 0, k = bitmap_packed.w * bitmap_packed.h - bitmap_packed.w; i < bitmap_packed.w * bitmap_packed.h; i += bitmap_packed.w, k -= bitmap_packed.w)
	{
		for (int j = 0; j < bitmap_packed.w; ++j)
		{
			for (unsigned int l = 0; l < bitmap_packed.bpp; ++l)
			{
				img_flipped[(i + j) * bitmap_packed.bpp + l] = bitmap_packed.data[(k + j) * bitmap_packed.bpp + l];
			}
		}
	}
	memcpy(bitmap_packed.data, img_flipped, bitmap_packed.w * bitmap_packed.h * bitmap_packed.bpp);
	delete[] img_flipped;
	*/
	return bitmap_packed;
}

BitmapPack::Bitmap* BitmapPack::Get(unsigned int index)
{
	return &(bitmaps[index]);
}

void BitmapPack::Insert(Bitmap* bitmap)
{
    for(unsigned int i = 0; i < free_slots.size(); ++i)
    {
        if(Fit(bitmap, i))
        {
            return;
        }
    }

    unsigned int slot_id = Expand(bitmap);

    Fit(bitmap, slot_id);

	if (bitmap->bpp > bitmap_packed.bpp)
		bitmap_packed.bpp = bitmap->bpp;
}

bool BitmapPack::Fit(Bitmap* bitmap, unsigned int slot_index)
{
    Slot slot = free_slots[slot_index];
    if(slot.w < bitmap->w)
        return false;
    if(slot.h < bitmap->h)
        return false;

	free_slots.erase(free_slots.begin() + slot_index);

    // if slot is larger than bitmap - split the slot
    if(slot.w > bitmap->w)
    {
		Slot slot_a = slot;
		Slot slot_b = slot;

		slot_a.w = bitmap->w;
		slot_a.h = slot_a.h;
		slot_a.x = slot_a.x;
		slot_a.y = slot_a.y;
		slot_index = free_slots.size();
		free_slots.push_back(slot_a);

		bool result = Fit(bitmap, slot_index);

		slot_b.w = slot_b.w - bitmap->w;
		slot_b.h = slot_b.h;
		slot_b.x = slot_b.x + bitmap->w;
		slot_b.y = slot_b.y;
		slot_index = free_slots.size();
		free_slots.push_back(slot_b);
		return result;
    }

    if(slot.h > bitmap->h)
    {
		Slot slot_a = slot;
		Slot slot_b = slot;

		slot_a.w = slot_a.w;
		slot_a.h = bitmap->h;
		slot_a.x = slot_a.x;
		slot_a.y = slot_a.y;
		slot_index = free_slots.size();
		free_slots.push_back(slot_a);

		bool result = Fit(bitmap, slot_index);
		
		slot_b.w = slot_b.w;
		slot_b.h = slot_b.h - bitmap->h;
		slot_b.x = slot_b.x;
		slot_b.y = slot_b.y + bitmap->h;
		slot_index = free_slots.size();
		free_slots.push_back(slot_b);
		return result;
    }

    bitmap->uv_left = (float)slot.x;
    bitmap->uv_bottom = (float)slot.y;
    bitmap->uv_right = (float)(slot.x + slot.w);
    bitmap->uv_top = (float)(slot.y + slot.h);

    return true;
}

unsigned int BitmapPack::Expand(Bitmap* bitmap)
{
    unsigned int slot_index = 0;
    if(bitmap_packed.w > bitmap_packed.h)
    {
        // Stretch by Y
        Slot slot;
        slot.w = bitmap_packed.w;
        slot.h = bitmap->h;
        slot.x = 0;
        slot.y = bitmap_packed.h;
        slot_index = free_slots.size();
        free_slots.push_back(slot);
        bitmap_packed.h = bitmap_packed.h + bitmap->h;
    }
    else
    {
        // Stretch by X
        Slot slot;
        slot.w = bitmap->w;
        slot.h = bitmap_packed.h;
        slot.x = bitmap_packed.w;
        slot.y = 0;
        slot_index = free_slots.size();
        free_slots.push_back(slot);
        bitmap_packed.w = bitmap_packed.w + bitmap->w;
    }
    return slot_index;
}

void BitmapPack::BitmapBlit(Bitmap& to, Bitmap from)
{
	unsigned char* pixel = new unsigned char[to.bpp];
	for (unsigned int y = 0; y < from.h; ++y)
	{
		for (unsigned int x = 0; x < from.w; ++x)
		{
			unsigned int from_pixel_pos = y * from.w + x;
			unsigned int to_pixel_pos = (float)((y + from.uv_bottom) * to.w + x + from.uv_left);

			if(to.bpp == 4)
				if(from.bpp == 4)
					to.data[to_pixel_pos * to.bpp + 3] = from.data[from_pixel_pos * from.bpp + 3];
				else
					to.data[to_pixel_pos * to.bpp + 3] = 255;

			if (to.bpp >= 3)
				if (from.bpp >= 3)
					to.data[to_pixel_pos * to.bpp + 2] = from.data[from_pixel_pos * from.bpp + 2];
				else
					to.data[to_pixel_pos * to.bpp + 2] = from.data[from_pixel_pos * from.bpp + 0];

			if (to.bpp >= 2)
				if (from.bpp >= 2)
					to.data[to_pixel_pos * to.bpp + 1] = from.data[from_pixel_pos * from.bpp + 1];
				else
					to.data[to_pixel_pos * to.bpp + 1] = from.data[from_pixel_pos * from.bpp + 0];

			if (to.bpp >= 1)
				if (from.bpp >= 1)
					to.data[to_pixel_pos * to.bpp + 0] = from.data[from_pixel_pos * from.bpp + 0];
		}
	}
	delete pixel;
}