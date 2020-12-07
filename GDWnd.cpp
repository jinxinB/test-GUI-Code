#include "StdAfx.h"
#include "GDWnd.h"


CGDWnd::CGDWnd(void)
{
	// 置零，由其它函数设置初始值
	m_state = 0;
	// 默认值
	m_zorder = 0;

	m_CtrlID = -1;

	m_pParent = NULL;
	m_pChild = NULL;
	m_pLeft = NULL;
	m_pRight = NULL;
	m_pWndManager = NULL;

	//m_fun_OnLButtonDown = NULL;
	//m_fun_OnRButtonDown = NULL;
	//m_fun_OnMButtonDown = NULL;
	//m_fun_OnLButtonUp = NULL;
	//m_fun_OnRButtonUp = NULL;
	//m_fun_OnMButtonUp = NULL;
	//m_fun_OnMouseMove = NULL;
	//m_fun_OnMouseHover = NULL;
	//m_fun_OnMouseWheel = NULL;
	//m_fun_OnMouseMoveIn = NULL;
	//m_fun_OnMouseMoveOut = NULL;
	//m_fun_OnKeyDown = NULL;
	//m_fun_OnKeyUp = NULL;
	//m_fun_OnChar = NULL;
	//m_fun_OnCommand = NULL;
}


CGDWnd::~CGDWnd(void)
{
	CGDWnd* pWnd;
	ASSERT(m_pChild == NULL || m_pChild->m_pLeft == NULL );
	while( m_pChild != NULL )
	{
		pWnd = m_pChild;
		m_pChild = m_pChild->m_pRight;
		pWnd->Release();
	}
}
void CGDWnd::InvalidateRect(RECT& rc)
{
	RECT r;
	SetDirtyState(TRUE);
	if( m_pWndManager != NULL && m_pWndManager->hWnd != NULL )
	{
		r.left = rc.left + m_rect.left;
		r.right = rc.right + m_rect.left;
		r.top = rc.top + m_rect.top;
		r.bottom = rc.bottom + m_rect.top;
		if( m_pParent != NULL )
		{
			m_pParent->InvalidateRect(r);
		}
		else
		{
			::InvalidateRect(m_pWndManager->hWnd , &r , FALSE);
		}
	}
}
void CGDWnd::Invalidate()
{
	SetDirtyState(TRUE);
	if( m_pWndManager != NULL && m_pWndManager->hWnd != NULL )
	{
		if( m_pParent != NULL )
		{
			m_pParent->InvalidateRect(m_rect);
		}
		else
		{
			::InvalidateRect(m_pWndManager->hWnd , &m_rect , FALSE);
		}
	}
}
void CGDWnd::AddChild(CGDWnd* pWnd , INT zorder)
{
	CGDWnd* pTmpWnd;
	ASSERT(m_pChild == NULL || m_pChild->m_pLeft == NULL );
	INT ti;
	pWnd->m_pWndManager = m_pWndManager;
	if( m_pWndManager != NULL && pWnd->m_CtrlID < 0 )
		pWnd->m_CtrlID = m_pWndManager->m_LastDefaultId++;
	// 查找窗口位置
	pTmpWnd = m_pChild;
	while( pTmpWnd != NULL && pTmpWnd->m_pRight != NULL )
	{
		if( zorder != 0 && zorder < pTmpWnd->m_zorder )
			break;
		pTmpWnd = pTmpWnd->m_pRight;
	}
	// 没有子窗口
	if( pTmpWnd == NULL )
	{
		pWnd->m_pParent = this;
		pWnd->m_pLeft = NULL;
		pWnd->m_pRight = NULL;
		pWnd->m_pChild = NULL;
		pWnd->m_zorder = 0;
		m_pChild = pWnd;
	}
	// 有子窗口
	else
	{
		// 放在最后
		if( pTmpWnd->m_pRight == NULL && (zorder == 0 || pTmpWnd->m_zorder < zorder) )
		{
			pWnd->m_pParent = this;
			pWnd->m_pLeft = pTmpWnd;
			pWnd->m_pRight = NULL;
			pWnd->m_pChild = NULL;
			pWnd->m_zorder = pTmpWnd->m_zorder + 100;
			pTmpWnd->m_pRight = pWnd;
		}
		// 放在左边
		else
		{
			pWnd->m_pParent = this;
			pWnd->m_pLeft = pTmpWnd->m_pLeft;
			pWnd->m_pRight = pTmpWnd;
			pWnd->m_pChild = NULL;
			// 放在第一个后面的位置
			if( pTmpWnd->m_pLeft != NULL )
			{
				pTmpWnd->m_pLeft->m_pRight = pWnd;
				pWnd->m_zorder = (pTmpWnd->m_zorder + pTmpWnd->m_pLeft->m_zorder)/2;
				pTmpWnd->m_pLeft = pWnd;
			}
			// 放在第一个位置
			else
			{
				pTmpWnd->m_pLeft = pWnd;
				// 如果图层位置值太小
				if( pTmpWnd->m_zorder < 100 )
				{
					ti = 100 - pTmpWnd->m_zorder;
					pWnd->m_zorder = 0;
					while( pTmpWnd != NULL )
					{
						pTmpWnd->m_zorder += ti;
						pTmpWnd = pTmpWnd->m_pRight;
					}
				}
				// 如果图层位置值可用
				else
				{
					pWnd->m_zorder = pTmpWnd->m_zorder - 100;
				}// 如果图层位置值可用
			}// 放在第一个位置
		}// 放在左边
	}// 有子窗口
	if( m_pWndManager != NULL )
	{
		m_pWndManager->AddCtrl(pWnd);
	}
}
//CGDWnd* CGDWnd::PostMessage(UINT nID, UINT wmEvent, HWND hCtrlWnd)
//{
//	return E_WMCN_End;
//}
BOOL CGDWnd::PostMessage(UINT nID, UINT wmEvent, HWND hCtrlWnd)
{
	return TRUE;
}
BOOL CGDWnd::SetTimer(UINT idEvent , UINT uElapse , TIMERPROC lpTimerFunc)
{
	if( m_pWndManager == NULL )
		return FALSE;
	ASSERT(m_pWndManager->GetItemFromId(m_CtrlID) != NULL);
	::SetTimer(m_pWndManager->hWnd , MAKELONG(m_CtrlID,idEvent) , uElapse , lpTimerFunc);
	return TRUE;
}
BOOL CGDWnd::KillTimer(UINT idEvent)
{
	if( m_pWndManager == NULL )
		return FALSE;
	ASSERT(m_pWndManager->GetItemFromId(m_CtrlID) != NULL);
	::KillTimer(m_pWndManager->hWnd , MAKELONG(m_CtrlID,idEvent));
	return TRUE;
}
BOOL CGDWnd::SetId(INT id)
{
	if( m_CtrlID >= 0 )
		return FALSE;
	if( m_pWndManager != NULL && m_pWndManager->GetItemFromId(id) == NULL )
		return FALSE;
	m_CtrlID = id;
	return TRUE;
}