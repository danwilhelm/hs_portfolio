class CAnimation
{

private:

public:
	float x, y;				// x and y coordinate of animation
	float x0, y0;				// x and y velocity of animation
	int frame, numframes;	// frame and the total frames
	int width, height;

	int state, type;		// state of animation, type of animation

	CSurface *surface[16];		// dynamic array of images
	
	CAnimation *attanimation;	// attached surface

	int (*LogicFunc)(CAnimation *aniptr);  // logic function ptr to determine move

	
	CAnimation(void);
	int Animate(CSurface *dest);

	CAnimation &operator=(CAnimation &ani);
};


CAnimation &CAnimation::operator=(CAnimation &ani)
{
	CAnimation tempani;

	memcpy(&tempani, &ani, sizeof(CAnimation));

	return tempani;
}


CAnimation::CAnimation(void)
{
	x  = 0;
	y  = 0;
	x0 = 0;
	y0 = 0;

	state = 0;
	type  = 0;

	width = 0;
	height = 0;

	frame = 0;
	numframes = 0;
	LogicFunc = NULL;
}


int CAnimation::Animate(CSurface *dest)
{
	int retval = 0;

	// move the image
	x += x0;
	y += y0;

	// call the object's logic
	if(this->LogicFunc)
		retval = this->LogicFunc(this);

	// blit it to the screen
	dest->Blit(surface[frame], x, y);
	
	// cycle through the animation
	if(++frame >= numframes)
		frame = 0;

	return retval;
}
