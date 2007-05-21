
///////////////////////////////////////////////////////////////////
// Graphic Template Library
// 
// Vatai Emil
// vatai@inf.elte.hu
// 2007. majus
// 
// 
// Kisteleki Róbert
// kistel@elte.hu
// munkaja alapjan


#ifndef _GRAPHIC_TEMPLATE_LIBRARY
#define _GRAPHIC_TEMPLATE_LIBRARY

#include<vector>
#include<iostream>
#include<math.h>

namespace gtl {

	///////////////////////////////////////////////////////////////////
	// Konstansok

#define PI 3.1415926535897931159979634685441851615906F


	///////////////////////////////////////////////////////////////////
	// Kellekek

	// Egy pont leirasara hasznalt helyvektor
	template<typename T>
	class vect
	{
	public:
		T x,y;
		vect( T _x=0, T _y=0 ) : x(_x),y(_y) {}

		vect<T>& operator=( const vect<T>& mit )
		{
			x = mit.x;
			y = mit.y;
			return *this;
		}

		bool operator==( const vect<T>& v ) const
		{ return( x==v.x && y==v.y); }

		void rotate( double r )
		{
			T xt,yt;
			xt = T(double(x*cos(r)+y*sin(r)));
			yt = T(double(-x*sin(r)+y*cos(r)));
			x = xt;
			y = yt;
		}

		// template<typename T>
		friend std::ostream& operator<<( std::ostream& ar, const vect<T>& v );
	};

	template<typename T>
		vect<T> rotate( vect<T>& v, double rad )
	{
		vect<T> ret = v;
		ret.rotate( rad );
		return ret;
	}

	template<typename T>
		std::ostream& operator<<( std::ostream& ar, const vect<T>& v )
	{
		return ar << "X: " << v.x << " Y: " << v.y << "\n";
	}


	// Helyvektorok osszege
	template<typename T>
		inline
		vect<T>
		operator+( const vect<T>& v1, const vect<T>& v2 )
	{
		return vect<T>( v1.x+v2.x, v1.y+v2.y );
	}

	// Helyvektorok kulonbsege
	template<typename T>
		inline
		vect<T>
		operator-( const vect<T>& v1, const vect<T>& v2 )
	{
		return vect<T>( v1.x-v2.x, v1.y-v2.y );
	}

	// Helyvektor skalarral valo szorzasa
	template<typename T>
		inline
		vect<T>
		operator*( const vect<T>& v1, const T q )
	{
		return vect<T>( v1.x*q, v1.y*q );
	}


	////////////////////////////////////////////////////////////////////////
	// Adatszerkezetek


	// Altalanos poligon

	template<typename T>
	class poly
	{
		std::vector< vect<T> > v; // maguk a csucsok
	public:
		poly() {}
		poly( const vect<T> & w ) { v.push_back(w); }


		class input_iterator
		{
		public:
			typename size_t inner_iter;
			typename std::vector< vect<T> >& vi;
			typename std::vector< vect<T> > null_vector;

			typedef input_iterator iterator;

			typedef vect<T>  value_type;
			typedef vect<T>& reference_type;
			typedef vect<T>* pointer_type;

			input_iterator( std::vector< vect<T> >& vekt ) 
				: vi(vekt)
			{
				inner_iter = 0;
			}
			input_iterator() 
				: vi(null_vector)
			{
				inner_iter = 0;
			}

			iterator null_iterator() { return input_iterator(); }

			iterator& operator=( const iterator& mit )
			{
				vi = mit.vi;
				inner_iter = mit.inner_iter;
				return *this;
			}

			
			bool operator==( const iterator& x ) const 
			{ return(  
				vi==x.vi 
				&& 
				inner_iter==x.inner_iter 
				); }
			bool operator!=( const iterator& x ) const 
			{ return ! (*this==x); }

			
			value_type operator*()
			{
				if( vi.size()==0 )
					return 0;
				else
				{
					return vi[inner_iter];
				}
			}

			iterator& operator++()
			{
  				inner_iter++;
				inner_iter %= vi.size();
				// if( inner_iter==vi.end() )
				//	inner_iter = vi.begin();
				return *this;
			}

