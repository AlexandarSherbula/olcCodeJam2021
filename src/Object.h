#ifndef OBJECT_H
#define OBJECT_H

#include "olcPixelGameEngine.h"

#include <array>

class Object
{
public:
	olc::vf2d position;
	olc::vi2d size;
	olc::Pixel color;
public:
	Object();
	Object(const olc::vf2d& pos, const olc::Pixel& color, const olc::vi2d& size);

	virtual void Behaviour(float fElapsedTime) = 0;

	virtual void Draw();
	virtual void Draw(const olc::vi2d& position, const olc::Pixel& color = olc::WHITE);
};

class Player : public Object
{
private:
	bool mOnGround;
	olc::vf2d velocity;
	olc::Sprite* mSprite;
	olc::Decal* mDecal;

	olc::vi2d mTileID;
	float mTimeCounter;
	float mTimeBetweenFrames;

	bool isDead;
public:
	olc::vf2d speed;
public:
	Player();
	Player(const olc::vf2d& position, const olc::vf2d& speed, const olc::Pixel& color = olc::WHITE, const olc::vi2d& size = {16, 16});

	void Behaviour(float fElapsedTime) override;

	void Draw() override;
private:
	bool CoinPickedUp(const olc::vi2d& newPos);
	void CoinPointUp(const olc::vi2d& newPos, std::array<int, 4>& coinType);
};

class CoinPickUp
{
public:
	static olc::Sprite* sSprite;
	static olc::Decal* sDecal;

	static olc::vi2d sTileID;
	static float sTimeCounter;
	static float sTimeBetweenFrames;
public:
	static void CreateSprite();
	static void DeleteSprite();

	static void Animation(float fElapsedTime);
};


#endif // !OBJECT_H