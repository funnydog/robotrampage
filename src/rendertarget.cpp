#include <iostream>
#include <cassert>

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "rendertarget.hpp"

#include "color.hpp"
#include "font.hpp"
#include "glcheck.hpp"
#include "sprite.hpp"
#include "tilemap.hpp"
#include "utility.hpp"
#include "window.hpp"

namespace
{
static const glm::vec2 QuadUnits[4] = {
	{ 0.f, 0.f },
	{ 0.f, 1.f },
	{ 1.f, 0.f },
	{ 1.f, 1.f },
};

static const std::uint16_t QuadIndices[] = { 0, 1, 2, 1, 3, 2 };
}

RenderTarget::RenderTarget()
	: mCamera(&mDefaultCamera)
	, mTexture(nullptr)
	, mVertexOffset(0)
	, mIndexOffset(0)
	, mVertexCount(0)
	, mIndexCount(0)
	, mVBO(0)
	, mEBO(0)
	, mVAO(0)
{
}

RenderTarget::~RenderTarget()
{
	if (mVAO)
	{
		glCheck(glBindVertexArray(0));
		glCheck(glDeleteVertexArrays(1, &mVAO));
	}
	if (mEBO)
	{
		glCheck(glDeleteBuffers(1, &mEBO));
	}
	if (mVBO)
	{
		glCheck(glDeleteBuffers(1, &mVBO));
	}
}

void
RenderTarget::use(const Window &window)
{
	mWhiteTexture.create(1, 1, &Color::White);
	mShader.create();
	if (!mShader.attachFile(Shader::Type::Vertex, "assets/shaders/pos_uv_color.vs")
	    || !mShader.attachFile(Shader::Type::Fragment, "assets/shaders/uv_color.fs")
	    || !mShader.link())
	{
		throw std::runtime_error("Cannot compile the shader");
	}

	glm::vec2 size = window.getSize();
	mDefaultCamera.setPosition({0.f, 0.f});
	mDefaultCamera.setSize(size);
	mCamera = &mDefaultCamera;

	mShader.use();
	mShader.getUniform("projection").setMatrix4(mCamera->getTransform());
	mShader.getUniform("image").setInteger(0);

	glCheck(glEnable(GL_CULL_FACE));
	glCheck(glEnable(GL_BLEND));
	glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	// allocate the EBO and VBO
	glCheck(glGenBuffers(1, &mEBO));
	glCheck(glGenBuffers(1, &mVBO));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, mVBO));

	// allocate and configure the VAO
	glCheck(glGenVertexArrays(1, &mVAO));
	glCheck(glBindVertexArray(mVAO));
		glCheck(glEnableVertexAttribArray(0));
	glCheck(glVertexAttribPointer(
			0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			reinterpret_cast<GLvoid*>(offsetof(Vertex, pos))));
	glCheck(glEnableVertexAttribArray(1));
	glCheck(glVertexAttribPointer(
			1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			reinterpret_cast<GLvoid*>(offsetof(Vertex, uv))));
	glCheck(glEnableVertexAttribArray(2));
	glCheck(glVertexAttribPointer(
			2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex),
			reinterpret_cast<GLvoid*>(offsetof(Vertex, color))));

	glCheck(glBindVertexArray(0));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

const Camera&
RenderTarget::getDefaultCamera() const
{
	return mDefaultCamera;
}

const Camera&
RenderTarget::getCamera() const
{
	return *mCamera;
}

void
RenderTarget::setCamera(const Camera &view)
{
	mCamera = &view;
}

void
RenderTarget::clear(Color color)
{
	glm::vec4 clearColor(color);
	glCheck(glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a));
	glCheck(glClear(GL_COLOR_BUFFER_BIT));
}

void
RenderTarget::addLayer()
{
	endBatch();
}

void
RenderTarget::beginBatch()
{
	mBatches.clear();
	mVertices.clear();
	mIndices.clear();
	mTexture = &mWhiteTexture;
	mVertexOffset = mIndexOffset = 0;
	mVertexCount = mIndexCount = 0;
}

void
RenderTarget::endBatch()
{
	mBatches.emplace_back(
		mTexture,
		mVertexOffset,
		mIndexOffset,
		mIndexCount-mIndexOffset);
	mVertexOffset = mVertexCount;
	mIndexOffset = mIndexCount;
}

void
RenderTarget::reserve(unsigned vertices, std::span<const std::uint16_t> indices)
{
	auto base = mVertices.size() - mVertexOffset;
	if (base + vertices > UINT16_MAX)
	{
		endBatch();
		base = 0;
	}
	mVertexCount += vertices;
	for (auto i : indices)
	{
		mIndices.push_back(base + i);
	}
	mIndexCount += indices.size();
}

