#pragma once

#include <span>
#include <unordered_map>
#include <vector>

#include "color.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex.hpp"
#include "camera.hpp"

class Canvas;
class Font;
class TileMap;
class Window;

class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();

	RenderTarget(const RenderTarget &) = delete;
	RenderTarget(RenderTarget &&) noexcept = delete;
	RenderTarget& operator=(const RenderTarget &) = delete;
	RenderTarget& operator=(RenderTarget &&) noexcept = delete;

	/**
	 * Get the Camera associated with the RenderTarget.
	 */
	const Camera& getCamera() const;

	/**
	 * Set the Camera associated with the RenderTarget.
	 * @param[in] view
	 */
	void setCamera(const Camera &view);

	/**
	 * Get the default Camera associated with the RenderTarget.
	 */
	const Camera& getDefaultCamera() const;

	/**
	 * Clear the target with the given @color.
	 * @param[in] color
	 */
	void clear(Color = Color::Black);

	/**
	 * Force a new draw command.
	 */
	void addLayer();

	void beginBatch();
	void endBatch();

	/**
	 * Send the blob of vertices to the GPU.
	 */
	void draw() const;

	void draw(const std::string &text, Font &font, glm::vec2 pos, Color color);
	void draw(const TileMap &map);

	/**
	 * Set the texture for the next primitive.
	 */
	void setTexture(const Texture *texture);

	/**
	 * Blit the texture.
	 */
	void blitQuad(const Texture &texture,
		      const FloatRect &textureRect,
		      const FloatRect &dstRect,
		      Color color);

	void blitQuad(const Texture &texture,
		      const FloatRect &textureRect,
		      const FloatRect &dstRect,
		      Color color,
		      float rotationAngle,
		      glm::vec2 offset,
		      float scale);

	/**
	 * Use the @window as a drawing backend.
	 *
	 * @param[in] window Window to use as a drawing backend.
	 */
	void use(const Window &window);

protected:
	void initialize();

private:
	void reserve(unsigned vertices, std::span<const std::uint16_t> indices);

	struct Batch
	{
		const Texture *texture;
		unsigned vertexOffset;
		unsigned indexOffset;
		unsigned indexCount;
	};

private:
	Camera mDefaultCamera;
	const Camera *mCamera;

	std::vector<Batch>         mBatches;
	std::vector<Vertex>        mVertices;
	std::vector<std::uint16_t> mIndices;
	const Texture *mTexture;
	unsigned mVertexOffset;
	unsigned mIndexOffset;

	Texture       mWhiteTexture;
	Shader        mShader;
	unsigned      mVBO;
	unsigned      mEBO;
	unsigned      mVAO;
};
