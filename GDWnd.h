#pragma once

#include "..\MyLib\BlockList.h"
#include "..\MyLib\RedBlackTree.h"
#include "GraphDrawGP.h"


#define STATE_FUNCTION(name,state,val)	\
	protected: \
	inline void Set##name##State(BOOL b##name) \
	{ if( b##name ) val |= state; else val &= ~state; } \
	inline BOOL Get##name##State() { return (val&state)!=0; } \
	public: \
	virtual BOOL Is##name##() { return Get##name##State(); }

enum eWndMsgCallNext
{
	// 消息处理完成
	E_WMCN_End , 
	// 消息未处理 Not implemented
	E_WMCN_NOTIMPL , 
	// 消息处理完成，父窗口不可进行消息记录
	E_WMCN_FINISHED , 
	//// 异常
	//E_WMCN_Error , 
};
//
//class CGDWnd;
//typedef CGDWnd* (CGDWnd::*fMouseEvent)(UINT nFlags , INT x , INT y);
//typedef CGDWnd* (CGDWnd::*fMouseWheelEvent)(INT KeyState , INT zDelta , INT x , INT y);
//typedef CGDWnd* (CGDWnd::*fMouseContextEvent)(UINT nFlags , INT x , INT y , CGDWnd* pContextWnd);
//typedef CGDWnd* (CGDWnd::*fKeyEvent)(UINT nChar, UINT nRepCnt, UINT nFlags);
//typedef CGDWnd* (CGDWnd::*fCommandEvent)(UINT nID, UINT wmEvent, HWND hCtrlWnd);
//typedef CGDWnd* (CGDWnd::*fMessageEvent)(UINT message, WPARAM wParam, LPARAM lParam);

class CGDWndManager;

// CGraphDrawWnd
class CGDWnd
{
public:
	CGDWnd* m_pParent;
	CGDWnd* m_pChild;
	CGDWnd* m_pLeft;
	CGDWnd* m_pRight;
protected:
	CGDWnd(void);
	~CGDWnd(void);
	// 状态值
	INT m_state;
	// Z向图层序号值
	INT m_zorder;
	// 
	INT m_ChildCount;
	// 
	INT m_CtrlID;
	// 窗口矩形（相对父窗口）
	RECT m_rect;
	enum eWndState
	{
		EWS_DIRTY		= 0x00000001 ,
		EWS_HIDE		= 0x00000002 ,
		EWS_ALPHA		= 0x00000004 ,
	};

	friend class CGDWndManager;
	friend class CGDFrameWnd;

	//fMouseEvent m_fun_OnLButtonDown;
	//fMouseEvent m_fun_OnRButtonDown;
	//fMouseEvent m_fun_OnMButtonDown;
	//fMouseEvent m_fun_OnLButtonUp;
	//fMouseEvent m_fun_OnRButtonUp;
	//fMouseEvent m_fun_OnMButtonUp;
	//fMouseEvent m_fun_OnMouseMove;
	//fMouseEvent m_fun_OnMouseHover;
	//fMouseWheelEvent m_fun_OnMouseWheel;
	//fMouseContextEvent m_fun_OnMouseMoveIn;
	//fMouseContextEvent m_fun_OnMouseMoveOut;
	//fKeyEvent m_fun_OnKeyDown;
	//fKeyEvent m_fun_OnKeyUp;
	//fKeyEvent m_fun_OnChar;
	//fCommandEvent m_fun_OnCommand;

public:
	CGDWndManager* m_pWndManager;
	virtual void OnDraw(HDC hdc , RECT& rc , INT offsetX , INT offsetY) = 0;
	virtual void Release() = 0;

	virtual void ResetPoint(INT& x , INT& y)
	{
		x -= m_rect.left;
		y -= m_rect.top;
	}
	virtual void PointFromMainWnd(INT& x , INT& y)
	{
		if( m_pParent != NULL )
			m_pParent->PointFromMainWnd(x , y);
		ResetPoint(x , y);
	}
	void AddChild(CGDWnd* pWnd , INT zorder = 0);
	virtual void Invalidate();
	virtual void InvalidateRect(RECT& rc);
	BOOL SetId(INT id);
	INT GetId(INT id)
	{
		return m_CtrlID;
	}
	BOOL HideWnd(BOOL bHide)
	{
		BOOL bh = GetHideState();
		SetHideState(bHide);
		if( bHide != bh )
			Invalidate();
		return bh;
	}

	virtual eWndMsgCallNext OnLButtonDown(UINT nFlags , INT x , INT y){return E_WMCN_NOTIMPL;}
	virtual eWndMsgCallNext OnRButtonDown(UINT nFlags , INT x , INT y){return E_WMCN_NOTIMPL;}
	virtual eWndMsgCallNext OnMButtonDown(UINT nFlags , INT x , INT y){return E_WMCN_NOTIMPL;}
	virtual eWndMsgCallNext OnLButtonUp(UINT nFlags , INT x , INT y){return E_WMCN_NOTIMPL;}
	virtual eWndMsgCallNext OnRButtonUp(UINT nFlags , INT x , INT y){return E_WMCN_NOTIMPL;}
	virtual eWndMsgCallNext OnMButtonUp(UINT nFlags , INT x , INT y){return E_WMCN_NOTIMPL;}
	virtual eWndMsgCallNext OnMouseMove(UINT nFlags , INT x , INT y){return E_WMCN_NOTIMPL;}
	virtual eWndMsgCallNext OnMouseHover(UINT nFlags , INT x , INT y){return E_WMCN_NOTIMPL;}

	virtual eWndMsgCallNext OnMouseWheel(INT KeyState , INT zDelta , INT x , INT y){return E_WMCN_NOTIMPL;}

	virtual void OnMouseMoveIn(UINT nFlags , INT x , INT y , CGDWnd* pContextWnd){}
	virtual void OnMouseMoveOut(UINT nFlags , INT x , INT y , CGDWnd* pContextWnd){}

	virtual eWndMsgCallNext OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){return E_WMCN_NOTIMPL;}
	virtual eWndMsgCallNext OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags){return E_WMCN_NOTIMPL;}
	virtual eWndMsgCallNext OnChar(UINT nChar, UINT nRepCnt, UINT nFlags){return E_WMCN_NOTIMPL;}

	virtual eWndMsgCallNext OnSize(UINT nFlags, INT width, INT height){return E_WMCN_NOTIMPL;}
	virtual eWndMsgCallNext OnNCHitTest(INT x , INT y , PRECT pWndRc , LRESULT& reResult){return E_WMCN_NOTIMPL;}

	virtual eWndMsgCallNext OnGetMinMaxInfo(MINMAXINFO* info){return E_WMCN_NOTIMPL;}

	virtual eWndMsgCallNext OnTimer(UINT idEvent){return E_WMCN_NOTIMPL;}
	virtual void OnCommand(UINT nID, UINT wmEvent, HWND hCtrlWnd){}

	static BOOL PostMessage(UINT nID, UINT wmEvent, HWND hCtrlWnd);
	//static CGDWnd* PostMessage(UINT nID, UINT wmEvent, HWND hCtrlWnd);

	BOOL SetTimer(UINT idEvent , UINT uElapse , TIMERPROC lpTimerFunc = NULL);
	BOOL KillTimer(UINT idEvent);

	//CGDWnd* OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//{
	//	if( m_fun_OnMessage == NULL )
	//		return E_WMCN_Next;
	//	return (this->*m_fun_OnMessage)(message, wParam, lParam);
	//}
	virtual BOOL MoveToWnd(INT x , INT y)
	{
		m_rect.right += x-m_rect.left;
		m_rect.bottom += y-m_rect.top;
		m_rect.left = x;
		m_rect.top = y;
		SetDirtyState(TRUE);
		if( m_pParent != NULL )
			m_pParent->Invalidate();
		return TRUE;
	}
	virtual BOOL SizeWnd(INT width , INT height)
	{
		m_rect.right = m_rect.left+width;
		m_rect.bottom = m_rect.top+height;
		return TRUE;
	}
	virtual void GetWndRect(RECT& rc)
	{
		rc = m_rect;
	}

	//// 状态值函数
	// 区域内容是否需要更新
	STATE_FUNCTION(Dirty,EWS_DIRTY,m_state)
	// 是否隐藏
	STATE_FUNCTION(Hide,EWS_HIDE,m_state)
	// 是否透明，即是否需要刷新下方窗口
	STATE_FUNCTION(Alpha,EWS_ALPHA,m_state)
};