			iterator operator++(int)
			{
				iterator tmp = *this;
				++*this;
				return tmp;
			}
			iterator& operator--()
			{
				if( inner_iter==0 )
					inner_iter = vi.size();
				inner_iter--;
				return *this;
			}
			iterator operator--(int)
			{
				iterator tmp = *this;
				--*this;
				return tmp;
			}

		}; // poly::input_iterator


		class output_iterator
		{
		public:
			typename std::vector< vect<T> >::iterator inner_iter;
			typename poly<T>& parent;

			typedef output_iterator iterator;

			typedef vect<T>  value_type;
			typedef vect<T>& reference_type;
			typedef vect<T>* pointer_type;
			typedef const vect<T>& const_reference_type;

			output_iterator( poly<T>& p ) : parent(p)
			{
				inner_iter = parent.v.begin();
			}

			bool operator==( const iterator& x ) const 
			{ return( parent.v==x.v ); }
			bool operator!=( const iterator& x ) const 
			{ return ! (*this==x); }

			iterator& operator*()
			{
				return *this;
			}

			iterator& operator=( const_reference_type mit )
			{
				if( inner_iter==parent.v.end() )
				{
					parent.v.push_back( mit );
					inner_iter = parent.v.end();
					inner_iter--;
				}
				else
				{
					*inner_iter = mit;
				}

				if( inner_iter==parent.v.end() )
					inner_iter = parent.v.begin();
				else
					inner_iter++;

				return *this;
			}

			iterator& operator++()
			{
				return *this;
			}

			iterator& operator++(int)
			{
				return *this;
			}

		}; // class poly::output_iterator


		input_iterator get_input_iter() { return input_iterator( v ); }
		output_iterator get_output_iter() { return output_iterator( *this ); }

		input_iterator null_input_iter() { return input_iterator(); }

		void add( vect<T>& csucs )
		{
			v.push_back( csucs );
		}

		void del()
		{
			if( size()>0 )
				v.pop_back();
		}

		int size()
		{
			return v.size();
		}

	};


	// Szabalyos poligon

	template<typename T>
	class reg_poly
	{
	public:
		reg_poly( const vect<T>& origo=0, const vect<T>& csucs=0, int ncsucs=0 )
			: o(origo), p(csucs), n(ncsucs) {}

			vect<T> o; // origo
			vect<T> p; // egy csucs
			int n;     // csucsok szama

			class input_iterator
			{
			public:
				vect<T> o;
				vect<T> p;
				int n;
				int i;      // hanyadik csucsnal jarunk

				typedef input_iterator iterator;

				typedef vect<T>  value_type;
				typedef vect<T>& reference_type;
				typedef vect<T>* pointer_type;

				input_iterator()
					: o(0),p(0),n(0),i(0)
				{}
				input_iterator( vect<T>& origo, vect<T>& csucs, int ncsucs )
					: o(origo), p(csucs), n(ncsucs), i(0)
				{}

				iterator null_iterator() { return iterator(); }

				bool operator==( const iterator& x ) const 
				{ return( o==x.o && p==x.p && n==x.n && i==x.i ); }
				bool operator!=( const iterator& x ) const 
				{ return ! (*this==x); }

				value_type operator*()
				{
					if( n==0 )
						return o;

					vect<T> t;
					t = p-o;
					t.rotate( 2.0*PI*i/n );
					t = t+o;
					return t;
				}

				iterator& operator++()
				{
					i = (i+1)%n;
					return *this;
				}
				iterator operator++(int)
				{
					iterator tmp = *this;
					++*this;
					return tmp;
				}
				iterator& operator--()
				{
					i = (i-1+n)%n;
					return *this;
				}
				iterator operator--(int)
				{
					iterator tmp = *this;
					--*this;
					return tmp;
				}
			}; // class reg_poly::input_iterator

			class output_iterator
			{
			public:
				vect<T> csucs[3]; // a kapott csucsok
				reg_poly<T>& parent;
				int i;      // hanyadik csucsnal jarunk

