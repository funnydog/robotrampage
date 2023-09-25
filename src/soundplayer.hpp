#pragma once

#include <filesystem>
#include <list>

#include <glm/glm.hpp>

#include "audiodevice.hpp"
#include "resources.hpp"
#include "resourceholder.hpp"
#include "soundbuffer.hpp"
#include "sound.hpp"

class SoundBuffer;

class SoundPlayer
{
public:
	explicit SoundPlayer(AudioDevice &audioDevice);

	void load(SoundID soundID, const std::filesystem::path &filename);

	void play(SoundID soundID);
	void play(SoundID soundID, glm::vec3 position);

	void removeStoppedSounds();

private:
	AudioDevice &mAudioDevice;
	SoundBufferHolder mSoundBuffers;

	std::list<Sound> mSounds;
};