inline BOOL PInRect( INT x , INT y , PRECT pr )
{ return( y >= pr->top && y <= pr->bottom && x >= pr->left && x <= pr->right ); }
inline void MergedRect(RECT& r,RECT& r1,RECT& r2)
{
	r.top = r1.top > r2.top ? r2.top : r1.top;
	r.left = r1.left > r2.left ? r2.left : r1.left;
	r.bottom = r1.bottom < r2.bottom ? r2.bottom : r1.bottom;
	r.right = r1.right < r2.right ? r2.right : r1.right;
}
inline RECT& operator +=(RECT& r , const INT i)
{
	r.top -= i;
	r.left -= i;
	r.bottom += i;
	r.right += i;
	return r;
}
inline RECT& operator &=(RECT& r , const RECT& r2)
{
	r.top = r.top < r2.top ? r2.top : r.top;
	r.left = r.left < r2.left ? r2.left : r.left;
	r.bottom = r.bottom > r2.bottom ? r2.bottom : r.bottom;
	r.right = r.right > r2.right ? r2.right : r.right;
	return r;
}
inline RECT& operator |=(RECT& r , const RECT& r2)
{
	r.top = r.top > r2.top ? r2.top : r.top;
	r.left = r.left > r2.left ? r2.left : r.left;
	r.bottom = r.bottom < r2.bottom ? r2.bottom : r.bottom;
	r.right = r.right < r2.right ? r2.right : r.right;
	return r;
}


#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
#endif
class CGDWndManager
{
protected:
	struct sGDWndIdIndexItem
	{
		sGDWndIdIndexItem(){}
		~sGDWndIdIndexItem(){}
		CGDWnd* pWnd;
		INT color;
		void SetColor(INT c)
		{
			color = c;
		}
		INT GetColor()
		{
			return color;
		}
		BOOL operator==( sGDWndIdIndexItem& d )
		{
			return pWnd->m_CtrlID == d.pWnd->m_CtrlID;
		}
		BOOL operator>( sGDWndIdIndexItem& d )
		{
			return pWnd->m_CtrlID > d.pWnd->m_CtrlID;
		}
		BOOL operator<( sGDWndIdIndexItem& d )
		{
			return pWnd->m_CtrlID < d.pWnd->m_CtrlID;
		}

		BOOL operator>( CGDWnd* pwnd )
		{
			return pWnd->m_CtrlID > pwnd->m_CtrlID;
		}
		BOOL operator<( CGDWnd* pwnd )
		{
			return pWnd->m_CtrlID < pwnd->m_CtrlID;
		}


		BOOL operator>( INT id )
		{
			return pWnd->m_CtrlID > id;
		}
		BOOL operator<( INT id )
		{
			return pWnd->m_CtrlID < id;
		}

