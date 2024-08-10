#include "Object.h"
#include "Game.h"

Object::Object()
{
}

Object::Object(const olc::vf2d& position, const olc::Pixel& color, const olc::vi2d& size)
{
	this->position = position;	
	this->color = color;
	this->size = size;
}

void Object::Draw()
{
	game->FillRectDecal(position * size, size, color);
}

void Object::Draw(const olc::vi2d& position, const olc::Pixel& color)
{
	game->FillRectDecal(position * size, size, color);
}

Player::Player() : Object()
{
}

Player::Player(const olc::vf2d& position, const olc::vf2d& speed, const olc::Pixel& color, const olc::vi2d& size)
	: Object(position, color, size)
{
	mSprite = new olc::Sprite("assets/images/PlayerSpriteSheet.png");
	mDecal = new olc::Decal(mSprite);

	mTileID = olc::vi2d(0, 0);

	mTimeCounter = 0.0f;
	mTimeBetweenFrames = 0.05f;

	this->speed = speed;
	mOnGround = false;

	isDead = false;
}

void Player::Behaviour(float fElapsedTime)
{
	if (game->GetKey(olc::A).bHeld && !game->GetKey(olc::D).bHeld && !isDead)
	{
		velocity.x = (mOnGround) ? -speed.x : -speed.x * 0.5f;

		mTimeCounter += fElapsedTime;

		if (mTimeCounter >= mTimeBetweenFrames)
		{
			mTimeCounter -= mTimeBetweenFrames;
			if (mTileID.x == 24)
				mTileID.x = 2;
			else
				mTileID.x += 2;
		}

		mTileID.y = 2;
	}
	if (game->GetKey(olc::D).bHeld && !game->GetKey(olc::A).bHeld && !isDead)
	{
		velocity.x = (mOnGround) ? speed.x : speed.x * 0.5f;

		mTimeCounter += fElapsedTime;

		if (mTimeCounter >= mTimeBetweenFrames)
		{
			mTimeCounter -= mTimeBetweenFrames;
			if (mTileID.x == 24)
				mTileID.x = 2;
			else
				mTileID.x += 2;
		}

		mTileID.y = 0;
	}

	if ((game->GetKey(olc::A).bReleased) || (game->GetKey(olc::D).bReleased) ||
		(game->GetKey(olc::A).bHeld && game->GetKey(olc::D).bHeld))
			mTileID.x = 0;

	if (game->GetKey(olc::SPACE).bPressed && velocity.y == 0.0f) 
		velocity.y = -speed.y;

	if (mOnGround)
	{
		velocity.x += -20.0f * velocity.x * fElapsedTime;
		if (fabs(velocity.x) < 0.01f)
			velocity.x = 0.0f;
	}

	velocity.y += 20.0f * fElapsedTime;

	if (velocity.x > 10.0f)
		velocity.x = 10.0f;
	if (velocity.x < -10.0f)
		velocity.x = -10.0f;

	if (velocity.y > 100.0f)
		velocity.y = 100.0f;
	if (velocity.y < -100.0f)
		velocity.y = -100.0f;

	olc::vf2d newPos = position + velocity * fElapsedTime;	

	if (CoinPickedUp(olc::vi2d(newPos.x + 0.0f, newPos.y + 0.0f)))
	{
		CoinPointUp(olc::vi2d(newPos.x + 0.0f, newPos.y + 0.0f), game->coinTypes);		
	}

	if (CoinPickedUp(olc::vi2d(newPos.x + 0.0f, newPos.y + 2.0f)))
	{
		CoinPointUp(olc::vi2d(newPos.x + 0.0f, newPos.y + 2.0f), game->coinTypes);
	}

	if (CoinPickedUp(olc::vi2d(newPos.x + 2.0f, newPos.y + 0.0f)))
	{
		CoinPointUp(olc::vi2d(newPos.x + 2.0f, newPos.y + 0.0f), game->coinTypes);
	}

	if (CoinPickedUp(olc::vi2d(newPos.x + 2.0f, newPos.y + 2.0f)))
	{
		CoinPointUp(olc::vi2d(newPos.x + 2.0f, newPos.y + 2.0f), game->coinTypes);
	}

	mOnGround = false;
	if (velocity.x <= 0) // Moving Left
	{
		if (game->map->IsSolid(newPos.x + 0.0f, position.y + 0.0f) || game->map->IsSolid(newPos.x + 0.0f, position.y + 1.9f))
		{
			newPos.x = (int)newPos.x + 1;
			velocity.x = 0;
		}
	}
	else // Moving Right
	{
		if (game->map->IsSolid(newPos.x + 2.0f, position.y + 0.0f) || game->map->IsSolid(newPos.x + 2.0f, position.y + 1.9f))
		{
			newPos.x = (int)newPos.x;
			velocity.x = 0;
		}
	}

	if (velocity.y <= 0) // Moving Up
	{
		if (game->map->IsSolid(position.x + 0.0f, newPos.y + 0.0f) || game->map->IsSolid(position.x + 1.9f, newPos.y + 0.0f))
		{
			newPos.y = (int)newPos.y + 1;
			velocity.y = 0;
		}
	}
	else // Moving Down
	{
		if (game->map->IsSolid(position.x + 0.0f, newPos.y + 2.0f) || game->map->IsSolid(position.x + 1.9f, newPos.y + 2.0f))
		{
			newPos.y = (int)newPos.y;
			velocity.y = 0;
			mOnGround = true;
		}
	}

	position = newPos;

	if (position.x < 0.0f) position.x = 0.0f;
	if (position.x > game->map->width - 3) position.x = game->map->width - 3;

	if (position.y > game->map->height)
	{
		isDead = true;
		velocity.x = 0.0f;
		if (position.y >= game->map->height + 100)
		{
			isDead = false;
			position = { 1.0f, 8.0f };
			velocity = { 0.0f, 0.0f };

			game->energyCounter = 0;

			game->map->Reset();
		}
	}
}

