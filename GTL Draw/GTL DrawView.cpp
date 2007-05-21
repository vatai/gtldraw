// GTL DrawView.cpp : implementation of the CGTLDrawView class
//

#include "stdafx.h"
#include "GTL Draw.h"

#include "GTL DrawDoc.h"
#include "GTL DrawView.h"
#include ".\gtl drawview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace gtl;

// CGTLDrawView

IMPLEMENT_DYNCREATE(CGTLDrawView, CView)

BEGIN_MESSAGE_MAP(CGTLDrawView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_DRAW_REGULARPOLYGON, OnDrawRegularpolygon)
	ON_UPDATE_COMMAND_UI(ID_DRAW_REGULARPOLYGON, OnUpdateDrawRegularpolygon)
	ON_COMMAND(ID_DRAW_POLYGON, OnDrawPolygon)
	ON_UPDATE_COMMAND_UI(ID_DRAW_POLYGON, OnUpdateDrawPolygon)
	ON_COMMAND(ID_DRAW_CIRCLE, OnDrawCircle)
	ON_UPDATE_COMMAND_UI(ID_DRAW_CIRCLE, OnUpdateDrawCircle)
	ON_COMMAND(ID_TRANSFORM_MIRROR, OnTransformMirror)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_MIRROR, OnUpdateTransformMirror)
	ON_COMMAND(ID_TRANSFORM_MOVE, OnTransformMove)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_MOVE, OnUpdateTransformMove)
	ON_COMMAND(ID_TRANSFORM_ROTATE, OnTransformRotate)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_ROTATE, OnUpdateTransformRotate)
	ON_WM_LBUTTONUP()
//	ON_COMMAND(ID_FILE_NEW, OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
//	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
//	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CGTLDrawView construction/destruction

CGTLDrawView::CGTLDrawView()
: m_Bitmap(NULL), m_N(5), m_state( POLY ), m_selected( MOVE )
{
	// TODO: add construction code here
}

CGTLDrawView::~CGTLDrawView()
{
}

BOOL CGTLDrawView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGTLDrawView drawing

void CGTLDrawView::OnDraw(CDC* pDC)
{
	CGTLDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	if( pDoc->m_vRegPoly.size() + 
		pDoc->m_vCircle.size() + 
		pDoc->m_vPoly.size() > 0 )
	{
		CRect rect;
		GetClientRect( rect );
		Bitmap bmp( rect.Width(), rect.Height() );
		Graphics* graph = Graphics::FromImage(&bmp);

		Pen pen(Color(128,0,255));

		for( size_t i=0; i < pDoc->m_vCircle.size(); i++ )
			Draw( pDoc->m_vCircle[i], *graph, pen );
		
		for( size_t i=0; i < pDoc->m_vPoly.size(); i++ )
			Draw( pDoc->m_vPoly[i], *graph, pen );

		for( size_t i=0; i < pDoc->m_vRegPoly.size(); i++ )
			Draw( pDoc->m_vRegPoly[i], *graph, pen );

		Pen sp( Color( 255,0,0), 3); // pen for drawing the selected shape
		if( m_selected < MOVE )
		{
			try
			{
				switch(m_selected)
				{
				case CIRCLE :
					Draw(pDoc->m_vCircle.at(m_index), *graph, sp );
					break;
				case POLY :
					Draw(pDoc->m_vPoly.at(m_index), *graph, sp );
					break;
				case REGPOLY :
					Draw(pDoc->m_vRegPoly.at(m_index), *graph, sp );
					break;
				}
			} catch(...){}
		}
		Rect rc(rect.left,rect.top,rect.Width(),rect.Height());

		// Clone the bitmap
		m_Bitmap = bmp.Clone(rc, PixelFormatDontCare);
		
		// Draw the bitmap graphics
		Graphics graphics(pDC->m_hDC);
		graphics.DrawImage(m_Bitmap, rc);
	}
}


// CGTLDrawView printing

BOOL CGTLDrawView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGTLDrawView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGTLDrawView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGTLDrawView diagnostics

#ifdef _DEBUG
void CGTLDrawView::AssertValid() const
{
	CView::AssertValid();
}

void CGTLDrawView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGTLDrawDoc* CGTLDrawView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGTLDrawDoc)));
	return (CGTLDrawDoc*)m_pDocument;
}
#endif //_DEBUG


// CGTLDrawView message handlers