		sGDWndIdIndexItem& operator = ( const sGDWndIdIndexItem& d )
		{
			pWnd = d.pWnd;
			return *this;
		}
		sGDWndIdIndexItem& operator = (CGDWnd* pwnd)
		{
			pWnd = pwnd;
			return *this;
		}
	};
	CMyBaseRedBlackTree<sGDWndIdIndexItem> m_CtrlIdIndex;
	CGDWnd* m_pMouseHookWnd;
public:
	INT m_LastDefaultId;
	CGDWnd* m_pLastMouseInWnd;
	CGDWnd* m_pFocusWnd;
	//CBlockList<CGDWnd*,CGDWnd*,false> m_CtrlList;
	CGDWnd* m_rootWnd;
	HWND hWnd;
	CGDWndManager()
	{
		m_pLastMouseInWnd = NULL;
		m_pMouseHookWnd = NULL;
		m_pFocusWnd = NULL;
		m_rootWnd = NULL;
		hWnd = NULL;
		m_LastDefaultId = 0;
	}
	CGDWnd* SetCapture(CGDWnd* pWnd)
	{
		CGDWnd* pTmpWnd = m_pMouseHookWnd;
		m_pMouseHookWnd = pWnd;
		if( hWnd != NULL )
			::SetCapture(hWnd);
		return pTmpWnd;
	}
	CGDWnd* ReleaseCapture()
	{
		CGDWnd* pTmpWnd = m_pMouseHookWnd;
		m_pMouseHookWnd = NULL;
		::ReleaseCapture();
		return pTmpWnd;
	}
	~CGDWndManager()
	{
		RemoveAllCtrl();
	}
	void RemoveAllCtrl()
	{
		if( m_rootWnd != NULL )
			m_rootWnd->Release();
		//POSITION pos;
		//CGDWnd** ppWnd;
		//pos = m_CtrlList.GetHeadPosition();
		//while(pos)
		//{
		//	ppWnd = m_CtrlList.GetNext(pos);
		//	(*ppWnd)->Release();
		//}
		//m_CtrlList.RemoveAll();
	}
	CGDWnd* GetItemFromId(INT id)
	{
		sGDWndIdIndexItem* pgdwiii = m_CtrlIdIndex.Find(id);
		if( pgdwiii != NULL && pgdwiii->pWnd != NULL )
			return pgdwiii->pWnd;
		return NULL;
	}
	BOOL RemoveCtrl(CGDWnd* pWnd)
	{
		sGDWndIdIndexItem gdwiii;
		gdwiii.pWnd = pWnd;
		//m_CtrlIdIndex.find(pWnd);
		return m_CtrlIdIndex.Delete2(gdwiii);
	}
	BOOL AddCtrl(CGDWnd* pWnd)
	{
		sGDWndIdIndexItem gdwiii;
		gdwiii.pWnd = pWnd;
		//m_CtrlIdIndex.find(pWnd);
		return m_CtrlIdIndex.Insert(gdwiii);
	}
	void OnLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		if( m_pMouseHookWnd == NULL )
			m_rootWnd->OnLButtonDown(wParam ,(INT)GET_X_LPARAM(lParam) , (INT)GET_Y_LPARAM(lParam) );
		else
		{
			INT x = (INT)GET_X_LPARAM(lParam);
			INT y = (INT)GET_Y_LPARAM(lParam);
			if( m_pMouseHookWnd->m_pParent != NULL )
				m_pMouseHookWnd->m_pParent->PointFromMainWnd(x,y);
			m_pMouseHookWnd->OnLButtonDown(wParam , x , y);
		}

	}
	void OnRButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		if( m_pMouseHookWnd == NULL )
			m_rootWnd->OnRButtonDown(wParam ,(INT)GET_X_LPARAM(lParam) , (INT)GET_Y_LPARAM(lParam) );
		else
		{
			INT x = (INT)GET_X_LPARAM(lParam);
			INT y = (INT)GET_Y_LPARAM(lParam);
			if( m_pMouseHookWnd->m_pParent != NULL )
				m_pMouseHookWnd->m_pParent->PointFromMainWnd(x,y);
			m_pMouseHookWnd->OnRButtonDown(wParam , x , y);
		}
	}
	void OnMButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		if( m_pMouseHookWnd == NULL )
			m_rootWnd->OnMButtonDown(wParam ,(INT)GET_X_LPARAM(lParam) , (INT)GET_Y_LPARAM(lParam) );
		else
		{
			INT x = (INT)GET_X_LPARAM(lParam);
			INT y = (INT)GET_Y_LPARAM(lParam);
			if( m_pMouseHookWnd->m_pParent != NULL )
				m_pMouseHookWnd->m_pParent->PointFromMainWnd(x,y);
			m_pMouseHookWnd->OnMButtonDown(wParam , x , y);
		}
	}
	void OnLButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		if( m_pMouseHookWnd == NULL )
			m_rootWnd->OnLButtonUp(wParam ,(INT)GET_X_LPARAM(lParam) , (INT)GET_Y_LPARAM(lParam) );
		else
		{
			INT x = (INT)GET_X_LPARAM(lParam);
			INT y = (INT)GET_Y_LPARAM(lParam);
			if( m_pMouseHookWnd->m_pParent != NULL )
				m_pMouseHookWnd->m_pParent->PointFromMainWnd(x,y);
			m_pMouseHookWnd->OnLButtonUp(wParam , x , y);
		}
	}
	void OnRButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		if( m_pMouseHookWnd == NULL )
			m_rootWnd->OnRButtonUp(wParam ,(INT)GET_X_LPARAM(lParam) , (INT)GET_Y_LPARAM(lParam) );
		else
		{
			INT x = (INT)GET_X_LPARAM(lParam);
			INT y = (INT)GET_Y_LPARAM(lParam);
			if( m_pMouseHookWnd->m_pParent != NULL )
				m_pMouseHookWnd->m_pParent->PointFromMainWnd(x,y);
			m_pMouseHookWnd->OnRButtonUp(wParam , x , y);
		}
	}
	void OnMButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		if( m_pMouseHookWnd == NULL )
			m_rootWnd->OnMButtonUp(wParam ,(INT)GET_X_LPARAM(lParam) , (INT)GET_Y_LPARAM(lParam) );
		else
		{
			INT x = (INT)GET_X_LPARAM(lParam);
			INT y = (INT)GET_Y_LPARAM(lParam);
			if( m_pMouseHookWnd->m_pParent != NULL )
				m_pMouseHookWnd->m_pParent->PointFromMainWnd(x,y);
			m_pMouseHookWnd->OnMButtonUp(wParam , x , y);
		}
	}
	void OnMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		if( m_pMouseHookWnd == NULL )
			m_rootWnd->OnMouseMove(wParam ,(INT)GET_X_LPARAM(lParam) , (INT)GET_Y_LPARAM(lParam) );
		else
		{
			INT x = (INT)GET_X_LPARAM(lParam);
			INT y = (INT)GET_Y_LPARAM(lParam);
			if( m_pMouseHookWnd->m_pParent != NULL )
				m_pMouseHookWnd->m_pParent->PointFromMainWnd(x,y);
			m_pMouseHookWnd->OnMouseMove(wParam , x , y);
		}
	}
	void OnMouseWheel(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		if( m_pMouseHookWnd == NULL )
			m_rootWnd->OnMouseWheel((INT)GET_KEYSTATE_WPARAM(wParam) , (INT)GET_WHEEL_DELTA_WPARAM(wParam) ,
			(INT)GET_X_LPARAM(lParam) , (INT)GET_Y_LPARAM(lParam) );
		else
		{
			INT x = (INT)GET_X_LPARAM(lParam);
			INT y = (INT)GET_Y_LPARAM(lParam);
			if( m_pMouseHookWnd->m_pParent != NULL )
				m_pMouseHookWnd->m_pParent->PointFromMainWnd(x,y);
			m_pMouseHookWnd->OnMouseWheel((INT)GET_KEYSTATE_WPARAM(wParam) , 
				(INT)GET_WHEEL_DELTA_WPARAM(wParam) , x , y );
		}
	}
	//void OnMouseMoveIn(HWND hWnd, WPARAM wParam, LPARAM lParam)
	//{
	//}
	//void OnMouseMoveOut(HWND hWnd, WPARAM wParam, LPARAM lParam)
	//{
	//}
	void OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		m_rootWnd->OnKeyDown(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam));
	}
	void OnKeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		m_rootWnd->OnKeyUp(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam));
	}
	void OnChar(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		m_rootWnd->OnChar(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam));
	}
	void OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		m_rootWnd->OnSize(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam));
	}
	void OnGetMinMaxInfo(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		m_rootWnd->OnGetMinMaxInfo((MINMAXINFO*)lParam);
	}
	LRESULT OnNCHitTest(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		LRESULT re = HTCLIENT;
		INT x = (INT)GET_X_LPARAM(lParam);
		INT y = (INT)GET_Y_LPARAM(lParam);
		RECT rc;
		::GetWindowRect(hWnd , &rc);
		x -= rc.left;
		y -= rc.top;
		m_rootWnd->OnNCHitTest( x , y , &rc , re);
		return re;
	}
	void OnNCMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		INT x = (INT)GET_X_LPARAM(lParam);
		INT y = (INT)GET_Y_LPARAM(lParam);
		RECT rc;
		::GetWindowRect(hWnd , &rc);
		x -= rc.left;
		y -= rc.top;
		if( m_pMouseHookWnd == NULL )
			m_rootWnd->OnMouseMove( wParam , x , y );
		else
		{
			if( m_pMouseHookWnd->m_pParent != NULL )
				m_pMouseHookWnd->m_pParent->PointFromMainWnd(x,y);
			m_pMouseHookWnd->OnMouseMove( wParam , x , y );
		}
	}
	void OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		INT wmId    = LOWORD(wParam);
		INT wmEvent = HIWORD(wParam);
		sGDWndIdIndexItem* pgdwiii = m_CtrlIdIndex.Find(wmId);
		if( pgdwiii != NULL && pgdwiii->pWnd != NULL )
			pgdwiii->pWnd->OnCommand(wmId , wmEvent , (HWND)lParam);
	}
	void OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		INT wmId    = LOWORD(wParam);
		UINT wmEvent = HIWORD(wParam);
		sGDWndIdIndexItem* pgdwiii = m_CtrlIdIndex.Find(wmId);
		if( pgdwiii != NULL && pgdwiii->pWnd != NULL )
			pgdwiii->pWnd->OnTimer(wmEvent);
	}
	void OnPaint(HDC hdc , PAINTSTRUCT& ps)
	{
		//m_rootWnd->OnDraw(hdc , ps.rcPaint , 0 , 0);
		HDC tdc;
		HBITMAP tbm;
		//RECT rc;
		//rc.bottom = ps.rcPaint.bottom-ps.rcPaint.top;
		//rc.right = ps.rcPaint.right-ps.rcPaint.left;
		//rc.top = 0;;
		//rc.left = 0;

		//HDC tdc2 = GetWindowDC(GetDesktopWindow());
		//tdc = CreateCompatibleDC(tdc2);
		//ReleaseDC(GetDesktopWindow(),tdc2);
		tdc = CreateCompatibleDC(hdc);
		//tdc = CreateCompatibleDC(NULL);
		tbm = CreateCompatibleBitmap(hdc , ps.rcPaint.right , ps.rcPaint.bottom);
		SelectObject(tdc , tbm);
		m_rootWnd->OnDraw(tdc , ps.rcPaint , 0 , 0);

		//HBRUSH tbr = CreateSolidBrush(RGB(255,0,0));
		//FillRect(tdc , &rc , tbr);
		//DeleteObject(tbr);

		BitBlt(hdc , ps.rcPaint.left , ps.rcPaint.top , 
			ps.rcPaint.right-ps.rcPaint.left , ps.rcPaint.bottom-ps.rcPaint.top , tdc , 
			ps.rcPaint.left , ps.rcPaint.top , SRCCOPY);
		DeleteObject(tbm);
		DeleteDC(tdc);
	}
};

