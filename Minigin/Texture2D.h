#ifndef GAME_ENGINE_TEXTURE2D
#define GAME_ENGINE_TEXTURE2D

#include <glm/vec2.hpp>

struct SDL_Texture;
namespace engine
{
	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class Texture2D final
	{
	public:
		SDL_Texture* GetSDLTexture() const;
		explicit Texture2D(SDL_Texture* texture);
		~Texture2D();

		glm::ivec2 GetSize() const;
		void ChangeScale();

		Texture2D(const Texture2D&) = delete;
		Texture2D(Texture2D&&) = delete;
		Texture2D& operator= (const Texture2D&) = delete;
		Texture2D& operator= (const Texture2D&&) = delete;
	private:
		SDL_Texture* m_texture;
	};
}

#endif
