/************************************************************************
	CPolygon.h - by Daniel Wilhelm
	This document is part of the XMedia Programmer's Library (XMPL)
	
	No copying or modifying of this code is allowed.
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/


struct VERTEX2D
{
	float x;
	float y;
};


struct VERTEX3D
{
	float x;
	float y;
	float z;
};


class CPolygon
{
	friend class CSurface;

	private:
		VERTEX2D *vertex;
		VERTEX2D *orig;
	
		int		 maxvertex;
	
	public:
		RECT	box;
		xmCOLOR	color;
		int		x, y;
		int		xv, yv;

		CPolygon(void);
		Release(void);

		inline void Velocity(int velx, int vely);
		inline void GetRect(void);
		inline void Rotate(float angle);
		inline void Scale(float scalex, float scaley);
		inline void Color(xmCOLOR newcolor);
		inline void Translate(void);
		inline void Restore(void);
		
		void Create(VERTEX2D *vertices, int maxvertices, xmCOLOR polycolor);
};