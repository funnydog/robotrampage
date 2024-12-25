#include <iostream>
#include <cassert>

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "color.hpp"
#include "glcheck.hpp"
#include "rendertarget.hpp"
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
}

void
RenderTarget::endBatch()
{
	auto indexCount = mIndices.size();
	mBatches.emplace_back(
		mTexture,
		mVertexOffset,
		mIndexOffset,
		indexCount-mIndexOffset);
	mVertexOffset = mVertices.size();
	mIndexOffset = indexCount;
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
RenderTarget::reserve(unsigned vertices, std::span<const std::uint16_t> indices)
{
	auto base = mVertices.size() - mVertexOffset;
	if (base + vertices > UINT16_MAX)
	{
		endBatch();
		base = 0;
	}
	for (auto i : indices)
	{
		mIndices.push_back(base + i);
	}
}

std::uint16_t
RenderTarget::getPrimIndex(unsigned idxCount, unsigned vtxCount)
{
	// ensure we have enough space for the indices
	auto base = mVertices.size() - mVertexOffset;
	if (base + vtxCount > UINT16_MAX)
	{
		endBatch();
		base = 0;
	}
	// reserve the space for the vertices and the indices
	mVertices.reserve(mVertices.size() + vtxCount);
	mIndices.reserve(mIndices.size() + idxCount);
	return base;
}

Vertex*
RenderTarget::getVertexArray(unsigned vtxCount)
{
	auto size = mVertices.size();
	mVertices.resize(size + vtxCount);
	return &mVertices[size];
}

void
RenderTarget::blitQuad(const Texture &texture,
                       const FloatRect &srcRect,
                       const FloatRect &dstRect,
                       Color color)
{
	if (mCamera->isVisible(dstRect))
	{
		setTexture(&texture);
		reserve(4, QuadIndices);
		for (auto unit : QuadUnits)
		{
			mVertices.emplace_back(
				unit * dstRect.size + dstRect.pos,
				unit * srcRect.size + srcRect.pos,
				color);
		}
	}
}

void
RenderTarget::blitQuad(const Texture &texture,
                       const FloatRect &srcRect,
                       const FloatRect &dstRect,
                       Color color,
                       float rotationAngle,
                       glm::vec2 offset,
                       float scale)
{
	auto mat4 = glm::translate(
		glm::rotate(
			glm::translate(
				glm::scale(
					glm::mat4(1.f),
					glm::vec3(scale, scale, 1.f)),
				glm::vec3(offset, 0.f)),
			rotationAngle,
			glm::vec3(0.f, 0.f, 1.f)),
		glm::vec3(-offset, 0.f));

	setTexture(&texture);
	reserve(4, QuadIndices);
	for (auto unit : QuadUnits)
	{
		mVertices.emplace_back(
			glm::vec2(
				mat4 * glm::vec4(unit * dstRect.size, 0.f, 1.f)) + dstRect.pos,
			unit * srcRect.size + srcRect.pos,
			color);
	}
}
