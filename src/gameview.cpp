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
	, mSprite1(
		context.textures->get(TextureID::SpriteSheet), {{0, 64}, {32, 32}},
		glm::vec2(100.f, 100.f), glm::vec2(0.f))
	, mSprite2(
		context.textures->get(TextureID::SpriteSheet), {{0, 160}, {32, 32}},
		glm::vec2(200.f, 200.f), glm::vec2(0.f))
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
	glm::vec2 spriteMove(0.f);
	glm::vec2 cameraMove(0.f);

	if (mWindow.isKeyPressed(GLFW_KEY_A)) { spriteMove.x -= 1.f; }
	if (mWindow.isKeyPressed(GLFW_KEY_D)) { spriteMove.x += 1.f; }
	if (mWindow.isKeyPressed(GLFW_KEY_W)) { spriteMove.y -= 1.f; }
	if (mWindow.isKeyPressed(GLFW_KEY_S)) { spriteMove.y += 1.f; }

	if (mWindow.isKeyPressed(GLFW_KEY_LEFT))  { cameraMove.x -= 1.f; }
	if (mWindow.isKeyPressed(GLFW_KEY_RIGHT)) { cameraMove.x += 1.f; }
	if (mWindow.isKeyPressed(GLFW_KEY_UP))    { cameraMove.y -= 1.f; }
	if (mWindow.isKeyPressed(GLFW_KEY_DOWN))  { cameraMove.y += 1.f; }

	mCamera.move(cameraMove);
	mSprite1.setVelocity(spriteMove * 60.f);

	mSprite1.update(dt);
	mSprite2.update(dt);

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
	mSprite1.draw(target);
	mSprite2.draw(target);
	target.draw();
}
