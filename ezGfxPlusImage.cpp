
#include "ezGfxPlusImage.h"
#include "ezGfx.h"

using namespace std;
using namespace ezGfxPlus;


Image::Image()
{
	m_imageRef = nullptr;
}

Image::Image(const string& filePath)
{
	load(filePath);
}

Image::Image(const Image& other)
: RefCounted(other)
{
	m_imageRef = other.m_imageRef;
	addRef();
}

Image::~Image()
{
	release();

	if (refCount() == 0)
	{
		ezGfxImageRef imageRef = static_cast<ezGfxImageRef>(m_imageRef);
		ezGfxReleaseImage(imageRef);
	}
}

Image& Image::operator=(const Image& other)
{
	m_imageRef = other.m_imageRef;
	addRef();
	return *this;
}

bool Image::load(const std::string& filePath)
{
	m_imageRef = ezGfxLoadImage(filePath.c_str());
	return m_imageRef != nullptr;
}

int Image::getWidth() const
{
	ezGfxImageRef imageRef = static_cast<ezGfxImageRef>(m_imageRef);
	return ezGfxGetImageWidth(imageRef);
}

int Image::getHeight() const
{
	ezGfxImageRef imageRef = static_cast<ezGfxImageRef>(m_imageRef);
	return ezGfxGetImageHeight(imageRef);
}