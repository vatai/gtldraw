// GTL DrawView.h : interface of the CGTLDrawView class
//


#pragma once


class CGTLDrawView : public CView
{
protected: // create from serialization only
	CGTLDrawView();
	DECLARE_DYNCREATE(CGTLDrawView)

// Attributes
public:
	CGTLDrawDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CGTLDrawView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
private:

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnDrawRegularpolygon();
	afx_msg void OnUpdateDrawRegularpolygon(CCmdUI *pCmdUI);
	afx_msg void OnDrawPolygon();
	afx_msg void OnUpdateDrawPolygon(CCmdUI *pCmdUI);
	afx_msg void OnDrawCircle();
	afx_msg void OnUpdateDrawCircle(CCmdUI *pCmdUI);
	afx_msg void OnTransformMirror();
	afx_msg void OnUpdateTransformMirror(CCmdUI *pCmdUI);
	afx_msg void OnTransformMove();
	afx_msg void OnUpdateTransformMove(CCmdUI *pCmdUI);
	afx_msg void OnTransformRotate();
	afx_msg void OnUpdateTransformRotate(CCmdUI *pCmdUI);

private:
	// states
	enum state{ POLY, POLY_, REGPOLY, REGPOLY_, CIRCLE, CIRCLE_, MOVE, ROTATE, MIRROR } m_state;

	int m_N; // number of sides of reg_poly
	Bitmap *m_Bitmap; // screen buffer
	CGTLDrawDoc::Vect m_VectBuf; // a vector buffer, for storing the 1st click
	
	size_t m_index;  // index of the selected shape
	state m_selected; // the kind of selected shape (CIRCLE,POLY or REGPOLY; MOVE if none)

	// helper functions
	// point <-> Vect conversion
	Point ToPoint( const CGTLDrawDoc::Vect& v ) { return Point(v.x,v.y); }
	CGTLDrawDoc::Vect ToVect( const CPoint& p ) { return CGTLDrawDoc::Vect( p.x, p.y ); }
	
	// vect vect distance calculation
	CGTLDrawDoc::scalar VectDist( const CGTLDrawDoc::Vect& v1, const CGTLDrawDoc::Vect& v2 )
	{ return (sqrt( double((v2.x-v1.x)*(v2.x-v1.x) + (v2.y-v1.y)*(v2.y-v1.y) ) ) ); }


	// template functions
public:
	// Draw shape
 	template<typename T> void Draw( T& t, Graphics& g, Pen& p )
	{
		if( t.size() < 2 ) return;

		T::input_iterator iter, save;
		save = iter = t.get_input_iter();
		
		Point p1,p2;
		p1 = p2 = ToPoint( *iter );

		do
		{
			iter++;
			p2 = ToPoint( *iter );
			g.DrawLine(&p, p1, p2);
			p1 = p2;
		} while(iter!=save);
	}

	// Draw circle (specialization)
	template<> void Draw<CGTLDrawDoc::Circle>( CGTLDrawDoc::Circle& t, Graphics& g, Pen& p )
	{ g.DrawEllipse( &p, Rect(t.o.x-t.r,t.o.y-t.r,2*t.r,2*t.r) ); }

	// Point p on shape t
	template<typename T> BOOL Hit( T& t, CPoint &p )
	{
		BOOL hit = FALSE;

		Pen pen(Color(255,0,255),3);
		GraphicsPath path;
		T::input_iterator iter, save;
		save = iter = t.get_input_iter();
		
		Point p1,p2;
		p1 = p2 = ToPoint( *iter );

		do
		{
			iter++;
			p2 = ToPoint( *iter );
			path.AddLine( p1, p2 );
			hit = path.IsOutlineVisible(p.x, p.y, &pen);
			p1 = p2;
		} while( !hit && iter!=save);

		return hit;

	}
    	
	template<typename T> void FindHit( T& v, const state s, CPoint &point )
	{
		for( size_t i=0; i<v.size(); i++ )
			if( Hit(v[i],point ) )
			{
				m_index = i;
				m_selected = s;
				m_N = v[i].size();
				return;
			}
	}
	
	template<> void FindHit( CGTLDrawDoc::vCircle &v, const state s, CPoint &point )
	{
		for( size_t i=0; i<v.size(); i++)
		{
			if( abs(VectDist(v[i].o, ToVect(point)) - v[i].r )< 3 )
			{
				m_index = i;
				m_selected = CIRCLE;
				m_N = 0;
				return;
			}
		}
	}

	template<typename T> void Move( T* pc, const CGTLDrawDoc::Vect& v )
	{ if(pc) move(pc->get_input_iter(),pc->get_output_iter(),v ); }
	
	template<typename T> void Rotate( T* pc, const CGTLDrawDoc::Vect& v1, const CGTLDrawDoc::Vect& v2) 
	{ 
		if(pc){
			CGTLDrawDoc::Vect wp = weight_point( pc->get_input_iter() );
			CGTLDrawDoc::scalar dx1, dx2, dy1, dy2;
			double rad;
			
			dx1=wp.x-v1.x; dy1=wp.y-v1.y;
			dx2=wp.x-v2.x; dy2=wp.y-v2.y;
			rad=atan2(dy1,dx1);
			rad=atan2(dy2,dx2)-rad;
			inplace_rotate(pc->get_input_iter(), pc->get_output_iter(), rad );
		}
	}

	template<typename T> void Mirror( T* pc, const CGTLDrawDoc::Vect& v )
	{ if(pc) mirror(pc->get_input_iter(),pc->get_output_iter(), v); }

};

#ifndef _DEBUG  // debug version in GTL DrawView.cpp
inline CGTLDrawDoc* CGTLDrawView::GetDocument() const
   { return reinterpret_cast<CGTLDrawDoc*>(m_pDocument); }
#endif


