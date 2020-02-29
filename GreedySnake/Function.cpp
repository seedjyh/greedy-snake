#include <stdio.h>
#include <string.h>
#include <windows.h>

BOOL	Number2String(int iNumber, int iLen, TCHAR *szStr)
{
	if (0 == iNumber)
	{
		*(szStr+iLen)	= '\0';
		while (iLen--)
		{
			*(szStr+iLen)	= '0';
		}
		return TRUE;
	}
	else
	{
		if (0 > iNumber)
		{
			sprintf(szStr, TEXT("-"));
			iNumber	= -iNumber;
			szStr++;
		}
		szStr+=iLen;
		*szStr = '\0';
		for (iLen--,szStr-- ; iLen >=0 ; iLen--, iNumber /= 10, szStr--)
		{
			*szStr = iNumber%10+'0';
		}
		return (-1 == iLen);
	}
}

VOID	ShowHeadCoordinate(HDC hDC, POINT	pLeftTop, POINT	pCoordinate)
{
	TCHAR	szStr[128];
	sprintf(szStr, TEXT("x = "));
	Number2String(pCoordinate.x, 5, szStr + lstrlen((szStr)));
	sprintf(szStr+lstrlen(szStr), TEXT(", y = "));
	Number2String(pCoordinate.y, 5, szStr + lstrlen((szStr)));

	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(255, 200, 255));
	TextOut(hDC, pLeftTop.x, pLeftTop.y, szStr, lstrlen(szStr));
	return;
}

VOID	ShowCurrentMark(HDC	hDC, POINT pLeftTop, int	iCurrentMark)
{
	TCHAR	szStr[128];
	Number2String(iCurrentMark, 5, szStr);
	sprintf(szStr+5, TEXT(" golds."));

	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(200, 200, 255));
	TextOut(hDC, pLeftTop.x, pLeftTop.y, szStr, lstrlen(szStr));
	return;
}
