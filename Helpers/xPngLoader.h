/************************************************************************/
/* Purpose: PNG image support                                           */
/* Author:  Zviadi Varshanidze <elisagroup@gmail.com>                   */
/* Date:    10.03.2016                                                  */
/* Note:    Adopted from C source code written by Lode Vandevenne      */
/************************************************************************/

#ifndef XPNGLOADER_H
#define XPNGLOADER_H

class xPngLoader : public xIImageLoader
{
public:
	xPngLoader();
	~xPngLoader();

	virtual bool canLoad(const char* extension, FILE* file) const;
	virtual unsigned char* loadImage(FILE* file, bool keepAlpha);

	virtual int getWidth() const;
	virtual int getHeight() const;

private:
	int m_width;
	int m_height;
};

#endif	// XPNGLOADER_H
