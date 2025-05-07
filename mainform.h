/////////////////////////////////////////////////////////////////////////////
// Name:        mainform.h
// Purpose:     
// Author:      Sergey Talipov
// Modified by: 
// Created:     05.05.2025 22:25:56
// RCS-ID:      
// Copyright:   15681627-3C745D8E-630127F2
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _MAINFORM_H_
#define _MAINFORM_H_

/*!
 * Includes
 */

 ////@begin includes
#include "wx/frame.h"
#include "wx/gbsizer.h"
#include "wx/listctrl.h"
////@end includes

#include <sqlite3.h>  // Подключение библиотеки SQLite для работы с базой данных
#include "distro_db.h" // Подключение функций базы данных

/*!
 * Forward declarations
 */

 ////@begin forward declarations
class wxListCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

 ////@begin control identifiers
#define ID_MAINFORM 10000
#define ID_TEXTCTRL_filterCtrl 10003
#define ID_LISTCTRL 10005
#define ID_BUTTON_addBtn 10001
#define ID_BUTTON_deleteBtn 10002
#define ID_BUTTON_editBtn 10004
#define SYMBOL_MAINFORM_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_MAINFORM_TITLE wxT("База данных дистрибутивов Linux")
#define SYMBOL_MAINFORM_IDNAME ID_MAINFORM
#define SYMBOL_MAINFORM_SIZE wxSize(600, 600)
#define SYMBOL_MAINFORM_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * MainForm class declaration
 */

class MainForm : public wxFrame
{
	DECLARE_CLASS(MainForm)
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	MainForm();
	MainForm(wxWindow* parent, wxWindowID id = SYMBOL_MAINFORM_IDNAME, const wxString& caption = SYMBOL_MAINFORM_TITLE, const wxPoint& pos = SYMBOL_MAINFORM_POSITION, const wxSize& size = SYMBOL_MAINFORM_SIZE, long style = SYMBOL_MAINFORM_STYLE);

	bool Create(wxWindow* parent, wxWindowID id = SYMBOL_MAINFORM_IDNAME, const wxString& caption = SYMBOL_MAINFORM_TITLE, const wxPoint& pos = SYMBOL_MAINFORM_POSITION, const wxSize& size = SYMBOL_MAINFORM_SIZE, long style = SYMBOL_MAINFORM_STYLE);

	/// Destructor
	~MainForm();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();

	/// Обработчик для загрузки данных в ListCtrl
	void OnLoadData(wxEvent& event); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	////@begin MainForm event handler declarations

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_filterCtrl
	void OnTEXTCTRLFilterCtrlTextUpdated(wxCommandEvent& event);

	/// wxEVT_COMMAND_LIST_ITEM_ACTIVATED event handler for ID_LISTCTRL
	void OnListctrlItemActivated(wxListEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_addBtn
	void OnBUTTONAddBtnClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_deleteBtn
	void OnBUTTONDeleteBtnClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_editBtn
	void OnBUTTONEditBtnClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_EXIT
	void OnExitClick(wxCommandEvent& event);

	////@end MainForm event handler declarations


	////@begin MainForm member function declarations

	sqlite3* GetDb() const { return db; }
	void SetDb(sqlite3* value) { db = value; }

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource(const wxString& name);

	/// Retrieves icon resources
	wxIcon GetIconResource(const wxString& name);
	////@end MainForm member function declarations

	/// Should we show tooltips?
	static bool ShowToolTips();

	////@begin MainForm member variables
	wxTextCtrl* filterCtrl;
	wxListCtrl* listCtrl;
	wxButton* addBtn;
	wxButton* deleteBtn;
	wxButton* editBtn;
	wxBitmapButton* exitBtn;
protected:
	sqlite3* db;
	////@end MainForm member variables
};

#endif
// _MAINFORM_H_
