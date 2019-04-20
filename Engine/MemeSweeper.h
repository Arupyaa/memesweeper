#pragma once
#include "Vei2.h" 
#include "Graphics.h"
#include "SpriteCodex.h"

class MemeSweeper
{
public:
	MemeSweeper(int nMemes);
	bool CheckMeme(Vei2& gridpos);
	void DrawField(Graphics& gfx);
	void RevealedOnClick(Vei2& gridpos);
	void FlaggedOnClick(Vei2& gridpos);
	bool IsFucked = false;
	
private:
	class Title
	{
	public:
	void SpawnMeme();
	void DrawTitle(Vei2& screenpos, Graphics& gfx, bool Fuck);
	void SetReveal();
	bool IsRevealed();
	void SetFlagged();
	bool IsFlagged();
	void SetNumMemes(int nMemesCount);
		enum State
		{
			hidden,
			revealed,
			flagged
		};
		bool HasMeme = false;
		State state = State::hidden;
		int nNeighbouringMemes = -1;
	};
	int CountNeighbouringMemes(Vei2& gridpos);
	Title& GetTitle(Vei2& gridpos);
	static const int width = 26;
	static const int height = 24;
	Title Field[width*height];
	RectI rec = { Xstart*SpriteCodex::tileSize,(width+Xstart)*SpriteCodex::tileSize,Ystart*SpriteCodex::tileSize,(height+Ystart)*SpriteCodex::tileSize };
	static constexpr int Xstart = (Graphics::ScreenWidth/2 / SpriteCodex::tileSize) - width / 2;
	static constexpr int Ystart =(Graphics::ScreenHeight/2 / SpriteCodex::tileSize) - height / 2;
};