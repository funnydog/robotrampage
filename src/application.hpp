#pragma once

#include "audiodevice.hpp"
#include "eventqueue.hpp"
#include "font.hpp"
#include "rendertarget.hpp"
#include "resourceholder.hpp"
#include "resources.hpp"
#include "soundplayer.hpp"
#include "texture.hpp"
#include "viewstack.hpp"
#include "window.hpp"

class Application
{
public:
	Application();
	~Application();

	void run();

private:
	void loadAssets();
	void registerViews();
	void processInput();

private:
	EventQueue mEventQueue;
	Window mWindow;
	RenderTarget mRenderTarget;
	AudioDevice mAudioDevice;
	SoundPlayer mSoundPlayer;
	FontHolder mFonts;
	TextureHolder mTextures;
	ViewStack mViewStack;
};
