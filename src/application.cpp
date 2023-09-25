#include <cstdint>
#include <stdexcept>

#include <GLFW/glfw3.h>

#include "application.hpp"

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
	, mSoundPlayer(mAudioDevice)
	, mFonts()
	, mTextures()
	, mViewStack({
			&mAudioDevice,
			&mSoundPlayer,
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

	loadAssets();
	registerViews();

	// TODO: push the starting view
	mViewStack.pushView(ViewID::Title);
	mViewStack.update(0.f);
}

Application::~Application()
{
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
}

void
Application::processInput()
{
	mEventQueue.poll();

	Event event;
	while (mEventQueue.pop(event))
	{
		// TODO: forward the event to the current game state
		if (const auto ep(std::get_if<KeyPressed>(&event)); ep
		    && ep->key == GLFW_KEY_ESCAPE)
		{
			mWindow.close();
		}
	}
}

void
Application::run()
{
	const std::uint64_t deltaTicks = glfwGetTimerFrequency() / targetFPS;
	std::uint64_t currentTicks = glfwGetTimerValue();
	std::uint64_t accumulator = 0;
	while (!mWindow.isClosed())
	{
		auto newTicks = glfwGetTimerValue();
		accumulator += newTicks - currentTicks;
		currentTicks = newTicks;

		int steps = MaxStepsPerFrame;
		while (steps-->0 && accumulator >= deltaTicks)
		{
			accumulator -= deltaTicks;

			processInput();

			// TODO: update the game state
		}

		// TODO: render the game state

		mWindow.display();
	}
}
