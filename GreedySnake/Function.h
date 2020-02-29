#define		MAX_SNAKE_LEN	1024
#define		GRID_SIZE		10
#define		SNAKE_INILEN	20
#define		EFFICIENT_EAT	20
#define		FOOD_MARK		500

#define		TIMER_SEC		12113
#define		TIME_REFRESH	10

#define		TIMER_BGM		12114
#define		TIME_BGM_LEN	311000
typedef	POINT	SNAKESEGMENT;

struct	SNAKE {
	SNAKESEGMENT	ssSnakeBody[MAX_SNAKE_LEN];
	SNAKESEGMENT	ssHead;
	int		iStart;
	int		iLen;
	int		iMarks;
	int		iDirection;	//0 : left, 1: top, 2: right, 3: down
	POINT	pMove[4];
	SNAKE	() {}
	SNAKE	(int	len, int direction, POINT	pHead)
	{
		pMove[0].x	=-1;	pMove[0].y= 0;
		pMove[1].x	=0;		pMove[1].y= -1;
		pMove[2].x	=1;		pMove[2].y= 0;
		pMove[3].x	=0;		pMove[3].y= 1;

		iStart		= 0;
		iLen		= len;
		iMarks		= 0;
		iDirection	=direction;
		ssSnakeBody[0]	= pHead;
		for (len=1 ; len<iLen ; len++)
		{
			ssSnakeBody[len]	= ssSnakeBody[len-1];
			ssSnakeBody[len].x	=
				(ssSnakeBody[len].x-pMove[iDirection].x*GRID_SIZE);
			ssSnakeBody[len].y	=
				(ssSnakeBody[len].y-pMove[iDirection].y*GRID_SIZE);
		}
	}
	VOID	DrawSnake(HDC	hdc)
	{
		static	int	i, j;
		for (i=0, j=iStart ; i<iLen ; i++, j++)
		{
			if (MAX_SNAKE_LEN == j)
			{
				j=0;
			}
			Ellipse(hdc, ssSnakeBody[j].x-GRID_SIZE/2,
				ssSnakeBody[j].y-GRID_SIZE/2,
				ssSnakeBody[j].x+(GRID_SIZE-GRID_SIZE/2),
				ssSnakeBody[j].y+(GRID_SIZE-GRID_SIZE/2));
		}
		return;
	}
	VOID	MoveSnake(int cxClient, int cyClient)
	{
		if (0 == cxClient || 0 == cyClient)
		{
			return;
		}
		ssHead.x	= ssSnakeBody[iStart].x + pMove[iDirection].x * GRID_SIZE;
		ssHead.y	= ssSnakeBody[iStart].y + pMove[iDirection].y * GRID_SIZE;
		ssHead.x	= (ssHead.x + cxClient)%cxClient;
		ssHead.y	= (ssHead.y + cyClient)%cyClient;
		iStart	= (iStart-1+MAX_SNAKE_LEN)%MAX_SNAKE_LEN;
		ssSnakeBody[iStart]	= ssHead;
		return;
	}
};

struct FOOD {
	POINT	pPosition;
	int		iMark;
	bool	Eated;
	VOID	DrawFood(HDC	hdc)
	{
		Ellipse(hdc, pPosition.x-EFFICIENT_EAT/2, pPosition.y-EFFICIENT_EAT/2,
			pPosition.x+EFFICIENT_EAT/2, pPosition.y+EFFICIENT_EAT/2);

		return;
	}
};

BOOL   Number2String(int iNumber, int iLen, TCHAR *szStr);

VOID	ShowHeadCoordinate(HDC hDC, POINT	pLeftTop, POINT	pCoordinate);

VOID	ShowCurrentMark(HDC	hDC, POINT pLeftTop, int	iCurrentMark);