class CGDBackGround
{
public:
	virtual void OnDraw(HDC hdc , RECT& rc , RECT& drawRc , INT offsetX , INT offsetY) = 0;
	virtual void Release() = 0;
};

class CGDFrameWnd : public CGDWnd
{
protected:
	CGDBackGround* m_pBackGround;
	CBaseBlockStack<CGDWnd*> m_TmpBufList;
	void Init(RECT& rc , CGDBackGround* pBG)
	{
		m_rect = rc;
		m_pBackGround = pBG;
	}
public:
	// 背景颜色
	//Gdiplus::Color m_bkColor;
	CGDFrameWnd()
	{
		m_pBackGround = NULL;
		SetAlphaState(TRUE);
		SetDirtyState(TRUE);
	}
	~CGDFrameWnd()
	{
	}

	BOOL GetDirtyRect(RECT& rc)
	{
		if( CGDWnd::IsDirty() )
		{
			rc = m_rect;
			return TRUE;
		}
		rc.top = m_rect.bottom;
		rc.bottom = m_rect.top;
		rc.left = m_rect.right;
		rc.right = m_rect.left;
		CGDWnd* pWnd;
		pWnd = m_pChild;
		while( pWnd != NULL )
		{
			if( pWnd->IsDirty() )
			{
				if( rc.top > pWnd->m_rect.top )
					rc.top = pWnd->m_rect.top;
				if( rc.left > pWnd->m_rect.left )
					rc.left = pWnd->m_rect.left;
				if( rc.right < pWnd->m_rect.right )
					rc.right = pWnd->m_rect.right;
				if( rc.bottom < pWnd->m_rect.bottom )
					rc.bottom = pWnd->m_rect.bottom;
			}
			pWnd = pWnd->m_pRight;
		}
		return FALSE;
	}
	virtual BOOL IsDirty()
	{
		if( CGDWnd::IsDirty() )
			return TRUE;
		CGDWnd* pWnd;
		pWnd = m_pChild;
		while( pWnd != NULL )
		{
			if( pWnd->IsDirty() )
				return TRUE;
			pWnd = pWnd->m_pRight;
		}
		return FALSE;
	}
	virtual void OnDraw(HDC hdc , RECT& rc , INT offsetX , INT offsetY)
	{
		// 不显示
		if( IsHide() )
			return;
		CGDWnd* pWnd;
		ASSERT(m_pChild == NULL || m_pChild->m_pLeft == NULL );
		RECT drc;
		drc.bottom = m_rect.bottom + offsetY;
		drc.top = m_rect.top + offsetX;
		drc.right = m_rect.right + offsetX;
		drc.left = m_rect.left + offsetX;
		drc &= rc;
		m_TmpBufList.PopToBottom();
		if( m_pBackGround != NULL )
			m_pBackGround->OnDraw(hdc , m_rect , drc , offsetX , offsetY);
		offsetX += m_rect.left;
		offsetY += m_rect.top;
		pWnd = m_pChild;
		while( pWnd != NULL )
		{
			if( !pWnd->IsHide() )
			{
				pWnd->OnDraw(hdc , drc  , offsetX , offsetY);
			}
			pWnd = pWnd->m_pRight;
		}
		return;
	}
	virtual void Release()
	{
		delete this;
	}
	static CGDFrameWnd* CreateWnd(RECT& rc , CGDBackGround* pBG)
	{
		CGDFrameWnd* pgdfw = new CGDFrameWnd;
		pgdfw->Init(rc , pBG);
		return pgdfw;
	}
	virtual BOOL SizeWnd(INT width , INT height)
	{
		CGDWnd::SizeWnd(width , height);
		return TRUE;
	}
	virtual eWndMsgCallNext OnSize(UINT nFlags, INT width, INT height)
	{
		if( nFlags == SIZE_MINIMIZED ) // || (width == m_rect.right && height == m_rect.bottom) 
		{
			return E_WMCN_NOTIMPL;
		}
		SizeWnd(width ,  height);
		return E_WMCN_NOTIMPL;
	}
	CGDWnd* GetWndFromPoint(INT x , INT y)
	{
		CGDWnd* pWnd;
		CGDWnd* pGetWnd = NULL;
		ASSERT(m_pChild == NULL || m_pChild->m_pLeft == NULL );
		if( !IsHide() )
		{
			pWnd = m_pChild;
			while( pWnd != NULL )
			{
				if( !pWnd->IsHide() && PInRect(x,y,&pWnd->m_rect) && 
					(pGetWnd == NULL || pGetWnd->m_zorder < pWnd->m_zorder) )
				{
					pGetWnd = pWnd;
				}
				pWnd = pWnd->m_pRight;
			}
		}
		return pGetWnd;
	}
	virtual eWndMsgCallNext OnNCHitTest(INT x , INT y , PRECT pWndRc , LRESULT& reResult)
	{
		CGDWnd* pWnd;
		ResetPoint(x,y);
		pWnd = GetWndFromPoint(x,y);
		if( pWnd != NULL )
		{
			return pWnd->OnNCHitTest(x,y,pWndRc,reResult);
			//reResult = HTCLIENT;
		}
		else
		{
			reResult = HTCAPTION;
		}
		return E_WMCN_FINISHED;
	}
	virtual eWndMsgCallNext OnLButtonDown(UINT nFlags , INT x , INT y)
	{
		CGDWnd* pWnd;
		eWndMsgCallNext wmcn;
		ResetPoint(x,y);
		pWnd = GetWndFromPoint(x,y);
		if( pWnd != NULL )
		{
			wmcn = pWnd->OnLButtonDown(nFlags , x , y);
			if( wmcn != E_WMCN_FINISHED )
			{
				if( m_pWndManager->m_pFocusWnd != pWnd )
				{
					// 更改焦点
					m_pWndManager->m_pFocusWnd = pWnd;
				}
			}
		}
		return E_WMCN_FINISHED;
	}
	virtual eWndMsgCallNext OnRButtonDown(UINT nFlags , INT x , INT y)
	{
		CGDWnd* pWnd;
		eWndMsgCallNext wmcn;
		ResetPoint(x,y);
		pWnd = GetWndFromPoint(x,y);
		if( pWnd != NULL )
		{
			wmcn = pWnd->OnRButtonDown(nFlags , x , y);
			if( wmcn != E_WMCN_FINISHED )
			{
				if( m_pWndManager->m_pFocusWnd != pWnd )
				{
					// 更改焦点
					m_pWndManager->m_pFocusWnd = pWnd;
				}
			}
		}
		return E_WMCN_FINISHED;
	}
	virtual eWndMsgCallNext OnMButtonDown(UINT nFlags , INT x , INT y)
	{
		CGDWnd* pWnd;
		eWndMsgCallNext wmcn;
		ResetPoint(x,y);
		pWnd = GetWndFromPoint(x,y);
		if( pWnd != NULL )
		{
			wmcn = pWnd->OnMButtonDown(nFlags , x , y);
			if( wmcn != E_WMCN_FINISHED )
			{
				if( m_pWndManager->m_pFocusWnd != pWnd )
				{
					// 更改焦点
					m_pWndManager->m_pFocusWnd = pWnd;
				}
			}
		}
		return E_WMCN_FINISHED;
	}
	virtual eWndMsgCallNext OnLButtonUp(UINT nFlags , INT x , INT y)
	{
		CGDWnd* pWnd;
		eWndMsgCallNext wmcn;
		ResetPoint(x,y);
		pWnd = GetWndFromPoint(x,y);
		if( pWnd != NULL )
		{
			wmcn = pWnd->OnLButtonUp(nFlags , x , y);
			if( wmcn != E_WMCN_FINISHED )
			{
			}
		}
		return E_WMCN_FINISHED;
	}
	virtual eWndMsgCallNext OnRButtonUp(UINT nFlags , INT x , INT y)
	{
		CGDWnd* pWnd;
		eWndMsgCallNext wmcn;
		ResetPoint(x,y);
		pWnd = GetWndFromPoint(x,y);
		if( pWnd != NULL )
		{
			wmcn = pWnd->OnRButtonUp(nFlags , x , y);
			if( wmcn != E_WMCN_FINISHED )
			{
			}
		}
		return E_WMCN_FINISHED;
	}
	virtual eWndMsgCallNext OnMButtonUp(UINT nFlags , INT x , INT y)
	{
		CGDWnd* pWnd;
		eWndMsgCallNext wmcn;
		ResetPoint(x,y);
		pWnd = GetWndFromPoint(x,y);
		if( pWnd != NULL )
		{
			wmcn = pWnd->OnMButtonUp(nFlags , x , y);
			if( wmcn != E_WMCN_FINISHED )
			{
			}
		}
		return E_WMCN_FINISHED;
	}
	virtual eWndMsgCallNext OnMouseMove(UINT nFlags , INT x , INT y)
	{
		CGDWnd* pWnd;
		eWndMsgCallNext wmcn = E_WMCN_NOTIMPL;
		ResetPoint(x,y);
		pWnd = GetWndFromPoint(x,y);
		//TRACE(L"OnMouseMove:this:%08X X:%i Y%i" , this , x , y);
		if( pWnd != NULL )
		{
			wmcn = pWnd->OnMouseMove(nFlags , x , y);
		}
		if( wmcn != E_WMCN_FINISHED )
		{
			if( m_pWndManager->m_pLastMouseInWnd != pWnd )
			{
				if( m_pWndManager->m_pLastMouseInWnd != NULL )
				{
					m_pWndManager->m_pLastMouseInWnd->OnMouseMoveOut(nFlags , x , y , pWnd);
				}
				if( pWnd != NULL )
				{
					pWnd->OnMouseMoveIn(nFlags , x , y , m_pWndManager->m_pLastMouseInWnd);
				}
				// 更改焦点
				m_pWndManager->m_pLastMouseInWnd = pWnd;
			}
		}
		return E_WMCN_FINISHED;
	}
	virtual eWndMsgCallNext OnMouseHover(UINT nFlags , INT x , INT y)
	{
		CGDWnd* pWnd;
		eWndMsgCallNext wmcn;
		ResetPoint(x,y);
		pWnd = GetWndFromPoint(x,y);
		if( pWnd != NULL )
		{
			wmcn = pWnd->OnMouseHover(nFlags , x , y);
			if( wmcn != E_WMCN_FINISHED )
			{
			}
		}
		return E_WMCN_FINISHED;
	}