				typedef output_iterator iterator;

				typedef vect<T>  value_type;
				typedef vect<T>& reference_type;
				typedef vect<T>* pointer_type;
				typedef const vect<T>& const_reference_type;

				output_iterator( reg_poly<T>& p )
					: parent(p),i(0)
				{}

				bool operator==( const iterator& x ) const 
				{ return( o==x.o && p==x.p && n==x.n && i==x.i ); }
				bool operator!=( const iterator& x ) const 
				{ return ! (*this==x); }

				iterator& operator*()
				{
					return *this;
				}

				iterator& operator=( const_reference_type mit )
				{
					switch(i) {
			case 1:
				csucs[0] = parent.o = mit;
				parent.n = 1;
				break;

			case 2:
				parent.o = (parent.o+mit)*0.5;
				csucs[1] = parent.p = mit;
				parent.n = 2;
				break;

			case 3:
				csucs[2] = mit;
				T f1x,f1y,f2x,f2y,a1,b1,a2,b2,ox,oy;
				f1x = (csucs[0].x+csucs[1].x)/2;
				f1y = (csucs[0].y+csucs[1].y)/2;
				f2x = (csucs[1].x+csucs[2].x)/2;
				f2y = (csucs[1].y+csucs[2].y)/2;
				a1  = csucs[0].x-csucs[1].x;
				b1  = csucs[0].y-csucs[1].y;
				a2  = csucs[1].x-csucs[2].x;
				b2  = csucs[1].y-csucs[2].y;
				if( a2!= 0 ) {
					oy = (a1*f2x+b2*a1/a2*f2y-a1*f1x-b1*f1y)/(b2*a1/a2-b1);
				} else {
					oy = f2y;
				}
				if( b2!=0 ) {
					ox  = (b1*f2y+a2*b1/b2*f2x-a1*f1x-b1*f1y)/(a2*b1/b2-a1);
				} else {
					ox = f2x;
				}
				parent.o = vect<T>(ox,oy);
				parent.p = csucs[0];

				double e2,f2,q;
				e2 = (csucs[0].x-csucs[1].x)*(csucs[0].x-csucs[1].x) + (csucs[0].y-csucs[1].y)*(csucs[0].y-csucs[1].y);
				f2 = (csucs[0].x-csucs[2].x)*(csucs[0].x-csucs[2].x) + (csucs[0].y-csucs[2].y)*(csucs[0].y-csucs[2].y);
				q = PI-acos(1-f2/2/e2);
				parent.n = int(floor( 0.5 + 2*PI/q ));
				break;

			default:
				break;
					}
					return *this;
				}

				iterator& operator++()
				{
					i++;
					return *this;
				}

				iterator& operator++(int)
				{
					++i;
					return *this;
				}

			}; // class reg_poly::output_iterator


			void set( vect<T>& origo, vect<T>& csucs, int ncsucs )
			{
				o = origo;
				p = csucs;
				n = ncsucs;
			}

			int size() const { return n; }

			input_iterator get_input_iter() { return input_iterator(o,p,n); }
			output_iterator get_output_iter() { return output_iterator(*this); }

			input_iterator null_input_iter() { return input_iterator(); }

	};


	// Kor

	template<typename T>
	class circle
	{
	public:
		circle( const vect<T>& origo=0, const T sugar=0 ) : o(origo), r(sugar) {}

		vect<T> o; // origo
		T r;       // sugar

		class input_iterator
		{
		public:
			vect<T> o;
			T r;
			int n;      // csucsok szama (felbontas)
			int i;      // hanyadik csucsnal jarunk

			typedef input_iterator iterator;

			typedef vect<T>  value_type;
			typedef vect<T>& reference_type;
			typedef vect<T>* pointer_type;

			input_iterator( int ncsucs=0 ) : n(ncsucs), i(0)
			{}
			input_iterator( vect<T>& origo, T sugar, int ncsucs = 0 ) 
				: o(origo), r(sugar), n(ncsucs), i(0)
			{}

			iterator null_iterator() { return iterator(); }

