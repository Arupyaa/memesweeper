#include "MemeSweeper.h"
#include <random>
#include "SpriteCodex.h"
#include <assert.h>
#include <algorithm>

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

	//reveal test
	//for (int i = 0; i <= 70; ++i)
	//{
	//	Vei2 randompos;
	//	do
	//	{
	//		randompos = { xdist(rng),ydist(rng) };
	//	} while (GetTitle(randompos).IsRevealed());
	//	GetTitle(randompos).SetReveal();
	//}
	for (Vei2 gridpos{ 0,0 }; gridpos.x < width; ++gridpos.x)
	{
		for (gridpos.y = 0; gridpos.y < height; ++gridpos.y)
		{
			GetTitle(gridpos).SetNumMemes(CountNeighbouringMemes(gridpos));
		}
	}
}

bool MemeSweeper::CheckMeme(Vei2 & gridpos)
{
	return GetTitle(gridpos).HasMeme;
}

void MemeSweeper::DrawField(Graphics & gfx)
{
	gfx.DrawRect(rec, Colors::Gray);
	for (Vei2 gridpos{ Xstart,Ystart}; gridpos.x < width+Xstart; ++gridpos.x)
	{
		for (gridpos.y = Ystart; gridpos.y < height + Ystart; ++gridpos.y)
		{
			GetTitle(gridpos).DrawTitle(gridpos*SpriteCodex::tileSize,gfx,IsFucked);
		}
	}
}

void MemeSweeper::RevealedOnClick(Vei2 & gridpos)
{
	if (!IsFucked)
	{
		assert(gridpos.x > 0
			&& gridpos.x < width*SpriteCodex::tileSize
			&& gridpos.y < height*SpriteCodex::tileSize
			&& gridpos.y > 0);
		GetTitle(gridpos / SpriteCodex::tileSize).SetReveal();
		if (GetTitle(gridpos / SpriteCodex::tileSize).HasMeme)
		{
			IsFucked = true;
		}
	}
}

void MemeSweeper::FlaggedOnClick(Vei2 & gridpos)
{
	if (!IsFucked)
	{
		assert(gridpos.x > 0
			&& gridpos.x < width*SpriteCodex::tileSize
			&& gridpos.y < height*SpriteCodex::tileSize
			&& gridpos.y > 0);
		GetTitle(gridpos / SpriteCodex::tileSize).SetFlagged();
	}
}



int MemeSweeper::CountNeighbouringMemes(Vei2 & gridpos)
{
	int count = 0;
	int Xstart = std::max(0, gridpos.x - 1);
	int Ystart = std::max(0, gridpos.y - 1);
	int Xend = std::min(width, gridpos.x + 1);
	int Yend = std::min(height, gridpos.y + 1);
	
	for (Vei2 pos{ Xstart,Ystart }; pos.x <= Xend; ++pos.x)
	{
		for (pos.y = Ystart; pos.y <= Yend; ++pos.y)
		{
			if (GetTitle(pos).HasMeme)
			{
				++count;
			}
		}
	}
	return count;
}

MemeSweeper::Title & MemeSweeper::GetTitle(Vei2 & gridpos)
{
	return Field[gridpos.y*width + gridpos.x];
}

void MemeSweeper::Title::SpawnMeme()
{
	HasMeme = true;
}

void MemeSweeper::Title::DrawTitle(Vei2& screenpos, Graphics& gfx, bool Fuck)
{
	if (!Fuck)
	{
		switch (state)
		{
		case hidden:
			SpriteCodex::DrawTileButton(screenpos, gfx);
			break;
		case revealed:
			if (!HasMeme)
			{
				SpriteCodex::DrawTileNum(screenpos, nNeighbouringMemes, gfx);
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
	else if (Fuck)
	{
		switch (state)
		{
		case hidden:
			if (HasMeme)
			{
				SpriteCodex::DrawTileBomb(screenpos, gfx);
			}
			else
			{
				SpriteCodex::DrawTileButton(screenpos, gfx);
			}
			break;
		case revealed:
			if (!HasMeme)
			{
				SpriteCodex::DrawTileNum(screenpos, nNeighbouringMemes, gfx);
			}
			else
			{
				SpriteCodex::DrawTileBombRed(screenpos, gfx);
			}
			break;
		case flagged:
			if (HasMeme)
			{
				SpriteCodex::DrawTileBomb(screenpos, gfx);
				SpriteCodex::DrawTileFlag(screenpos, gfx);
			}
			else
			{
				SpriteCodex::DrawTileBomb(screenpos, gfx);
				SpriteCodex::DrawTileCross(screenpos, gfx);
			}

		}
	}
}

void MemeSweeper::Title::SetReveal()
{
	state = State::revealed;
}

bool MemeSweeper::Title::IsRevealed()
{
	return state == State::revealed;
}

void MemeSweeper::Title::SetFlagged()
{
	if (state == hidden)
	{
		state = State::flagged;
	}
	else if (state == flagged)
	{
		state = State::hidden;
	}
}

bool MemeSweeper::Title::IsFlagged()
{
	return state == State::flagged;
}

void MemeSweeper::Title::SetNumMemes(int nMemesCount)
{
	assert(nMemesCount >= 0 && nMemesCount <= 8);
	nNeighbouringMemes = nMemesCount;
}
