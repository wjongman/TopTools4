class TSnapRect
{
public:

  int left;
  int top;
  int right;
  int bottom;

//  TSnapRect()
//    {}

  TSnapRect(const TPoint& TL, const TPoint& BR)
    { left=TL.x; top=TL.y; right=BR.x; bottom=BR.y; }

  TSnapRect(WINDOWPOS* winpos)
    { left=winpos->x; top=winpos->y;
      right=winpos->x + winpos->cx; bottom=winpos->y + winpos->cy; }

  TSnapRect(int l, int t, int r, int b)
    { left=l; top=t; right=r; bottom=b; }

  TSnapRect(RECT& r)
    { left = r.left; top = r.top; right = r.right; bottom  = r.bottom; }

  bool operator ==(const TSnapRect& rc) const
    { return left==rc.left  && top==rc.top && right==rc.right && bottom==rc.bottom; }

  bool operator !=(const TSnapRect& rc) const
    { return !(rc==*this); }

  int Width () const
    { return right - left; }

  int Height() const
    { return bottom - top ; }

  // When we test for a window to snap to, we only snap right-to-left,
  // left-to-right, top-to-bottom, and bottom-to-top.
  // When we test for the screen we must reverse the order, snapping
  // the right of our window to the right edge of the screen, etc.

  void SnapToOutsideOf(const RECT& rcBounds, int SnapGap)
  // Adjust position of rcVar so it snaps outside of rcBounds.
  {
    int width = right - left;
    int height = bottom - top;

    // Snap X axis
    if (abs(left - rcBounds.right) <= SnapGap)
    {
      left = rcBounds.right;
      right = rcBounds.right + width;
    }
    else if (abs(right - rcBounds.left) <= SnapGap)
    {
      left = rcBounds.left - width;
      right = rcBounds.left;
    }
    // Snap Y axis
    if (abs(top - rcBounds.bottom) <= SnapGap)
    {
      top = rcBounds.bottom;
      bottom = rcBounds.bottom + height;
    }

    else if (abs(bottom - rcBounds.top) <= SnapGap)
    {
      top = rcBounds.top - height;
      bottom = rcBounds.top;
    }
  }

  void SnapToInsideOf(const RECT& rcBounds, int SnapGap)
  // Adjust position of rcVar so it snaps inside of rcBounds.
  {
    int width = right - left;
    int height = bottom - top;

    // Snap X axis
    if (abs(left - rcBounds.left) <= SnapGap)
    {
      left = rcBounds.left;
      right = rcBounds.left + width;
    }
    else if (abs(right - rcBounds.right) <= SnapGap)
    {
      left = rcBounds.right - width;
      right = rcBounds.right;
    }
    // Snap Y axis
    if (abs(top - rcBounds.top) <= SnapGap)
    {
      top = rcBounds.top;
      bottom = rcBounds.top + height;
    }

    else if (abs(bottom - rcBounds.bottom) <= SnapGap)
    {
      top = rcBounds.bottom - height;
      bottom = rcBounds.bottom;
    }
  }

};