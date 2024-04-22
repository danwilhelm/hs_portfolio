/************************************************************************
	CPolygon.cpp - by Daniel Wilhelm
	This document is part of the XMedia Programmer's Library (XMPL)
	
	No copying or modifying of this code is allowed.
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/


inline void CPolygon::Velocity(int velx, int vely)
{
	xv = velx;
	yv = vely;
}


// returns a bounding box around the polygon
inline void CPolygon::GetRect(void)
{
	box.left = box.right = box.top = box.bottom = 0;
	
	// loop through each vertex and find the max x, y, min x, y
	for(int i=0;i<=maxvertex;i++)
	{
		if(vertex[i].x < box.left)	 box.left	= (DWORD)vertex[i].x;
		if(vertex[i].x > box.right)	 box.right  = (DWORD)vertex[i].x;

		if(vertex[i].y < box.top)	 box.top	= (DWORD)vertex[i].y;
		if(vertex[i].y > box.bottom) box.bottom = (DWORD)vertex[i].y;
	}

	box.left   += x;
	box.right  += x;
	box.top	   += y;
	box.bottom += y;
}


// rotate using standard sin and cos trig
// FIXME: USE XMEDIA LOOKUP TABLES!!!!
inline void CPolygon::Rotate(float angle)
{
	for(int i=0;i<=maxvertex;i++)
	{
		float tempx = vertex[i].x;

		vertex[i].x = (float)(tempx * cos(angle) - vertex[i].y * sin(angle));
		vertex[i].y = (float)(tempx * sin(angle) + vertex[i].y * cos(angle));
	}
}


inline void CPolygon::Scale(float scalex, float scaley)
{
	for(int i=0;i<=maxvertex;i++)
	{
		vertex[i].x = vertex[i].x * scalex;
		vertex[i].y = vertex[i].y * scaley;
	}
}


inline void CPolygon::Color(xmCOLOR newcolor)
{
	color = newcolor;
}


inline void CPolygon::Translate(void)
{
	x += xv;
	y += yv;
}


inline void CPolygon::Restore(void)
{
	memcpy(vertex, orig, sizeof(VERTEX2D)*(maxvertex + 1));
}


void CPolygon::Create(VERTEX2D *vertices, int numvertices, xmCOLOR polycolor)
{
	if(vertex) delete [] vertex;
	if(orig)   delete [] orig;
	
	vertex = new VERTEX2D[numvertices];
	orig   = new VERTEX2D[numvertices];

	memcpy(vertex, vertices, sizeof(VERTEX2D)*numvertices); 
	memcpy(orig  , vertices, sizeof(VERTEX2D)*numvertices);
	ZeroMemory(&box, sizeof(RECT));

	maxvertex  = numvertices - 1;
	color	   = polycolor;

	x  = 0;
	y  = 0;
	xv = 0;
	yv = 0; 
}


CPolygon::CPolygon(void)
{
	vertex = NULL;
	orig   = NULL;
}


CPolygon::Release(void)
{
	if(vertex)	delete [] vertex;
	if(orig)	delete [] orig;
}
