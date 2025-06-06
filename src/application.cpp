#include <cstdint>
#include <stdexcept>

#include <GLFW/glfw3.h>

#include "application.hpp"

#include "gameview.hpp"

namespace
{
const std::uint64_t targetFPS = 60;
const float SecondsPerFrame = 1.f / targetFPS;
const int MaxStepsPerFrame = 5;

const unsigned ScreenWidth = 800;
const unsigned ScreenHeight = 600;
}

Application::Application()
	: mEventQueue()
	, mWindow()
	, mRenderTarget()
	, mAudioDevice()
	, mFonts()
	, mTextures()
	, mViewStack({
			&mAudioDevice,
			&mWindow,
			&mRenderTarget,
			&mFonts,
			&mTextures,
		})
{
	if (!glfwInit())
	{
		const char *error;
		glfwGetError(&error);
		throw std::runtime_error(error);
	}

	mWindow.open("RobotRampage", ScreenWidth, ScreenHeight);
	mEventQueue.track(mWindow);
	mRenderTarget.use(mWindow);

	if (!mAudioDevice.open(""))
	{
		throw std::runtime_error("Cannot open the audio device");
	}
	mAudioDevice.setMasterVolume(30.f);

	loadAssets();
	registerViews();

	// TODO: push the starting view
	mViewStack.pushView(ViewID::Playing);
	mViewStack.update(0.f);
}

Application::~Application()
{
	mTextures.destroy();
	glfwTerminate();
}

void
Application::loadAssets()
{

	mFonts.load(FontID::Pericles14, "assets/fonts/Peric.ttf", 14);
	mTextures.load(TextureID::TitleScreen, "assets/textures/TitleScreen.png");
	mTextures.load(TextureID::SpriteSheet, "assets/textures/SpriteSheet.png");
}

void
Application::registerViews()
{
	mViewStack.registerView<GameView>(ViewID::Playing);
}

void
Application::processInput()
{
	mEventQueue.poll();

	Event event;
	while (mEventQueue.pop(event))
	{
		if (mViewStack.handleEvent(event))
		{
			// event handled by the view
		}
		else if (const auto ep(std::get_if<KeyPressed>(&event)); ep
		         && ep->key == GLFW_KEY_ESCAPE)
		{
			mWindow.close();
		}
	}
}

void
Application::run()
{
	// variable-time game loop
	auto currentTime = glfwGetTime();
	while (!mWindow.isClosed() && !mViewStack.empty())
	{
		auto newTime = glfwGetTime();
		auto frameTime = newTime - currentTime;
		currentTime = newTime;

		processInput();
		mViewStack.update(frameTime);
		mAudioDevice.update();

		// render
		mViewStack.render(mRenderTarget);
		mWindow.display();
	}
}
