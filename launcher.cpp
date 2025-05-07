/////////////////////////////////////////////////////////////////////////////
// Name:        launcher.cpp
// Purpose:     
// Author:      Sergey Talipov
// Modified by: 
// Created:     07.05.2025 10:11:53
// RCS-ID:      
// Copyright:   15681627-3C745D8E-630127F2
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "launcher.h"

////@begin XPM images

////@end XPM images


/*
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( DialogBlocks_SQLite_vext )
////@end implement app


/*
 * DialogBlocks_SQLite_vext type definition
 */

IMPLEMENT_CLASS( DialogBlocks_SQLite_vext, wxApp )


/*
 * DialogBlocks_SQLite_vext event table definition
 */

BEGIN_EVENT_TABLE( DialogBlocks_SQLite_vext, wxApp )

////@begin DialogBlocks_SQLite_vext event table entries
////@end DialogBlocks_SQLite_vext event table entries

END_EVENT_TABLE()


/*
 * Constructor for DialogBlocks_SQLite_vext
 */

DialogBlocks_SQLite_vext::DialogBlocks_SQLite_vext()
{
    Init();
}


/*
 * Member initialisation
 */

void DialogBlocks_SQLite_vext::Init()
{
////@begin DialogBlocks_SQLite_vext member initialisation
////@end DialogBlocks_SQLite_vext member initialisation
}

/*
 * Initialisation for DialogBlocks_SQLite_vext
 */

bool DialogBlocks_SQLite_vext::OnInit()
{    
////@begin DialogBlocks_SQLite_vext initialisation
    // Remove the comment markers above and below this block
    // to make permanent changes to the code.

#if wxUSE_XPM
    wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
    wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
    wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
    wxImage::AddHandler(new wxGIFHandler);
#endif
    MainForm* mainWindow = new MainForm( NULL );
    mainWindow->Show(true);
////@end DialogBlocks_SQLite_vext initialisation

    return true;
}


/*
 * Cleanup for DialogBlocks_SQLite_vext
 */

int DialogBlocks_SQLite_vext::OnExit()
{    
////@begin DialogBlocks_SQLite_vext cleanup
    return wxApp::OnExit();
////@end DialogBlocks_SQLite_vext cleanup
}

