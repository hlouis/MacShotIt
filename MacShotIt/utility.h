#pragma once

namespace cloud
{

#include <assert.h>

/**
* Note:		Convert lpstr to lpwstr
* Return:	lpwstr, Remeber to delete after you use it!!!!!
			if return null mean fail.
* Argu:		lpstr text
*/
inline LPWSTR Lpstr2Lpwstr(LPCSTR text)
{
	if (text == NULL)
		return NULL;
	LPWSTR lpwstr	= new WCHAR[strlen(text) + 1];
	int iRet = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, text, (int)(strlen(text) + 1), lpwstr,
		(int)(strlen(text) + 1));

	if (iRet == 0)
	{
		delete lpwstr;
		return NULL;
	}
	else
		return lpwstr;
};

/**
 * Note: 	Convert string to unsigned long
 * Return:  DWORD value
 * Argu:    need to convert
 */
inline DWORD StrToULong(const char* strData)
{
	char* endPtr;
	DWORD ret = strtoul(strData, &endPtr, 10);		// Need stdlib.h
	return ret;
}

inline bool IsExistFile(const char* strPath)
{
	FILE * pFile = _tfopen(strPath, _T("r"));
	if(pFile)
	{
		fclose(pFile);
		return true;
	}
	else
		return false;
}

#ifdef _GDIPLUS_H
using namespace Gdiplus;
inline GraphicsPath* MakeRoundRect(Point topLeft, Point bottomRight, INT percentageRounded)
{
          assert (percentageRounded >= 1 && percentageRounded <= 100);

          INT left  = min(topLeft.X, bottomRight.X);
          INT right = max(topLeft.X, bottomRight.X);

          INT top    = min(topLeft.Y, bottomRight.Y);
          INT bottom = max(topLeft.Y, bottomRight.Y);

          INT offsetX = (right-left)*percentageRounded/100; 
          INT offsetY = (bottom-top)*percentageRounded/100;

          GraphicsPath pt;
          GraphicsPath * path = pt.Clone();

          path->AddArc(right-offsetX, top, offsetX, offsetY, 270, 90);

          path->AddArc(right-offsetX, bottom-offsetY, offsetX, offsetY, 0, 90);

          path->AddArc(left, bottom - offsetY, offsetX, offsetY, 90, 90);

          path->AddArc(left, top, offsetX, offsetY, 180, 90);

          path->AddLine(left + offsetX, top, right - offsetX/2, top);

          return path;
}
inline void RECTToRect( const RECT& gdiRect,	/* [in]		need convert rect */ 
					   Gdiplus::Rect& gdiPlusRect		/* [out]	output result */)
{
	gdiPlusRect.X		= gdiRect.left;
	gdiPlusRect.Y		= gdiRect.top;
	gdiPlusRect.Width	= gdiRect.right - gdiRect.left;
	gdiPlusRect.Height	= gdiRect.bottom - gdiRect.top;
};

/*
*	Calculate the real rect of the text.
*	All text should be in one line now.
*	Width and Height in layout rect before will be overwritten
*	Parameters:
*		[in]text:		need measure string
*		[in]strFromat:	string format
*		[out]rectOut:	real bound of this string
*	Return:
*		false if fail
*/
inline Status GetOneLineTextRealBound(LPCWSTR text, Graphics* graphics,
									  Font* font, StringFormat* strFormat, RectF* rectOut)
{
	CharacterRange charRanges[1] = {
		CharacterRange(0, (int)wcslen(text))
	};
	Region* pCharRegions = new Region[1];
	RectF rect (0, 0, 2000.0, font->GetHeight(graphics));
	strFormat->SetMeasurableCharacterRanges(1, charRanges);

	Status sta = graphics->MeasureCharacterRanges(text, -1, font, rect, strFormat, 1, pCharRegions);
	pCharRegions[0].GetBounds(rectOut, graphics);

	// Need increase the width a little
	rectOut->Width++;

	delete []pCharRegions;

	return sta;
}


#endif

}	// namespace cloud