
#ifndef EZGFXPLUS_IMAGE_H
#define EZGFXPLUS_IMAGE_H

#include "ezGfxPlus.h"
#include "ezGfxPlusRefCounted.h"

namespace ezGfxPlus
{
	class EZGFXPLUS_API Image : public RefCounted
	{
	public:
		Image();
		Image(const std::string& filePath);
		Image(const Image& other);
		~Image();

		bool load(const std::string& filePath);
		int getWidth() const;
		int getHeight() const;

		Image& operator=(const Image& other);

	private:
		friend class Canvas;
		void* m_imageRef;
	};
}

#endif // EZGFXPLUS_IMAGE_H