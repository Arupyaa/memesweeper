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
	
private:
	class Title
	{
	public:
	void SpawnMeme();
	void DrawTitle(Vei2& screenpos, Graphics& gfx);
	void SetReveal();
	bool IsRevealed();
	void SetFlagged();
	bool IsFlagged();
		enum State
		{
			hidden,
			revealed,
			flagged
		};
		bool HasMeme = false;
		State state = State::hidden;
	};
	Title& GetTitle(Vei2& gridpos);
	static const int width = 32;
	static const int height = 28;
	Title Field[width*height];
	RectI rec = { 0,width*SpriteCodex::tileSize,0,height*SpriteCodex::tileSize };
};