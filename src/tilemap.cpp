#include <iostream>
#include <cassert>

#include "rendertarget.hpp"
#include "tilemap.hpp"

TileMap::TileMap(const Texture &texture, unsigned width, unsigned height)
	: mMap(width * height, 0)
	, mMapTileSize(width, height)
	, mTexture(&texture)
	, mTextureSize(texture.getSize())
	, mTiles()
	, mTileSize(0.f)
	, mMapPixelSize(0.f)
{
}

void
TileMap::setTexture(const Texture &texture)
{
	mTexture = &texture;
}

void
TileMap::addTile(const FloatRect &tile)
{
	assert((mTileSize == glm::vec2(0.f) || tile.size == mTileSize)
	       && "Tile size different from previous tiles");

	mTileSize = tile.size;
	mTiles.emplace_back(tile.pos / mTextureSize, tile.size / mTextureSize);
}

glm::ivec2
TileMap::getSquareByPixel(glm::vec2 pixelPos) const
{
	return pixelPos / mTileSize;
}

glm::vec2
TileMap::getSquareCenter(glm::ivec2 squarePos) const
{
	return mTileSize * glm::vec2(squarePos) + mTileSize * 0.5f;
}

FloatRect
TileMap::getSquareRectangle(glm::ivec2 squarePos) const
{
	return { mTileSize * glm::vec2(squarePos), mTileSize };
}

int
TileMap::getTileAtSquare(glm::ivec2 squarePos) const
{
	if (squarePos.x < 0 || squarePos.x >= mMapTileSize.x
	    || squarePos.y < 0 || squarePos.y >= mMapTileSize.y)
	{
		return -1;
	}
	return mMap[squarePos.y * mMapTileSize.x + squarePos.x];
}

void
TileMap::setTileAtSquare(glm::ivec2 squarePos, int tile)
{
	assert(0 <= squarePos.x && squarePos.x < mMapTileSize.x
	       && 0 <= squarePos.y && squarePos.y < mMapTileSize.y
	       && "Square position out of bounds");
	mMap[squarePos.y * mMapTileSize.y + squarePos.x] = tile;
}

int
TileMap::getTileAtPixel(glm::vec2 pixelPos) const
{
	return getTileAtSquare(getSquareByPixel(pixelPos));
}

void
TileMap::setTileAtPixel(glm::vec2 pixelPos, int tile)
{
	setTileAtSquare(getSquareByPixel(pixelPos), tile);
}

bool
TileMap::isWallTile(glm::ivec2 squarePos) const
{
	int tileIndex = getTileAtSquare(squarePos);
	return tileIndex >= 4;
}

bool
TileMap::isWallTileByPixel(glm::vec2 pixelPos) const
{
	return isWallTile(getSquareByPixel(pixelPos));
}

void
TileMap::draw(RenderTarget &target)
{
	glm::vec2 cameraStart = target.getCamera().getPosition();
	glm::vec2 cameraEnd = cameraStart + target.getCamera().getSize();
	glm::ivec2 start = getSquareByPixel(cameraStart);
	glm::ivec2 end = getSquareByPixel(cameraEnd);

	glm::ivec2 cur;
	for (cur.x = start.x; cur.x <= end.x; cur.x++)
	{
		for (cur.y = start.y; cur.y <= end.y; cur.y++)
		{
			int tile = getTileAtSquare(cur);
			if (tile != -1)
			{
				target.blitQuad(
					*mTexture,
					mTiles[tile],
					getSquareRectangle(cur),
					Color::White);
			}
		}
	}
}
