/************************************************************************/
/* Purpose:	Image loading                                               */
/* Author:	Zviadi Varshanidze <elisagroup@gmail.com>                   */
/* Date:	01.11.2008                                                  */
/************************************************************************/

#ifndef XIMAGE_H
#define XIMAGE_H

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <vector>


// Image loader interface
class xIImageLoader
{
public:
	virtual bool canLoad(const char* extension, FILE* file) const = 0;
	virtual unsigned char* loadImage(FILE* file, bool keepAlpha) = 0;

	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;
};

class xImage
{
public:
	xImage();
	~xImage();

	bool isLoaded() const;
	int getWidth() const;
	int getHeight() const;
	unsigned char* getData() const;

	enum ImageType
	{
		IMAGE_TARGA,
		IMAGE_JPEG,
		IMAGE_PNG
	};

	int GetImageType() const;

	bool Load(const char* filename, bool keepAlpha = true, bool forceBGRA = false);
	void Unload();
	
private:
	std::vector <xIImageLoader*> m_imageLoaders;
	int m_currLoader;
	unsigned char* m_imageData;
};

inline bool xImage::isLoaded() const
{
	return (m_imageData != 0);
}

inline int xImage::getWidth() const
{
	return m_imageLoaders[m_currLoader]->getWidth();
}

inline int xImage::getHeight() const
{
	return m_imageLoaders[m_currLoader]->getHeight();
}

inline unsigned char* xImage::getData() const
{
	return m_imageData;
}

#endif	// XIMAGE_H
