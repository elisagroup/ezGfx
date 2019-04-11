/************************************************************************/
/* Purpose: JPEG image support                                          */
/* Author:  Zviadi Varshanidze <elisagroup@gmail.com>                   */
/* Date:    22.01.2009                                                  */
/* Note:    Adopted from C source code written by an unknown author     */
/************************************************************************/

#ifndef XJPEGLOADER_H
#define XJPEGLOADER_H

class xJpegLoader: public xIImageLoader
{
public:
	xJpegLoader();
	~xJpegLoader();

	virtual bool canLoad(const char* extension, FILE* file) const;
	virtual unsigned char* loadImage(FILE* file, bool keepAlpha);

	virtual int getWidth() const;
	virtual int getHeight() const;
};

#endif	// XJPEGLOADER_H
