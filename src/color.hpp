#pragma once

#include <cstdint>

#include <glm/glm.hpp>

class Color
{
public:
	constexpr Color();
	constexpr Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255);

	constexpr void setRed(std::uint8_t value);
	constexpr void setGreen(std::uint8_t value);
	constexpr void setBlue(std::uint8_t value);
	constexpr void setAlpha(std::uint8_t value);

	constexpr operator std::uint32_t() const;
	constexpr operator glm::vec4() const;

	constexpr Color& operator*=(float intensity);

public:
	static const Color Black;
	static const Color White;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color Yellow;
	static const Color Magenta;
	static const Color Cyan;
	static const Color Transparent;

public:
	std::uint8_t r;
	std::uint8_t g;
	std::uint8_t b;
	std::uint8_t a;

	friend constexpr Color mix(Color x, Color y, float a);
};

constexpr
Color::Color()
	: Color(255,255,255)
{
}

constexpr
Color::Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
	: r(r)
	, g(g)
	, b(b)
	, a(a)
{
}

constexpr
Color::operator std::uint32_t() const
{
	return static_cast<std::uint32_t>(r)
		| static_cast<std::uint32_t>(g) << 8
		| static_cast<std::uint32_t>(b) << 16
		| static_cast<std::uint32_t>(a) << 24;
}

constexpr
Color::operator glm::vec4() const
{
	return {
		static_cast<float>(r) / 255.0f,
		static_cast<float>(g) / 255.0f,
		static_cast<float>(b) / 255.0f,
		static_cast<float>(a) / 255.0f
	};
}

constexpr Color&
Color::operator*=(float intensity)
{
	r = static_cast<uint8_t>(r * intensity);
	g = static_cast<uint8_t>(g * intensity);
	b = static_cast<uint8_t>(b * intensity);
	return *this;
}

constexpr Color
mix(Color x, Color y, float a)
{
	x.r = static_cast<uint8_t>(x.r * (1.0 - a) + y.r * a);
	x.g = static_cast<uint8_t>(x.g * (1.0 - a) + y.g * a);
	x.b = static_cast<uint8_t>(x.b * (1.0 - a) + y.b * a);
	x.a = static_cast<uint8_t>(x.a * (1.0 - a) + y.a * a);
	return x;
}


constexpr Color Color::Black(0, 0, 0);
constexpr Color Color::White(255, 255, 255);
constexpr Color Color::Red(255, 0, 0);
constexpr Color Color::Green(0, 255, 0);
constexpr Color Color::Blue( 0, 0, 255);
constexpr Color Color::Yellow(255, 255, 0);
constexpr Color Color::Magenta(255, 0, 255);
constexpr Color Color::Cyan(0, 255, 255);
constexpr Color Color::Transparent(0, 0, 0, 0);