void Player::Draw()
{
	if (game->energyCounter >= 10)
	{
		game->DrawPartialDecal((position - game->map->camera->offset) * size, size, mDecal, (olc::vf2d(0, 0) + mTileID) * size, size);
		game->DrawPartialDecal(olc::vf2d((position.x + 1.0f) - game->map->camera->offset.x, position.y - game->map->camera->offset.y) * size, size, mDecal, (olc::vf2d(1, 0) + mTileID) * size, size);
		game->DrawPartialDecal(olc::vf2d(position.x - game->map->camera->offset.x, (position.y + 1.0f) - game->map->camera->offset.y) * size, size, mDecal, (olc::vf2d(0, 1) + mTileID) * size, size);
		game->DrawPartialDecal(olc::vf2d((position.x + 1.0f) - game->map->camera->offset.x, (position.y + 1.0f) - game->map->camera->offset.y) * size, size, mDecal, (olc::vf2d(1, 1) + mTileID) * size, size);
	}
	else
	{
		game->FillRectDecal((position - game->map->camera->offset) * size, size, color);
		game->FillRectDecal(olc::vf2d((position.x + 1.0f) - game->map->camera->offset.x, position.y - game->map->camera->offset.y) * size, size, color);
		game->FillRectDecal(olc::vf2d(position.x - game->map->camera->offset.x, (position.y + 1.0f) - game->map->camera->offset.y) * size, size, color);
		game->FillRectDecal(olc::vf2d((position.x + 1.0f) - game->map->camera->offset.x, (position.y + 1.0f) - game->map->camera->offset.y) * size, size, color);
	}	
}

bool Player::CoinPickedUp(const olc::vi2d& newPos)
{
	return game->map->GetTile(newPos) == '1' ||
		   game->map->GetTile(newPos) == '2' ||
		   game->map->GetTile(newPos) == '3' ||
		   game->map->GetTile(newPos) == '4';
}

void Player::CoinPointUp(const olc::vi2d& newPos, std::array<int, 4>& coinType)
{
	if (game->map->GetTile(newPos) == '1')
		coinType[0]++;
	else if (game->map->GetTile(newPos) == '2')
		coinType[1]++;
	else if (game->map->GetTile(newPos) == '3')
		coinType[2]++;
	else if (game->map->GetTile(newPos) == '4')
		coinType[3]++;

	game->map->SetTile(newPos, '.');
	game->energyCounter++;
}

olc::Sprite* CoinPickUp::sSprite = nullptr;
olc::Decal* CoinPickUp::sDecal = nullptr;

olc::vi2d CoinPickUp::sTileID = {0, 0};
float CoinPickUp::sTimeCounter = 0.0f;
float CoinPickUp::sTimeBetweenFrames = 0.05f;

void CoinPickUp::CreateSprite()
{
	sSprite = new olc::Sprite("assets/images/Coins/Full Coins.png");
	sDecal = new olc::Decal(sSprite);
}

void CoinPickUp::DeleteSprite()
{
	delete sDecal;
	delete sSprite;
}

void CoinPickUp::Animation(float fElapsedTime)
{
	sTimeCounter += fElapsedTime;

	if (sTimeCounter >= sTimeBetweenFrames)
	{
		sTimeCounter -= sTimeBetweenFrames;
		if (sTileID.x == 7)
			sTileID.x = 0;
		else
			sTileID.x++;
	}
}