void CGTLDrawView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CGTLDrawDoc* pDoc = GetDocument();
	CGTLDrawDoc::vCircle &vCircle	= pDoc->m_vCircle;
	CGTLDrawDoc::vPoly &vPoly		= pDoc->m_vPoly;
	CGTLDrawDoc::vRegPoly &vRegPoly = pDoc->m_vRegPoly;

	if( vPoly.size() < 1 && POLY_==m_state ) m_state=POLY;
	switch(m_state)
	{
	case POLY:
		vPoly.push_back( CGTLDrawDoc::Poly());
		vPoly[vPoly.size()-1].add( ToVect(point) );
		m_state = POLY_;
		break;
	case POLY_:
		vPoly[vPoly.size()-1].add(ToVect(point));
		break;
	case REGPOLY:
		m_VectBuf = ToVect(point);
		m_state = REGPOLY_;
		break;
	case REGPOLY_:
		vRegPoly.push_back(CGTLDrawDoc::RegPoly( m_VectBuf, ToVect(point), m_N));
		m_state = REGPOLY;
		m_VectBuf = CGTLDrawDoc::Vect();
		break;
	case CIRCLE:
		m_VectBuf = ToVect(point);
		m_state = CIRCLE_;
		break;
	case CIRCLE_:
		vCircle.push_back(CGTLDrawDoc::Circle( m_VectBuf, VectDist( m_VectBuf, ToVect(point) )));
		m_state = CIRCLE;
		m_VectBuf = CGTLDrawDoc::Vect();
		break;
	default:
		break;
	}

	if( m_state >= MOVE )
	{
		m_selected = MOVE;
		FindHit( vCircle, CIRCLE, point );
		FindHit( vPoly, POLY, point );
		FindHit( vRegPoly, REGPOLY, point );
		if( m_selected < MOVE ) // found a hit
			m_VectBuf = ToVect(point);
	}
	pDoc->SetChanged();

	Invalidate();

	CView::OnLButtonDown(nFlags, point);
}

void CGTLDrawView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if( m_state < MOVE && m_selected < MOVE) return; // drag&drop only used with transformations

	CGTLDrawDoc::Vect vec = ToVect( point );
	CGTLDrawDoc::Circle *pc =0;
	CGTLDrawDoc::Poly *pp =0;
	CGTLDrawDoc::RegPoly *pr =0;
	try{
		switch(m_selected)
		{
		case CIRCLE:
			pc = &(GetDocument()->m_vCircle.at(m_index));
			break;
		case POLY:
			pp = &(GetDocument()->m_vPoly.at(m_index));
			break;
		case REGPOLY:
			pr = &(GetDocument()->m_vRegPoly.at(m_index));
			break;
		}
	}catch(...){}

	switch(m_state)
	{
	case MOVE:
		Move( pc, vec-m_VectBuf );
		Move( pp, vec-m_VectBuf );
		Move( pr, vec-m_VectBuf );
		break;
	case ROTATE:
		Rotate( pp, vec, m_VectBuf );
		Rotate( pr, vec, m_VectBuf );
		// no need to rotate a circle!
		break;
	case MIRROR:
		Mirror( pc, vec );
		Mirror( pp, vec );
		Mirror( pr, vec );
		break;
	}
	
	m_selected = MOVE;
	Invalidate();

	CView::OnLButtonUp(nFlags, point);
}

void CGTLDrawView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(m_state==POLY_) m_state=POLY;
	CView::OnRButtonDown(nFlags, point);
}

void CGTLDrawView::OnDrawRegularpolygon()
{
	m_state = REGPOLY;
	GetNDlg d;
	d.m_Nval = 3;
	if( d.DoModal() == IDOK )
		m_N = d.m_Nval;
	else return;
}

void CGTLDrawView::OnUpdateDrawRegularpolygon(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( m_state == REGPOLY || m_state == REGPOLY_ );
}

void CGTLDrawView::OnDrawPolygon()
{
	m_state = POLY;
}

void CGTLDrawView::OnUpdateDrawPolygon(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( m_state == POLY || m_state == POLY_ );
}

void CGTLDrawView::OnDrawCircle()
{
	m_state = CIRCLE;
}

void CGTLDrawView::OnUpdateDrawCircle(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( m_state == CIRCLE || m_state == CIRCLE_ );
}

void CGTLDrawView::OnTransformMirror()
{
	m_state = MIRROR;
}

void CGTLDrawView::OnUpdateTransformMirror(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( m_state == MIRROR );
}

void CGTLDrawView::OnTransformMove()
{
	m_state = MOVE;
}

void CGTLDrawView::OnUpdateTransformMove(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( m_state == MOVE );
}

void CGTLDrawView::OnTransformRotate()
{
	m_state = ROTATE;
}

void CGTLDrawView::OnUpdateTransformRotate(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( m_state == ROTATE );
}

