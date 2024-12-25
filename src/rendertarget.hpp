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
class Sprite;
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
	void reserve(unsigned vertices, std::span<const std::uint16_t> indices);

	/**
	 * Send the blob of vertices to the GPU.
	 */
	void draw() const;

	void draw(const std::string &text, Font &font, glm::vec2 pos, Color color);
	void draw(const Sprite &sprite);
	void draw(const TileMap &map);

	/**
	 * Set the texture for the next primitive.
	 */
	void setTexture(const Texture *texture);

	/**
	 * Use the @window as a drawing backend.
	 *
	 * @param[in] window Window to use as a drawing backend.
	 */
	void use(const Window &window);

protected:
	void initialize();

private:
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
	unsigned mVertexCount;
	unsigned mIndexCount;

	Texture       mWhiteTexture;
	Shader        mShader;
	unsigned      mVBO;
	unsigned      mEBO;
	unsigned      mVAO;
};
