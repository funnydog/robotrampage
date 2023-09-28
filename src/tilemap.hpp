#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "rect.hpp"

class RenderTarget;
class Texture;

class TileMap
{
public:
	TileMap(const Texture &texture, unsigned width, unsigned height);

	void setTexture(const Texture &texture);
	void addTile(const FloatRect &tile);

	glm::ivec2 getSquareByPixel(glm::vec2 pixelPos) const;
	glm::vec2 getSquareCenter(glm::ivec2 squarePos) const;
	FloatRect getSquareRectangle(glm::ivec2 squarePos) const;

	int getTileAtSquare(glm::ivec2 squarePos) const;
	void setTileAtSquare(glm::ivec2 squarePos, int tile);

	int getTileAtPixel(glm::vec2 pixelPos) const;
	void setTileAtPixel(glm::vec2 pixelPos, int tile);

	bool isWallTile(glm::ivec2 squarePos) const;
	bool isWallTileByPixel(glm::vec2 pixelPos) const;

	void draw(RenderTarget &target);

private:
	std::vector<int> mMap;
	glm::ivec2 mMapTileSize;

	const Texture *mTexture;
	glm::vec2 mTextureSize;

	std::vector<FloatRect> mTiles;
	glm::vec2 mTileSize;
	glm::vec2 mMapPixelSize;
};
