BOOL CRegion::CreateRegion(CScreen *scrn, UINT x, UINT y, UINT width, UINT height)
{
	if(x == NULL) x = 0;
	if(y == NULL) y = 0;
	
	if(width  == NULL) width  = scrn->scrinfo.width;
	if(height == NULL) height = scrn->scrinfo.height;
	
	screen			= scrn;
	boundary.left	= x;
	boundary.right	= x + width;
	boundary.top	= y;
	boundary.bottom	= y + height;

	scrpitch		= scrn->pitch;
	scrbytespp		= scrn->bytespp;

	return TRUE;
}