			bool operator==( const iterator& x ) const 
			{ return( o==x.o && r==x.r && n==x.n && i==x.i ); }
			bool operator!=( const iterator& x ) const 
			{ return ! (*this==x); }

			value_type operator*()
			{
				if( n==0 && n==1 )
					return o;

				vect<T> t(0,r);
				t.rotate( 2.0*PI*i/n );
				t = t+o;
				return t;
			}

			iterator& operator++()
			{
				i = (i+1)%n;
				return *this;
			}
			iterator operator++(int)
			{
				iterator tmp = *this;
				++*this;
				return tmp;
			}
			iterator& operator--()
			{
				i = (i-1+n)%n;
				return *this;
			}
			iterator operator--(int)
			{
				iterator tmp = *this;
				--*this;
				return tmp;
			}
		}; // circle::input_iterator

		class output_iterator
		{
		public:
			vect<T> csucs[3]; // a kapott csucsok
			circle<T>& parent;
			int i;      // hanyadik csucsnal jarunk

			typedef output_iterator iterator;

			typedef vect<T>  value_type;
			typedef vect<T>& reference_type;
			typedef vect<T>* pointer_type;
			typedef const vect<T>& const_reference_type;

			output_iterator( circle<T>& p )
				: parent(p), i(0)
			{}

			bool operator==( const iterator& x ) const 
			{ return( i==x.i && csucs[0]==x.csucs[0] && csucs[1]==x.csucs[1] && csucs[2]==x.csucs[2] ); }
			bool operator!=( const iterator& x ) const 
			{ return ! (*this==x); }

			iterator& operator*()
			{
				return *this;
			}

			iterator& operator=( const_reference_type mit )
			{
				switch(i) {
		case 1:
			csucs[0] = parent.o = mit;
			parent.r = 0;
			break;

		case 2:
			parent.o = (parent.o+mit)*0.5;
			csucs[1] = mit;
			parent.r = sqrt( (csucs[0].x-csucs[1].x)*(csucs[0].x-csucs[1].x) + (csucs[0].y-csucs[1].y)*(csucs[0].y-csucs[1].y) )/2;
			break;


		case 3:
			csucs[2] = mit;
			T f1x,f1y,f2x,f2y,a1,b1,a2,b2,ox,oy;
			f1x = (csucs[0].x+csucs[1].x)/2;
			f1y = (csucs[0].y+csucs[1].y)/2;
			f2x = (csucs[1].x+csucs[2].x)/2;
			f2y = (csucs[1].y+csucs[2].y)/2;
			a1  = csucs[0].x-csucs[1].x;
			b1  = csucs[0].y-csucs[1].y;
			a2  = csucs[1].x-csucs[2].x;
			b2  = csucs[1].y-csucs[2].y;
			if( a2!= 0 ) {
				oy = (a1*f2x+b2*a1/a2*f2y-a1*f1x-b1*f1y)/(b2*a1/a2-b1);
			} else {
				oy = f2y;
			}
			if( b2!=0 ) {
				ox  = (b1*f2y+a2*b1/b2*f2x-a1*f1x-b1*f1y)/(a2*b1/b2-a1);
			} else {
				ox = f2x;
			}
			parent.o = vect<T>(ox,oy);
			parent.r = sqrt( (ox-csucs[1].x)*(ox-csucs[1].x) + (oy-csucs[1].y)*(oy-csucs[1].y) );
			break;

		default:
			break;
				}
				return *this;
			}

			iterator& operator++()
			{
				i++;
				return *this;
			}

			iterator& operator++(int)
			{
				++i;
				return *this;
			}

		}; // circle::output_iterator


		void set( vect<T>& origo, T sugar )
		{
			o = origo;
			r = sugar;
		}

		input_iterator get_input_iter( int n=3 ) { return input_iterator( o, r, n ); }
		output_iterator get_output_iter() { return output_iterator(*this); }

		input_iterator null_input_iter() { return input_iterator(); }

	};



	////////////////////////////////////////////////////////////////////////
	// Algoritmusok

	// nonmodifying sequence

