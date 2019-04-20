#include "MemeSweeper.h"
#include <random>
#include "SpriteCodex.h"

MemeSweeper::MemeSweeper(int nMemes)
{
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xdist (0, width - 1);
	std::uniform_int_distribution<int> ydist(0, height - 1);
	
	for (int nSpawned = 0; nSpawned <= nMemes; ++nSpawned)
	{
		Vei2 gridpos;
		do {
			gridpos = { xdist(rng),ydist(rng) };


		} while (CheckMeme(gridpos));
		GetTitle(gridpos).SpawnMeme();
	}
}

bool MemeSweeper::CheckMeme(Vei2 & gridpos)
{
	return GetTitle(gridpos).HasMeme;
}

void MemeSweeper::DrawField(Graphics & gfx)
{
	for (Vei2 gridpos{ 0,0 }; gridpos.x <= width; ++gridpos.x)
	{
		for (gridpos.y = 0; gridpos.y <= height; ++gridpos.y)
		{
			GetTitle(gridpos).DrawTitle(gridpos*SpriteCodex::tileSize, gfx);
		}
	}
}



MemeSweeper::Title & MemeSweeper::GetTitle(Vei2 & gridpos)
{
	return Field[gridpos.y*width + gridpos.x];
}

void MemeSweeper::Title::SpawnMeme()
{
	HasMeme = true;
}

void MemeSweeper::Title::DrawTitle(Vei2 & screenpos, Graphics & gfx)
{
	switch (state)
	{
	case hidden:
		SpriteCodex::DrawTileButton(screenpos, gfx);
		break;
	case revealed:
		if (!HasMeme)
		{
			SpriteCodex::DrawTile0(screenpos, gfx);
		}
		else
		{
			SpriteCodex::DrawTileBomb(screenpos, gfx);
		}
		break;
	case flagged:
		SpriteCodex::DrawTileButton(screenpos, gfx);
		SpriteCodex::DrawTileFlag(screenpos, gfx);
	}
}