void
RenderTarget::draw() const
{
	assert(mVAO && mVBO && mEBO && "OpenGL objects not initialized.");

	glCheck(glBindVertexArray(mVAO));

	glCheck(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
	glCheck(glBufferData(GL_ARRAY_BUFFER,
			     mVertices.size() * sizeof(mVertices[0]),
			     mVertices.data(),
			     GL_STREAM_DRAW));

	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO));
	glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			     mIndices.size() * sizeof(mIndices[0]),
			     mIndices.data(),
			     GL_STREAM_DRAW));

	for (const auto &batch : mBatches)
	{
		batch.texture->bind(0);
		glCheck(glDrawElementsBaseVertex(
				GL_TRIANGLES,
				batch.indexCount,
				GL_UNSIGNED_SHORT,
				reinterpret_cast<GLvoid*>(batch.indexOffset * sizeof(mIndices[0])),
				batch.vertexOffset));
	}
	glCheck(glBindVertexArray(0));
}

void
RenderTarget::setTexture(const Texture *texture)
{
	if (texture == nullptr)
	{
		texture = &mWhiteTexture;
	}
	if (texture != mTexture && mIndices.size() > mIndexOffset)
	{
		endBatch();
	}
	mTexture = texture;
}

void
RenderTarget::draw(const std::string &text, Font &font, glm::vec2 pos, Color color)
{
	if (text.empty())
	{
		return;
	}

	setTexture(&font.getTexture());
	pos.y += font.getLineHeight();
	auto codepoints = Utility::decodeUTF8(text);
	for (auto codepoint : codepoints)
	{
		reserve(4, QuadIndices);

		const auto &glyph = font.getGlyph(codepoint);
		pos.x += glyph.bearing.x;
		pos.y -= glyph.bearing.y;
		for (auto unit : QuadUnits)
		{
			mVertices.emplace_back(
				glyph.size * unit + pos,
				glyph.uvSize * unit + glyph.uvPos,
				color);
		}
		pos.x += glyph.advance - glyph.bearing.x;
		pos.y += glyph.bearing.y;
	}
}

void
RenderTarget::draw(const Sprite &sprite)
{
	setTexture(&sprite.getTexture());
	reserve(4, QuadIndices);
	const auto &uvRect = sprite.getSource();
	FloatRect dstRect = sprite.getDestination();
	Color color = sprite.getTintColor();
	float rotation = sprite.getRotation();
	if (rotation == 0.f)
	{
		for (auto unit : QuadUnits)
		{
			mVertices.emplace_back(
				unit * dstRect.size + dstRect.pos,
				unit * uvRect.size + uvRect.pos,
				color);
		}
	}
	else
	{
		glm::vec2 offset = sprite.getSize() * .5f;
		auto mat4 = glm::translate(
			glm::rotate(
				glm::translate(glm::mat4(1.f), glm::vec3(offset, 0.f)),
				rotation,
				glm::vec3(0.f, 0.f, 1.f)),
			glm::vec3(-offset, 0.f));
		for (auto unit : QuadUnits)
		{
			mVertices.emplace_back(
				glm::vec2(mat4 * glm::vec4(unit * dstRect.size, 0.f, 1.f)) + dstRect.pos,
				unit * uvRect.size + uvRect.pos,
				color);

		}
	}
}

void
RenderTarget::draw(const TileMap &map)
{
	glm::vec2 cameraStart = mCamera->getPosition();
	glm::vec2 cameraEnd = cameraStart + mCamera->getSize();
	glm::ivec2 start = map.getSquareByPixel(cameraStart);
	glm::ivec2 end = map.getSquareByPixel(cameraEnd);

	setTexture(&map.getTexture());
	glm::ivec2 cur;
	for (cur.x = start.x; cur.x <= end.x; cur.x++)
	{
		for (cur.y = start.y; cur.y <= end.y; cur.y++)
		{
			int tile = map.getTileAtSquare(cur);
			if (tile == -1)
			{
				continue;
			}
			auto dstRect = map.getSquareRectangle(cur);
			const auto &uvRect = map.getTileUVRect(tile);
			reserve(4, QuadIndices);
			for (auto unit : QuadUnits)
			{
				mVertices.emplace_back(
					unit * dstRect.size + dstRect.pos,
					unit * uvRect.size + uvRect.pos,
					Color::White);
			}
		}
	}
}