	// minden pontra meghívja a functort
	template<typename input_iterator, typename Functor>
		void for_each( input_iterator mit, Functor func )
	{
		input_iterator save = mit;
		func(*mit++);
		while(mit!=save)
			func(*mit++);
	}

	// elsõ olyan pont, amire pred igaz
	template<typename input_iterator, typename Predicate>
		input_iterator find( input_iterator mit, Predicate pred )
	{
		input_iterator save = mit;

		do {
			if( pred(*mit) )
				return mit;
			mit++;
		} while(mit!=save);
		return mit.null_iterator();
	}

	// hány olyan pont van, amire pred igaz
	template<typename input_iterator, typename Predicate>
		int count( input_iterator mit, Predicate pred )
	{
		input_iterator save = mit;
		int ret = 0;

		do {
			if( pred(*mit++) )
				ret++;
		} while(mit!=save);

		return ret;
	}

	// izomorf-e két objektum
	template<typename input_iterator1, typename input_iterator2>
		bool isomorph( input_iterator1 egy, input_iterator2 ketto )
	{
		input_iterator1 save = egy;
		bool ret = true;

		do {
			if( *egy++!=*ketto++ )
				ret = false;
		} while(save!=egy && ret);

		return ret;
	}

	// sulypont
	template<typename input_iterator>
		typename input_iterator::value_type weight_point( input_iterator mit )
	{
		typename input_iterator::value_type w;
		int n;
		input_iterator save = mit;
		w = *mit++;
		n = 1;
		while(mit!=save) {
			w = w+*mit++;
			n++;
		}
		return w*(1.0/n);
	}

	// benne van-e egy pont az objektumban
	/*
	template<typename input_iterator>
		bool in( input_iterator mit, typename input_iterator::reference_type v )
	{
		input_iterator save = mit;
		typename input_iterator::value_type cs[3];
		typename input_iterator::value_type w;

		if( v==*mit )
			return true;
		cs[0] = *mit++;
		if( mit==save )
			return false;

		cs[1] = *mit++ - cs[0];

		// ...

		return false;
	}
	//*/

	// modifying sequence op

	// transform: valójában elég sok ez (move, rotate, ...)

	// klónozás: valójában ez non-modifying!
	template<typename input_iterator, typename output_iterator>
		void clone( input_iterator mit, output_iterator hova )
	{
		input_iterator save = mit;
		*hova++ = *mit++;
		while( mit!=save )
			*hova++ = *mit++;
	}

	// elmozdítás
	template<typename input_iterator, typename output_iterator, typename T>
		void move( input_iterator mit, output_iterator hova, const vect<T>& v )
	{
		input_iterator save = mit;
		*hova++ = *mit+++v;
		while( mit!=save )
			*hova++ = *mit+++v;
	}

	// tükrözés
	template<typename input_iterator, typename output_iterator, typename T>
		void mirror( input_iterator mit, output_iterator hova, const vect<T>& v )
	{
		typename input_iterator::value_type tmp;
		input_iterator save = mit;
		do{
			tmp = *mit++; 
			tmp.x += 2*(v.x-tmp.x);
			*hova++ = tmp;
		}while( mit!=save );
	}

	// elforgatás
	template<typename input_iterator, typename output_iterator>
		void rotate( input_iterator mit, output_iterator hova, double rad )
	{
		input_iterator save = mit;
		typename input_iterator::value_type v;

		v = *mit++;
		*hova++ = rotate(v,rad);
		while( mit!=save ) {
			v = *mit++;
			*hova++ = rotate(v,rad);
		}
	}

	// elforgatás súlypont körül
	template<typename input_iterator, typename output_iterator>
		void inplace_rotate( input_iterator mit, output_iterator hova, double rad )
	{
		input_iterator save = mit;
		typename input_iterator::value_type v;
		typename input_iterator::value_type w = weight_point( mit );

		v = *mit++ - w;
		*hova++ = rotate(v,rad) + w;
		while( mit!=save ) {
			v = *mit++ - w;
			*hova++ = rotate(v,rad) + w;
		}
	}


} // namespace gtl

#endif // _GRAPHIC_TEMPLATE_LIBRARY


