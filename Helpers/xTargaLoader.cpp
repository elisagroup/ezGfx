/************************************************************************/
/* Purpose:	Targa image support                                         */
/* Author:	Zviadi Varshanidze <elisagroup@gmail.com>                   */
/* Date:	01.11.2008                                                  */
/************************************************************************/

#include "xImage.h"
#include "xTargaLoader.h"

#pragma warning(disable:4146)


xTargaLoader::xTargaLoader() : m_pTgaHdr(0)
{
}

xTargaLoader::~xTargaLoader()
{
	if (m_pTgaHdr)
		delete m_pTgaHdr;
}

bool xTargaLoader::canLoad(const char* extension, FILE* file) const
{
	TGAFooter footer;
	fseek(file, -sizeof(TGAFooter), SEEK_END);
	if ( fread(&footer, sizeof(TGAFooter), 1, file) != 0 )
	{
		if ( strcmp(footer.Signature, "TRUEVISION-XFILE.") == 0 )
			return true;
	}
	return false;
}

unsigned char* xTargaLoader::loadImage(FILE* file, bool keepAlpha)
{
	// Allocate buffer for the header
	m_pTgaHdr = new TGAHeader;

	if ( fread(m_pTgaHdr, sizeof(TGAHeader), 1, file) == 0 )
		return 0;

	// Skip image identification field if present
	if (m_pTgaHdr->identsize != 0)
		fseek(file, SEEK_CUR, m_pTgaHdr->identsize);

	unsigned char* pPalette;
	// Read palette if present
	if (m_pTgaHdr->colorMapEntries > 0)
	{
		int paletteBytes = m_pTgaHdr->colorMapEntries * m_pTgaHdr->colorMapBits / 8;
		pPalette = new unsigned char[paletteBytes];
		if ( !pPalette || fread(pPalette, paletteBytes, 1, file) == 0 )
		{
			delete[] pPalette;
			return 0;
		}
	}

	int curPos = ftell(file);
	fseek(file, -sizeof(TGAFooter), SEEK_END);
	int bytesToRead = ftell(file) - curPos;
	fseek(file, curPos, SEEK_SET);
	unsigned char* pData = new unsigned char[bytesToRead];
	if ( fread(pData, bytesToRead, 1, file) == 0 )
		return 0;

	bool imageReversed = (m_pTgaHdr->descriptor & 0x20) == 0;
	int pixelSize = m_pTgaHdr->bpp / 8;
	int imageSize = m_pTgaHdr->width * m_pTgaHdr->height * pixelSize;

	unsigned char *pSrc, *pDest;

	if (m_pTgaHdr->imageType & 0x8)
	{
		// RLE compressed...
		unsigned char* pTempBuffer = new unsigned char[imageSize];
		unsigned char hdrByte;
		pDest = pTempBuffer;
		pSrc = pData;

		while (imageSize > 0)
		{
			hdrByte = *pSrc++;	// Read packet header
			int count = (hdrByte & 0x7F) + 1;
			imageSize -= count * pixelSize;

			// Check the top bit (0 - Raw, 1 - RLE)
			if (hdrByte & 0x80)	// RLE
			{
				while (count--)
				{
					memcpy(pDest, pSrc, pixelSize);
					pDest += pixelSize;
				}
				pSrc += pixelSize;
			}
			else	// Raw data
			{
				count *= pixelSize;
				memcpy(pDest, pSrc, count);
				pSrc += count;
				pDest += count;
			}
		}

		delete[] pData;
		pSrc = pTempBuffer;
	}
	else
	{
		pSrc = pData;
	}

	pixelSize = keepAlpha ? 4 : 3;
	imageSize = m_pTgaHdr->width * m_pTgaHdr->height * pixelSize;
	unsigned char *pComp, *pPixels = new unsigned char[imageSize];
	unsigned short* p;
	pDest = pPixels;
	pComp = pSrc;

	int inc = 0;

	// Convert to 24 or 32 bpp
	switch (m_pTgaHdr->bpp)
	{
	case 8:
		if ( (m_pTgaHdr->imageType & 0x3) == 1)
		{
			// Indexed
			for (int y = 0; y < m_pTgaHdr->height; ++y)
			{
				const int rowIdx = imageReversed ? m_pTgaHdr->height - y - 1 : y;
				pDest = pPixels + rowIdx * m_pTgaHdr->width * pixelSize;

				for (int x = 0; x < m_pTgaHdr->width; ++x)
				{
					const int destPixelIdx = x * pixelSize;

					int palIndex = *pComp * 3;
					pDest[destPixelIdx + 0] = pPalette[palIndex + 2];
					pDest[destPixelIdx + 1] = pPalette[palIndex + 1];
					pDest[destPixelIdx + 2] = pPalette[palIndex];
					if (keepAlpha) pDest[destPixelIdx + 3] = 0xFF;	// Alpha
					pComp++;
				}
			}
		}
		else
		{
			// Grayscale
			for (int y = 0; y < m_pTgaHdr->height; ++y)
			{
				const int rowIdx = imageReversed ? m_pTgaHdr->height - y - 1 : y;
				pDest = pPixels + rowIdx * m_pTgaHdr->width * pixelSize;

				for (int x = 0; x < m_pTgaHdr->width; ++x)
				{
					const int destPixelIdx = x * pixelSize;

					pDest[destPixelIdx + 0] = *pComp;
					pDest[destPixelIdx + 1] = *pComp;
					pDest[destPixelIdx + 2] = *pComp;
					if (keepAlpha) pDest[destPixelIdx + 3] = 0xFF;	// Alpha
					pComp++;
				}
			}
		}
		break;
	case 16:
		p = (unsigned short*)pSrc;
		for (int y = 0; y < m_pTgaHdr->height; ++y)
		{
			const int rowIdx = imageReversed ? m_pTgaHdr->height - y - 1 : y;
			pDest = pPixels + rowIdx * m_pTgaHdr->width * pixelSize;

			for (int x = 0; x < m_pTgaHdr->width; ++x)
			{
				const int destPixelIdx = x * pixelSize;

				pDest[destPixelIdx + 0] = (((*p >> 10) & 0x1F) << 3) | 0x3;
				pDest[destPixelIdx + 1] = (((*p >> 5) & 0x1F) << 3) | 0x3;
				pDest[destPixelIdx + 2] = ((*p & 0x1F) << 3) | 0x3;
				if (keepAlpha) pDest[destPixelIdx + 3] = 0xFF;	// Alpha
				p++;
			}
		}
		break;
	case 24:
		for (int y = 0; y < m_pTgaHdr->height; ++y)
		{
			const int rowIdx = imageReversed ? m_pTgaHdr->height - y - 1 : y;
			pDest = pPixels + rowIdx * m_pTgaHdr->width * pixelSize;

			for (int x = 0; x < m_pTgaHdr->width; ++x)
			{
				const int destPixelIdx = x * pixelSize;

				// Swap R and B
				pDest[destPixelIdx + 0] = pComp[2];
				pDest[destPixelIdx + 1] = pComp[1];
				pDest[destPixelIdx + 2] = pComp[0];
				if (keepAlpha) pDest[destPixelIdx + 3] = 0xFF;	// Alpha
				pComp += 3;
			}
		}
		break;
	case 32:
		for (int y = 0; y < m_pTgaHdr->height; ++y)
		{
			const int rowIdx = imageReversed ? m_pTgaHdr->height - y - 1 : y;
			pDest = pPixels + rowIdx * m_pTgaHdr->width * pixelSize;

			for (int x = 0; x < m_pTgaHdr->width; ++x)
			{
				const int destPixelIdx = x * pixelSize;

				// Swap R and B
				pDest[destPixelIdx + 0] = pComp[2];
				pDest[destPixelIdx + 1] = pComp[1];
				pDest[destPixelIdx + 2] = pComp[0];
				if (keepAlpha) pDest[destPixelIdx + 3] = pComp[3];	// Alpha
				pComp += 4;
			}
		}
		break;
	}

	delete[] pSrc;

	return pPixels;
}


int xTargaLoader::getWidth() const
{
	if (m_pTgaHdr) return m_pTgaHdr->width;
	return 0;
}

int xTargaLoader::getHeight() const
{
	if (m_pTgaHdr) return m_pTgaHdr->height;
	return 0;
}