	virtual eWndMsgCallNext OnMouseWheel(INT KeyState , INT zDelta , INT x , INT y)
	{
		CGDWnd* pWnd;
		eWndMsgCallNext wmcn;
		ResetPoint(x,y);
		pWnd = GetWndFromPoint(x,y);
		if( pWnd != NULL )
		{
			wmcn = pWnd->OnMouseWheel(KeyState , zDelta , x , y);
			if( wmcn != E_WMCN_FINISHED )
			{
			}
		}
		return E_WMCN_FINISHED;
	}

	//virtual void OnMouseMoveIn(UINT nFlags , INT x , INT y , CGDWnd* pContextWnd){}
	//virtual void OnMouseMoveOut(UINT nFlags , INT x , INT y , CGDWnd* pContextWnd){}

	//virtual eWndMsgCallNext OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){return E_WMCN_NOTIMPL;}
	//virtual eWndMsgCallNext OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags){return E_WMCN_NOTIMPL;}
	//virtual eWndMsgCallNext OnChar(UINT nChar, UINT nRepCnt, UINT nFlags){return E_WMCN_NOTIMPL;}
};

class CMsgCallBackClass{};
typedef void (CMsgCallBackClass::*fOnBtnClicked)();
typedef void (CMsgCallBackClass::*fOnSelectItem)(INT id);
class CGDButton : public CGDWnd
{
public:
	CMsgCallBackClass* m_msgClass;
	fOnBtnClicked fun_OnBtnClicked;
	BOOL bHover;
	BOOL bDown;
	BOOL bDownClick;
	// 标准按钮图片
	CImgDraw m_btnImg;
	// 鼠标悬停图
	CImgDraw m_hImg;
	// 鼠标按下图
	CImgDraw m_dImg;
	virtual void Release()
	{
		delete this;
	}
	virtual void OnDraw(HDC hdc , RECT& rc , INT offsetX , INT offsetY)
	{
		Gdiplus::Graphics graphics(hdc);
		graphics.TranslateTransform( (Gdiplus::REAL)offsetX , (Gdiplus::REAL)offsetY );
		if( bDown )
			m_dImg.OnDraw(graphics);
		else if( bHover )
			m_hImg.OnDraw(graphics);
		else
			m_btnImg.OnDraw(graphics);
	}
	CGDButton(POINT& pt ,IStream* ist,IStream* istd,IStream* isth , 
		CMsgCallBackClass* msgClass , fOnBtnClicked fnOBC ) : m_btnImg(ist),m_hImg(isth),m_dImg(istd)
	{
		m_btnImg.SetPoint(pt);
		m_hImg.SetPoint(pt);
		m_dImg.SetPoint(pt);
		m_btnImg.GetRect(m_rect);
		m_msgClass = msgClass;
		fun_OnBtnClicked = fnOBC;
		bHover = FALSE;
		bDown = FALSE;
		bDownClick = (istd==NULL) ? TRUE : FALSE;
	}
	~CGDButton()
	{
	}
	virtual eWndMsgCallNext OnLButtonDown(UINT nFlags , INT x , INT y)
	{
		if( bDownClick )
		{
			if( m_msgClass != NULL && fun_OnBtnClicked != NULL )
				(m_msgClass->*(fun_OnBtnClicked))();
		}
		else
		{
			bDown = TRUE;
			m_pWndManager->SetCapture(this);
		}
		Invalidate();
		return E_WMCN_End;
	}
	virtual eWndMsgCallNext OnLButtonUp(UINT nFlags , INT x , INT y)
	{
		bDown = FALSE;
		if( m_pWndManager->ReleaseCapture() == this && PInRect(x,y,&m_rect) && 
			m_msgClass != NULL && fun_OnBtnClicked != NULL )
			(m_msgClass->*(fun_OnBtnClicked))();
		Invalidate();
		return E_WMCN_End;
	}
	virtual void OnMouseMoveIn(UINT nFlags , INT x , INT y , CGDWnd* pContextWnd)
	{
		bHover = TRUE;
		Invalidate();
	}
	virtual void OnMouseMoveOut(UINT nFlags , INT x , INT y , CGDWnd* pContextWnd)
	{
		bHover = FALSE;
		Invalidate();
	}
	static CGDButton* Add(POINT& pt ,IStream* ist,IStream* istd,IStream* isth , 
		CMsgCallBackClass* msgClass , fOnBtnClicked fnOBC )
	{
		CGDButton* pBtn = new CGDButton(pt , ist, istd, isth, msgClass, fnOBC);
		return pBtn;
	}
};
class CGDBackGround_Solid : public CGDBackGround
{
public:
	Gdiplus::SolidBrush m_brs;
	CGDBackGround_Solid(Gdiplus::Color col) : m_brs(col)
	{
	}
	~CGDBackGround_Solid()
	{
	}
	virtual void Release()
	{
		delete this;
	}
	virtual void OnDraw(HDC hdc , RECT& rc , RECT& drawRc , INT offsetX , INT offsetY)
	{
		Gdiplus::Graphics graphics(hdc);
		graphics.TranslateTransform( (Gdiplus::REAL)offsetX , (Gdiplus::REAL)offsetY );
		//Gdiplus::Pen pen(Gdiplus::Color(255,0,0,0),3.0f);
		//graphics.DrawRectangle(&pen,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top);
		graphics.FillRectangle(&m_brs , Gdiplus::Rect(drawRc.left,drawRc.top,drawRc.right-drawRc.left,drawRc.bottom-drawRc.top) );
	}
	static CGDBackGround_Solid* Add(Gdiplus::Color col)
	{
		CGDBackGround_Solid* pbglg = new CGDBackGround_Solid(col);
		return pbglg;
	}
};
class CGDBackGround_LG : public CGDBackGround
{
public:
	Gdiplus::Color m_col1;
	Gdiplus::Color m_col2;
	Gdiplus::REAL m_angle;
	CGDBackGround_LG()
	{
	}
	~CGDBackGround_LG()
	{
	}
	void Init(Gdiplus::Color col1 , Gdiplus::Color col2 , Gdiplus::REAL angle)
	{
		m_col1 = col1;
		m_col2 = col2;
		m_angle = angle;
	}
	virtual void Release()
	{
		delete this;
	}
	virtual void OnDraw(HDC hdc , RECT& rc , RECT& drawRc , INT offsetX , INT offsetY)
	{
		Gdiplus::Graphics graphics(hdc);
		graphics.TranslateTransform( (Gdiplus::REAL)offsetX , (Gdiplus::REAL)offsetY );
		Gdiplus::LinearGradientBrush brs( Gdiplus::Rect(rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top) , 
			m_col1 , m_col2 , m_angle );
		graphics.FillRectangle(&brs , Gdiplus::Rect(drawRc.left,drawRc.top,drawRc.right-drawRc.left,drawRc.bottom-drawRc.top) );
	}
	static CGDBackGround_LG* Add(Gdiplus::Color col1 , Gdiplus::Color col2 , Gdiplus::REAL angle)
	{
		CGDBackGround_LG* pbglg = new CGDBackGround_LG;
		pbglg->Init(col1,col2,angle);
		return pbglg;
	}
};
class CGDSelHeadlBar : public CGDWnd
{
public:
	Gdiplus::Font m_textFont;
	BOOL m_bOnTimer;
	CMsgCallBackClass* m_msgClass;
	fOnSelectItem fun_OnSelectItem;
	INT m_selId;
	INT m_hoverId;
	struct sItem
	{
		std::wstring text;
	};
	CBaseBlockStack<sItem> m_ItemList;
	void Init()
	{
		if( m_bOnTimer )
			KillTimer(1451);
		m_rect.top = 0;
		m_rect.left = 0;
		m_rect.right = 0;
		m_rect.bottom = 0;
		m_selId = -1;
		m_hoverId = -1;
		m_ItemList.PopToBottomExit();
	}
	CGDSelHeadlBar() : m_textFont(L"宋体" , 20.0f , Gdiplus::FontStyleBold , Gdiplus::UnitPixel)
	{
		m_msgClass = NULL;
		fun_OnSelectItem = NULL;
		m_bOnTimer = FALSE;
	}
	~CGDSelHeadlBar()
	{
		m_ItemList.PopToBottomExit();
	}
	virtual void Release()
	{
		delete this;
	}
	BOOL AddItem(LPWSTR text)
	{
		sItem* pi;
		pi = m_ItemList.PushInit();
		pi->text = text;
		SizeWnd(m_ItemList.GetCount()*110 , 40);
		return TRUE;
	}
	INT SetSel(INT sel)
	{
		INT ps = m_selId;
		if( sel != m_selId )
			Invalidate();
		m_selId = sel;
		return ps;
	}
	static CGDSelHeadlBar* Add()
	{
		CGDSelHeadlBar* pshb = new CGDSelHeadlBar;
		pshb->Init();
		return pshb;
	}
	virtual eWndMsgCallNext OnTimer(UINT idEvent)
	{
		return E_WMCN_NOTIMPL;
	}
	virtual void OnDraw(HDC hdc , RECT& rc , INT offsetX , INT offsetY)
	{
		Gdiplus::Graphics graphics(hdc);
		graphics.TranslateTransform( (Gdiplus::REAL)offsetX , (Gdiplus::REAL)offsetY );
		Gdiplus::GraphicsPath roundedRect;
		roundedRect.StartFigure();
		roundedRect.AddArc(0, 0, 10, 10, 180, 90);
		roundedRect.AddLine(5, 0, 100, 0);
		roundedRect.AddArc(98, 0, 10, 10, 270, 90);
		roundedRect.AddLine(108, 5, 108, 40);
		roundedRect.AddLine(108, 40, 0, 40);
		roundedRect.AddLine(0,40, 0, 5);
		roundedRect.CloseFigure();
		Gdiplus::Region rn1(&roundedRect);
		Gdiplus::SolidBrush br(Gdiplus::Color(255,40,160,255));
		Gdiplus::SolidBrush brs(Gdiplus::Color(255,25,100,255));
		Gdiplus::SolidBrush brh(Gdiplus::Color(255,80,160,255));
		Gdiplus::SolidBrush tbr(Gdiplus::Color(255,255,255,255));
		INT i; // , k , j;
		sItem* pi;
		sBaseBlockStackPosition pos;
		m_ItemList.GetHeadPosition(pos);
		i = 0;
		Gdiplus::RectF grc;
		Gdiplus::StringFormat stringFormat;
		stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
		stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
		grc.X = 5;
		grc.Y = 0;
		grc.Width = 100;
		grc.Height = 40;
		graphics.TranslateTransform( (Gdiplus::REAL)m_rect.left , (Gdiplus::REAL)m_rect.top );
		while( (pi=m_ItemList.GetNext(pos)) != NULL )
		{
			if( i == m_selId )
				graphics.FillRegion(&brs , &rn1);
			else if( i == m_hoverId )
				graphics.FillRegion(&brh , &rn1);
			else
				graphics.FillRegion(&br , &rn1);
			graphics.DrawString(pi->text.c_str(),-1,&m_textFont,grc,&stringFormat,&tbr);
			graphics.TranslateTransform( 110 , 0 );
			i++;
		}
		graphics.TranslateTransform( (Gdiplus::REAL)(-110*i) , 0 );
		graphics.TranslateTransform( (Gdiplus::REAL)(-m_rect.left) , (Gdiplus::REAL)(-m_rect.top) );
	}
	virtual eWndMsgCallNext OnLButtonDown(UINT nFlags , INT x , INT y)
	{
		INT tid =(x-m_rect.left) / 110;
		if( tid != m_selId )
			Invalidate();
		m_selId = tid;
		if( m_msgClass != NULL && fun_OnSelectItem != NULL )
			(m_msgClass->*(fun_OnSelectItem))(m_selId);
		return E_WMCN_End;
	}
	//virtual void OnMouseMoveIn(UINT nFlags , INT x , INT y , CGDWnd* pContextWnd)
	//{
	//	//Invalidate();
	//}
	virtual void OnMouseMoveOut(UINT nFlags , INT x , INT y , CGDWnd* pContextWnd)
	{
		if( m_selId != m_hoverId )
			Invalidate();
		m_hoverId = -1;
		//Invalidate();
	}
	virtual eWndMsgCallNext OnMouseMove(UINT nFlags , INT x , INT y)
	{
		INT tid =(x-m_rect.left) / 110;
		if( tid != m_hoverId )
			Invalidate();
		m_hoverId = tid;
		return E_WMCN_End;
	}
};
class CTitleIconWnd : public CGDWnd
{
public:
	CImgDraw m_Img;
	CTitleIconWnd(POINT& pt , IStream* ist) : m_Img(ist)
	{
		m_Img.SetPoint(pt);
		m_Img.GetRect(m_rect);
	}
	~CTitleIconWnd()
	{

	}
	virtual void Release()
	{
		delete this;
	}
	static CTitleIconWnd* Add(POINT& pt , IStream* ist)
	{
		CTitleIconWnd* ptiw = new CTitleIconWnd(pt,ist);
		return ptiw;
	}
	virtual void OnDraw(HDC hdc , RECT& rc , INT offsetX , INT offsetY)
	{
		Gdiplus::Graphics graphics(hdc);
		graphics.TranslateTransform( (Gdiplus::REAL)offsetX , (Gdiplus::REAL)offsetY );
		m_Img.OnDraw(graphics);
	}
	virtual eWndMsgCallNext OnNCHitTest(INT x , INT y , PRECT pWndRc , LRESULT& reResult)
	{
		reResult = HTCAPTION;
		return E_WMCN_FINISHED;
	}
};