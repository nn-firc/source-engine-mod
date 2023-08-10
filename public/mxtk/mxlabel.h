//
//                 mxToolKit (c) 1999 by Mete Ciragan
//
// file:           mxLabel.h
// implementation: all
// last modified:  Mar 14 1999, Mete Ciragan
// copyright:      The programs and associated files contained in this
//                 distribution were developed by Mete Ciragan. The programs
//                 are not in the public domain, but they are freely
//                 distributable without licensing fees. These programs are
//                 provided without guarantee or warrantee expressed or
//                 implied.
//
#ifndef INCLUDED_MXLABEL
#define INCLUDED_MXLABEL



#ifndef INCLUDED_MXWIDGET
#include "mxtk/mxWidget.h"
#endif



class mxWindow;



class mxLabel_i;
class mxLabel : public mxWidget
{
	mxLabel_i *d_this;

public:
	// CREATORS
	mxLabel (mxWindow *parent, int x, int y, int w, int h, const char *label = 0);
	virtual ~mxLabel ();

private:
	// NOT IMPLEMENTED
	mxLabel (const mxLabel&);
	mxLabel& operator= (const mxLabel&);
};



#endif // INCLUDED_MXLABEL
