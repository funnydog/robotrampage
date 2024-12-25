#pragma once

enum class ViewID
{
	None,
	Title,
	Playing,
	GameOver,
};

enum class FontID
{
	Pericles14,
};

enum class TextureID
{
	TitleScreen,
	SpriteSheet,
};

enum class SoundID
{
	Shot1,
	Shot2,
	Explosion1,
	Explosion2,
	Explosion3,
	Explosion4,
};

template<typename Resource, typename Identifier>
class ResourceHolder;

class Font;
typedef ResourceHolder<Font, FontID> FontHolder;

class Texture;
typedef ResourceHolder<Texture, TextureID> TextureHolder;
