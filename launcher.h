/////////////////////////////////////////////////////////////////////////////
// Name:        launcher.h
// Purpose:     
// Author:      Sergey Talipov
// Modified by: 
// Created:     07.05.2025 10:11:53
// RCS-ID:      
// Copyright:   15681627-3C745D8E-630127F2
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _LAUNCHER_H_
#define _LAUNCHER_H_


/*!
 * Includes
 */

 ////@begin includes
#include "wx/image.h"
#include "mainform.h"
////@end includes

/*!
 * Forward declarations
 */

 ////@begin forward declarations
 ////@end forward declarations

 /*!
  * Control identifiers
  */

  ////@begin control identifiers
  ////@end control identifiers

  /*!
   * DialogBlocks_SQLite_vext class declaration
   */

class DialogBlocks_SQLite_vext : public wxApp
{
	DECLARE_CLASS(DialogBlocks_SQLite_vext)
	DECLARE_EVENT_TABLE()

public:
	/// Constructor
	DialogBlocks_SQLite_vext();

	void Init();

	/// Initialises the application
	virtual bool OnInit();

	/// Called on exit
	virtual int OnExit();

	////@begin DialogBlocks_SQLite_vext event handler declarations
	////@end DialogBlocks_SQLite_vext event handler declarations

	////@begin DialogBlocks_SQLite_vext member function declarations
	////@end DialogBlocks_SQLite_vext member function declarations

	////@begin DialogBlocks_SQLite_vext member variables
	////@end DialogBlocks_SQLite_vext member variables
};

/*!
 * Application instance declaration
 */

 ////@begin declare app
DECLARE_APP(DialogBlocks_SQLite_vext)
////@end declare app

#endif
	// _LAUNCHER_H_
