#include "alcheck.hpp"
#include "soundplayer.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace
{
const float Attenuation = 8.f;
const float ListenerZ = 300.f;
const float MinDistance2D = 200.f;
const float MinDistance3D = std::sqrt(MinDistance2D*MinDistance2D + ListenerZ*ListenerZ);
}

SoundPlayer::SoundPlayer(AudioDevice &audioDevice)
	: mAudioDevice(audioDevice)
	, mSoundBuffers()
	, mSounds()
{
}

void
SoundPlayer::load(SoundID soundID, const std::filesystem::path &filename)
{
	mSoundBuffers.load(soundID, filename);
}

void
SoundPlayer::play(SoundID soundID)
{
	auto &buffer = mSoundBuffers.get(soundID);

	auto &sound = mSounds.emplace_back(buffer);
	sound.play();
}

void
SoundPlayer::play(SoundID soundID, glm::vec3 position)
{
	auto &buffer = mSoundBuffers.get(soundID);

	auto &sound = mSounds.emplace_back(buffer);
	sound.setPosition(position);
	sound.setAttenuation(Attenuation);
	sound.setMinDistance(MinDistance3D);
	sound.play();
}

void
SoundPlayer::removeStoppedSounds()
{
	mSounds.remove_if([](const auto &sound) {
		return sound.getStatus() == SoundSource::Stopped;
	});
}
