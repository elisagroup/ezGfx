/************************************************************************/
/* Purpose: Targa image support                                         */
/* Author:  Zviadi Varshanidze <elisagroup@gmail.com>                   */
/* Date:    01.11.2008                                                  */
/************************************************************************/

#ifndef XTARGALOADER_H
#define XTARGALOADER_H

// Supports all types of Targas (indexed, rgb, grayscale, compressed or uncompressed)

struct TGAHeader;

class xTargaLoader: public xIImageLoader
{
public:
	xTargaLoader();
	~xTargaLoader();

	virtual bool canLoad(const char* extension, FILE* file) const;
	virtual unsigned char* loadImage(FILE* file, bool keepAlpha);

	virtual int getWidth() const;
	virtual int getHeight() const;

private:
	TGAHeader* m_pTgaHdr;
};

// Save the current alignment
#pragma pack(push, packing)
#pragma pack(1)
struct TGAHeader
{
	unsigned char	identsize;			// Size of ID field that follows header (0)
	unsigned char	colorMapType;		// 0 = None, 1 = paletted
	unsigned char	imageType;			// 0 = none, 1 = indexed, 2 = rgb, 3 = gray, +8=rle
	unsigned short	colorMapStart;		// First color map entry
	unsigned short	colorMapEntries;	// Number of color map entries
	unsigned char 	colorMapBits;		// bits per palette entry
	unsigned short	xstart;				// image x origin
	unsigned short	ystart;				// image y origin
	unsigned short	width;				// width in pixels
	unsigned short	height;				// height in pixels
	unsigned char	bpp;				// bits per pixel (8, 16, 24, 32)
	unsigned char	descriptor;			// image descriptor
};

struct TGAFooter
{
	unsigned long ExtensionOffset;
	unsigned long DeveloperOffset;
	char Signature[18];
};

// Restore default alignment
#pragma pack(pop, packing)

#endif	// XTARGALOADER_H
