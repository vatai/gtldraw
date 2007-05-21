// GTL DrawDoc.h : interface of the CGTLDrawDoc class
//


#pragma once

class CGTLDrawDoc : public CDocument
{
protected: // create from serialization only
	CGTLDrawDoc();
	DECLARE_DYNCREATE(CGTLDrawDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CGTLDrawDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:
	// typedefs
	typedef double scalar;
	typedef gtl::vect<scalar> Vect;

	typedef gtl::circle<scalar>		Circle;
	typedef gtl::poly<scalar>		Poly;
	typedef gtl::reg_poly<scalar>	RegPoly;

	typedef std::vector<Circle>		vCircle;
	typedef std::vector<Poly>		vPoly;
	typedef std::vector<RegPoly>	vRegPoly;
	
	// members
	vCircle		m_vCircle;
	vPoly		m_vPoly;
	vRegPoly	m_vRegPoly;
	void SetChanged(bool l=true){ changed = l; }
private:
	bool changed;

};


