/************************************************************************/
/* Purpose: Image loading                                               */
/* Author:	Zviadi Varshanidze <elisagroup@gmail.com>                   */
/* Date:	01.11.2008                                                  */
/************************************************************************/

#include "xImage.h"
#include "xTargaLoader.h"
#include "xJpegLoader.h"
#include "xPngLoader.h"

static const int g_ImageTypes[] = {
	xImage::IMAGE_TARGA,
	xImage::IMAGE_JPEG,
	xImage::IMAGE_PNG
};

xImage::xImage() : m_imageData(0), m_currLoader(0)
{
	// Create loaders and push them into the array
	// Later we can add a new image file format support.
	// This is the place that will need to be edited...

	m_imageLoaders.push_back( new xTargaLoader() );
	m_imageLoaders.push_back( new xJpegLoader() );
	m_imageLoaders.push_back( new xPngLoader() );
}

xImage::~xImage()
{
	// Release memory
	for (int i = 0; i < (int)m_imageLoaders.size(); i++)
	{
		delete m_imageLoaders[i];
	}

	Unload();
}

typedef unsigned int Color32;

inline Color32 color32Reverse(Color32 x)
{

	// Source is in format: 0xAABBGGRR
	// Return value is in format:  0xAARRGGBB
	return
		((x & 0xFF000000)) |       //AA______
		((x & 0x00FF0000) >> 16) | //______BB
		((x & 0x0000FF00)) |       //____GG__
		((x & 0x000000FF) << 16);  //__RR____
}

bool xImage::Load(const char* filename, bool keepAlpha /*= true*/, bool forceBGRA /*= false*/)
{
	// Unload the previous image first
	Unload();

	FILE* file;
	if ( !(file = fopen(filename, "rb")) )
	{
		return false;
	}

	const char* extension = filename, *temp = filename;
	char ch;
	while ( (ch = *temp++) != '\0' )
	{
		if (ch == '.') extension = temp;
	}

	// Loop through the loaders and find the proper one
	for (m_currLoader = 0; m_currLoader < (int)m_imageLoaders.size(); m_currLoader++)
	{
		fseek(file, 0, SEEK_SET);
		if ( m_imageLoaders[m_currLoader]->canLoad(extension, file) )
		{
			fseek(file, 0, SEEK_SET);
			m_imageData = m_imageLoaders[m_currLoader]->loadImage(file, keepAlpha);
			fclose(file);

			if (m_imageData != 0 && forceBGRA)
			{
				unsigned int* pixels = reinterpret_cast<unsigned int*>(m_imageData);

				int numPixels = getWidth() * getHeight();
				for (int i = 0; i < numPixels; i++)
				{
					pixels[i] = color32Reverse(pixels[i]);
				}
			}

			return (m_imageData != 0);
		}
	}

	fclose(file);
	return false;
}

void xImage::Unload()
{
	if (isLoaded())
	{
		delete[] m_imageData;
		m_imageData = 0;
	}
}

int xImage::GetImageType() const
{
	return g_ImageTypes[m_currLoader];
}

