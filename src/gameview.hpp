#pragma once

#include "view.hpp"
#include "viewstack.hpp"

#include "camera.hpp"
#include "player.hpp"
#include "sprite.hpp"
#include "tilemap.hpp"
#include "window.hpp"

class GameView: public View
{
public:
	GameView(ViewStack &stack, const Context &context);
	virtual ~GameView() override = default;

	virtual bool update(float dt) override;
	virtual bool handleEvent(const Event &event) override;
	virtual void render(RenderTarget &target) override;

private:
	ViewStack &mViewStack;
	Window &mWindow;
	Camera mCamera;
	Player mPlayer;
	TileMap mTileMap;
};
