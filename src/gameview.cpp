#include <GLFW/glfw3.h>

#include "gameview.hpp"

#include "color.hpp"
#include "rendertarget.hpp"
#include "resources.hpp"
#include "resourceholder.hpp"
#include "texture.hpp"

GameView::GameView(ViewStack &stack, const Context &context)
	: mViewStack(stack)
	, mWindow(*context.window)
	, mCamera({0.f, 0.f}, context.window->getSize())
	, mPlayer(
		context.textures->get(TextureID::SpriteSheet),
		{{0, 64}, {32, 32}}, 6,
		{{0, 96}, {32, 32}}, 1,
		glm::vec2(300, 300))
	, mTileMap(
		context.textures->get(TextureID::SpriteSheet), 50, 50)
{
	mCamera.setWorldRectangle({{0.f, 0.f}, {1600.f, 1600.f}});
	context.target->setCamera(mCamera);

	mTileMap.addTile({{0.f, 0.f}, {32.f, 32.f}});
	mTileMap.addTile({{32.f, 0.f}, {32.f, 32.f}});
	mTileMap.addTile({{64.f, 0.f}, {32.f, 32.f}});
	mTileMap.addTile({{96.f, 0.f}, {32.f, 32.f}});
	mTileMap.addTile({{0.f, 32.f}, {32.f, 32.f}});
	mTileMap.addTile({{32.f, 32.f}, {32.f, 32.f}});
	mTileMap.addTile({{64.f, 32.f}, {32.f, 32.f}});
	mTileMap.addTile({{96.f, 32.f}, {32.f, 32.f}});
	mTileMap.generateRandomMap();
}

bool
GameView::update(float dt)
{
	mPlayer.update(dt);
	return true;
}

bool
GameView::handleEvent(const Event &)
{
	return false;
}

void
GameView::render(RenderTarget &target)
{
	target.clear(Color::White);
	mTileMap.draw(target);
	mPlayer.draw(target);
	target.draw();
}
