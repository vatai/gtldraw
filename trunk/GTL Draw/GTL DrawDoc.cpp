// GTL DrawDoc.cpp : implementation of the CGTLDrawDoc class
//

#include "stdafx.h"
#include "GTL Draw.h"

#include "GTL DrawDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGTLDrawDoc

IMPLEMENT_DYNCREATE(CGTLDrawDoc, CDocument)

BEGIN_MESSAGE_MAP(CGTLDrawDoc, CDocument)
END_MESSAGE_MAP()


// CGTLDrawDoc construction/destruction

CGTLDrawDoc::CGTLDrawDoc() 
	: changed(false)
{
	// TODO: add one-time construction code here

}

CGTLDrawDoc::~CGTLDrawDoc()
{
}

BOOL CGTLDrawDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	if( changed && (AfxMessageBox( "Discard changes?" )!=IDOK) ) return FALSE;			
	changed = false;
	m_vCircle.clear();
	m_vPoly.clear();
	m_vRegPoly.clear();

	return TRUE;
}




// CGTLDrawDoc serialization

void CGTLDrawDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar << m_vCircle.size();
		for( size_t i=0; i<m_vCircle.size(); i++)
		{
			ar << m_vCircle[i].r;
			ar << m_vCircle[i].o.x;
			ar << m_vCircle[i].o.y;
		}

		
		ar << m_vPoly.size();
		for( size_t i=0; i<m_vPoly.size(); i++ )
		{
			Poly& poly = m_vPoly[i];
			size_t size = poly.size();
			ar << size;
			
			Poly::input_iterator iter = poly.get_input_iter();
			Poly::input_iterator save = iter;
			do {
				Vect &v = *iter++;
				ar << v.x;
				ar << v.y;
			} while( save != iter );

		}

		ar << m_vRegPoly.size();
		for( size_t i=0; i<m_vRegPoly.size(); i++ )
		{
			ar << m_vRegPoly[i].n;
			ar << m_vRegPoly[i].o.x;
			ar << m_vRegPoly[i].o.y;
			ar << m_vRegPoly[i].p.x;
			ar << m_vRegPoly[i].p.y;
		}
	}
	else
	{
		// TODO: add loading code here
		OnNewDocument();
		size_t size;
		scalar r, x, y;
		ar >> size;	m_vCircle.reserve(size);
		for( size_t i=0; i<size; i++)
		{
			ar >> r;
			ar >> x;
			ar >> y;
			m_vCircle.push_back( Circle( Vect(x,y),r) );
		}

		ar >> size;
		m_vPoly.reserve(size);
		for( size_t i=0; i<size; i++ )
		{
			Poly poly;
			size_t size;
			ar >> size;
			for( size_t j=0; j<size; j++ )
			{
				scalar x,y;
				ar >> x; ar >> y;
				poly.add( Vect(x,y) );
			}
			m_vPoly.push_back( poly );
		}

		ar >> size;
		m_vRegPoly.reserve(size);
		for( size_t i=0; i<size; i++ )
		{
			int n;
			scalar ox, oy, px, py;
			ar >> n;
			ar >> ox;
			ar >> oy;
			ar >> px;
			ar >> py;
			m_vRegPoly.push_back( RegPoly( Vect(ox,oy), Vect(px,py), n ) );
		}
	}
	changed  = false;
}


// CGTLDrawDoc diagnostics

#ifdef _DEBUG
void CGTLDrawDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGTLDrawDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGTLDrawDoc commands
