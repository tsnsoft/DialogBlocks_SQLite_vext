/////////////////////////////////////////////////////////////////////////////
// Name:        mainform.cpp
// Purpose:     
// Author:      Sergey Talipov
// Modified by: 
// Created:     05.05.2025 22:25:56
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
#include "wx/imaglist.h"
////@end includes

#include "mainform.h"
#include "distro_db.h"       // Подключение функций базы данных

#include <thread>

////@begin XPM images
#include "tsnsoft.xpm"
#include "quit.xpm"
////@end XPM images

/*
 * MainForm type definition
 */

IMPLEMENT_CLASS(MainForm, wxFrame)

// Определение пользовательского события
wxDEFINE_EVENT(EVT_LOAD_DATA, wxThreadEvent); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

/*
 * MainForm event table definition
 */

BEGIN_EVENT_TABLE(MainForm, wxFrame)

////@begin MainForm event table entries
EVT_TEXT(ID_TEXTCTRL_filterCtrl, MainForm::OnTEXTCTRLFilterCtrlTextUpdated)
EVT_LIST_ITEM_ACTIVATED(ID_LISTCTRL, MainForm::OnListctrlItemActivated)
EVT_BUTTON(ID_BUTTON_addBtn, MainForm::OnBUTTONAddBtnClick)
EVT_BUTTON(ID_BUTTON_deleteBtn, MainForm::OnBUTTONDeleteBtnClick)
EVT_BUTTON(ID_BUTTON_editBtn, MainForm::OnBUTTONEditBtnClick)
EVT_BUTTON(wxID_EXIT, MainForm::OnExitClick)
////@end MainForm event table entries

EVT_CUSTOM(EVT_LOAD_DATA, wxID_ANY, MainForm::OnLoadData) // Обработчик события загрузки данных 

END_EVENT_TABLE()

/*
 * MainForm constructors
 */

	MainForm::MainForm()
{
	Init();
}

MainForm::MainForm(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
	Init();
	Create(parent, id, caption, pos, size, style);

	// Добавлеие в список listCtrl колонок
	listCtrl->AppendColumn(wxT("ID"), wxLIST_FORMAT_LEFT, 50);
	listCtrl->AppendColumn(wxT("Название"), wxLIST_FORMAT_LEFT, 140);
	listCtrl->AppendColumn(wxT("Основатель"), wxLIST_FORMAT_LEFT, 140);
	listCtrl->AppendColumn(wxT("Год выпуска"), wxLIST_FORMAT_LEFT, 100);

	// Инициализация базы данных и загрузка данных
	InitDatabase(db); // Инициализация базы данных
	LoadDataAsync(db, this); // Асинхронная загрузка данных
}

/*
 * MainForm creator
 */

bool MainForm::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
	////@begin MainForm creation
	wxFrame::Create(parent, id, caption, pos, size, style);

	CreateControls();
	SetIcon(GetIconResource(wxT("tsnsoft.xpm")));
	if (GetSizer())
	{
		GetSizer()->SetSizeHints(this);
	}
	Centre();
	////@end MainForm creation

	return true;
}

/*
 * MainForm destructor
 */

MainForm::~MainForm()
{
	// Освобождение ресурсов базы данных
	if (db)
		sqlite3_close(db);
}

/*
 * Member initialisation
 */

void MainForm::Init()
{
	////@begin MainForm member initialisation
	filterCtrl = NULL;
	listCtrl = NULL;
	addBtn = NULL;
	deleteBtn = NULL;
	editBtn = NULL;
	exitBtn = NULL;
	////@end MainForm member initialisation
}

/*
 * Control creation for MainForm
 */

void MainForm::CreateControls()
{
	////@begin MainForm content construction
	MainForm* itemFrame1 = this;

	wxGridBagSizer* itemGridBagSizer1 = new wxGridBagSizer(0, 0);
	itemGridBagSizer1->SetEmptyCellSize(wxSize(10, 19));
	itemFrame1->SetSizer(itemGridBagSizer1);

	wxStaticText* itemStaticText1 = new wxStaticText(itemFrame1, wxID_STATIC, wxT("Фильтр:"), wxDefaultPosition, wxDefaultSize, 0);
	itemGridBagSizer1->Add(itemStaticText1, wxGBPosition(1, 2), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 4);

	filterCtrl = new wxTextCtrl(itemFrame1, ID_TEXTCTRL_filterCtrl, wxEmptyString, wxDefaultPosition, wxSize(300, -1), 0);
	itemGridBagSizer1->Add(filterCtrl, wxGBPosition(1, 3), wxGBSpan(1, 4), wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 4);

	listCtrl = new wxListCtrl(itemFrame1, ID_LISTCTRL, wxDefaultPosition, wxSize(450, 300), wxLC_REPORT);
	itemGridBagSizer1->Add(listCtrl, wxGBPosition(3, 2), wxGBSpan(1, 5), wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 5);

	addBtn = new wxButton(itemFrame1, ID_BUTTON_addBtn, wxT("Добавить запись"), wxDefaultPosition, wxDefaultSize, 0);
	itemGridBagSizer1->Add(addBtn, wxGBPosition(2, 3), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 5);

	deleteBtn = new wxButton(itemFrame1, ID_BUTTON_deleteBtn, wxT("Удалить запись"), wxDefaultPosition, wxDefaultSize, 0);
	itemGridBagSizer1->Add(deleteBtn, wxGBPosition(2, 5), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 5);

	editBtn = new wxButton(itemFrame1, ID_BUTTON_editBtn, wxT("Изменить запись"), wxDefaultPosition, wxDefaultSize, 0);
	itemGridBagSizer1->Add(editBtn, wxGBPosition(2, 4), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 5);

	exitBtn = new wxBitmapButton(itemFrame1, wxID_EXIT, itemFrame1->GetBitmapResource(wxT("quit.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	itemGridBagSizer1->Add(exitBtn, wxGBPosition(4, 5), wxGBSpan(1, 5), wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 5);

	////@end MainForm content construction
}

/*
 * Should we show tooltips?
 */

bool MainForm::ShowToolTips()
{
	return true;
}

/*
 * Get bitmap resources
 */

wxBitmap MainForm::GetBitmapResource(const wxString& name)
{
	// Bitmap retrieval
	////@begin MainForm bitmap retrieval
	wxUnusedVar(name);
	if (name == wxT("quit.xpm"))
	{
		wxBitmap bitmap(quit_xpm);
		return bitmap;
	}
	return wxNullBitmap;
	////@end MainForm bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon MainForm::GetIconResource(const wxString& name)
{
	// Icon retrieval
	////@begin MainForm icon retrieval
	wxUnusedVar(name);
	if (name == wxT("tsnsoft.xpm"))
	{
		wxIcon icon(tsnsoft_xpm);
		return icon;
	}
	return wxNullIcon;
	////@end MainForm icon retrieval
}

// Обработка события загрузки данных в основном потоке
void MainForm::OnLoadData(wxEvent& event)
{
	// Проверяем, что событие является wxThreadEvent
	wxThreadEvent* threadEvent = dynamic_cast<wxThreadEvent*>(&event);
	if (!threadEvent)
	{
		return;
	}

	// Проверяем наличие ошибок в событии
	wxString error = threadEvent->GetString();
	if (!error.IsEmpty())
	{
		wxMessageBox(error, wxT("Ошибка"));
		return;
	}

	// Получаем данные из события
	auto data = threadEvent->GetPayload<wxVector<DistributionData>>();
	listCtrl->Freeze(); // Замораживаем список для повышения производительности
	listCtrl->DeleteAllItems(); // Удаляем все элементы списка

	for (const auto& item : data) // Перебираем все записи
	{
		long index = listCtrl->InsertItem(0, wxString::Format("%d", item.id)); // Вставляем новый элемент в начало списка
		listCtrl->SetItem(index, 1, item.name); // Устанавливаем название дистрибутива
		listCtrl->SetItem(index, 2, item.founder); // Устанавливаем основателя
		listCtrl->SetItem(index, 3, wxString::Format("%d", item.year)); // Устанавливаем год выпуска
	}

	listCtrl->Thaw(); // Размораживаем список
	listCtrl->Refresh(); // Обновляем отображение списка
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_deleteBtn
 */
 // Удаление записи из базы данных
void MainForm::OnBUTTONDeleteBtnClick(wxCommandEvent& event)
{
	// Получаем индекс выделенного элемента
	long selectedIndex = listCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (selectedIndex == -1) // Если ничего не выделено, показываем сообщение об ошибке
	{
		wxMessageBox(wxT("Выберите запись для удаления!"), wxT("Ошибка"));
		return;
	}

	int id = wxAtoi(listCtrl->GetItemText(selectedIndex)); // Получаем ID выделенного элемента

	int response = wxMessageBox(wxT("Вы уверены, что хотите удалить эту запись?"),
		wxT("Подтверждение"), wxYES_NO | wxICON_WARNING);

	if (response == wxYES) // Если пользователь подтвердил удаление
	{
		wxString error;
		// Удаляем запись из базы данных
		if (DeleteDistribution(db, id, error))
		{
			wxMessageBox(wxT("Запись успешно удалена"), wxT("Успех"));
			LoadDataAsync(db, this); // Загружаем данные асинхронно
		}
		else
		{
			wxMessageBox(error, wxT("Ошибка"));
		}
	}
}

/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_addBtn
 */
 // Добавление новой записи в базу данных
void MainForm::OnBUTTONAddBtnClick(wxCommandEvent& event)
{
	wxDialog addDialog(this, wxID_ANY, wxT("Добавить дистрибутив"));
	wxTextCtrl* nameCtrl = new wxTextCtrl(&addDialog, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(400, -1));
	wxTextCtrl* founderCtrl = new wxTextCtrl(&addDialog, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(400, -1));
	wxTextCtrl* yearCtrl = new wxTextCtrl(&addDialog, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(400, -1));

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(new wxStaticText(&addDialog, wxID_ANY, wxT("Название:")), 0, wxALL, 5);
	sizer->Add(nameCtrl, 0, wxALL | wxEXPAND, 5);
	sizer->Add(new wxStaticText(&addDialog, wxID_ANY, wxT("Основатель:")), 0, wxALL, 5);
	sizer->Add(founderCtrl, 0, wxALL | wxEXPAND, 5);
	sizer->Add(new wxStaticText(&addDialog, wxID_ANY, wxT("Год выпуска:")), 0, wxALL, 5);
	sizer->Add(yearCtrl, 0, wxALL | wxEXPAND, 5);
	sizer->Add(new wxButton(&addDialog, wxID_OK, wxT("Сохранить")), 0, wxALL | wxALIGN_CENTER, 5);
	sizer->Add(new wxButton(&addDialog, wxID_CANCEL, wxT("Отмена")), 0, wxALL | wxALIGN_CENTER, 5); // Добавляем кнопку "Отмена"

	addDialog.SetSizerAndFit(sizer);

	// Показываем диалог и проверяем, нажата ли кнопка "Сохранить"
	if (addDialog.ShowModal() != wxID_OK)
	{
		return; // Если не нажата "Сохранить", выходим
	}

	// Получаем данные из полей
	wxString name = nameCtrl->GetValue();
	wxString founder = founderCtrl->GetValue();
	wxString yearStr = yearCtrl->GetValue();

	// Проверяем, заполнены ли все поля
	if (name.IsEmpty() || founder.IsEmpty() || yearStr.IsEmpty())
	{
		wxMessageBox(wxT("Все поля должны быть заполнены!"), wxT("Ошибка"));
		return;
	}

	// Проверяем корректность года
	long year;
	if (!yearStr.ToLong(&year) || year < 1900 || year > wxDateTime::Now().GetYear())
	{
		wxMessageBox(wxT("Год должен быть числом между 1900 и текущим годом!"), wxT("Ошибка"));
		return;
	}

	// Добавляем запись в базу данных
	wxString error;
	if (InsertDistribution(db, name, founder, static_cast<int>(year), error))
	{
		wxMessageBox(wxT("Запись успешно добавлена"), wxT("Успех"));
		LoadDataAsync(db, this); // Загружаем данные асинхронно
	}
	else
	{
		wxMessageBox(error, wxT("Ошибка"));
	}
}

/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_editBtn
 */
 // Редактирование существующей записи в базе данных
void MainForm::OnBUTTONEditBtnClick(wxCommandEvent& event)
{
	// Получаем индекс выделенного элемента
	long selectedIndex = listCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (selectedIndex == -1) // Если ничего не выделено, показываем сообщение об ошибке
	{
		wxMessageBox(wxT("Выберите запись для редактирования!"), wxT("Ошибка"));
		return;
	}

	int id = wxAtoi(listCtrl->GetItemText(selectedIndex)); // Получаем ID выделенного элемента
	wxString name = listCtrl->GetItemText(selectedIndex, 1); // Получаем название дистрибутива
	wxString founder = listCtrl->GetItemText(selectedIndex, 2); // Получаем основателя
	wxString year = listCtrl->GetItemText(selectedIndex, 3); // Получаем год выпуска

	wxDialog editDialog(this, wxID_ANY, wxT("Редактировать дистрибутив"));
	wxTextCtrl* nameCtrl = new wxTextCtrl(&editDialog, wxID_ANY, name, wxDefaultPosition, wxSize(400, -1));
	wxTextCtrl* founderCtrl = new wxTextCtrl(&editDialog, wxID_ANY, founder, wxDefaultPosition, wxSize(400, -1));
	wxTextCtrl* yearCtrl = new wxTextCtrl(&editDialog, wxID_ANY, year, wxDefaultPosition, wxSize(400, -1));

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(new wxStaticText(&editDialog, wxID_ANY, wxT("Название:")), 0, wxALL, 5);
	sizer->Add(nameCtrl, 0, wxALL | wxEXPAND, 5);
	sizer->Add(new wxStaticText(&editDialog, wxID_ANY, wxT("Основатель:")), 0, wxALL, 5);
	sizer->Add(founderCtrl, 0, wxALL | wxEXPAND, 5);
	sizer->Add(new wxStaticText(&editDialog, wxID_ANY, wxT("Год выпуска:")), 0, wxALL, 5);
	sizer->Add(yearCtrl, 0, wxALL | wxEXPAND, 5);
	sizer->Add(new wxButton(&editDialog, wxID_OK, wxT("Сохранить")), 0, wxALL | wxALIGN_CENTER, 5);
	sizer->Add(new wxButton(&editDialog, wxID_CANCEL, wxT("Отмена")), 0, wxALL | wxALIGN_CENTER, 5); // Добавляем кнопку "Отмена"

	editDialog.SetSizerAndFit(sizer); // Устанавливаем sizer для диалога

	// Показываем диалог и проверяем, нажата ли кнопка "Сохранить"
	if (editDialog.ShowModal() != wxID_OK)
	{
		return; // Если не нажата "Сохранить", выходим
	}

	// Получаем данные из полей
	wxString newName = nameCtrl->GetValue(); // Получаем новое название
	wxString newFounder = founderCtrl->GetValue(); // Получаем нового основателя
	wxString newYearStr = yearCtrl->GetValue(); // Получаем новый год выпуска

	// Проверяем, заполнены ли все поля
	if (newName.IsEmpty() || newFounder.IsEmpty() || newYearStr.IsEmpty())
	{
		wxMessageBox(wxT("Все поля должны быть заполнены!"), wxT("Ошибка"));
		return;
	}

	// Проверяем корректность года
	long newYear;
	if (!newYearStr.ToLong(&newYear) || newYear < 1900 || newYear > wxDateTime::Now().GetYear())
	{
		wxMessageBox(wxT("Год должен быть числом между 1900 и текущим годом!"), wxT("Ошибка"));
		return;
	}

	// Обновляем запись в базе данных
	wxString error;
	if (UpdateDistribution(db, id, newName, newFounder, static_cast<int>(newYear), error))
	{
		wxMessageBox(wxT("Запись успешно обновлена"), wxT("Успех"));
		LoadDataAsync(db, this); // Загружаем данные асинхронно
	}
	else
	{
		wxMessageBox(error, wxT("Ошибка"));
	}
}

/*
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_filterCtrl
 */
 // Обработка события изменения текста в поле фильтра
void MainForm::OnTEXTCTRLFilterCtrlTextUpdated(wxCommandEvent& event)
{
	wxString filterText = filterCtrl->GetValue(); // Получаем текст фильтра
	wxVector<DistributionData> data; // Вектор для хранения отфильтрованных данных
	wxString error; // Переменная для хранения ошибок

	// Фильтруем данные по введенному тексту
	FilterData(db, filterText, data, error);

	if (!error.IsEmpty())
	{
		wxMessageBox(error, wxT("Ошибка"));
		return;
	}

	listCtrl->DeleteAllItems(); // Удаляем все элементы списка

	// Заполняем список отфильтрованными данными
	for (const auto& item : data)
	{
		// Вставляем новый элемент в список
		long index = listCtrl->InsertItem(listCtrl->GetItemCount(), wxString::Format(wxT("%d"), item.id)); // Получаем ID дистрибутива
		listCtrl->SetItem(index, 1, item.name); // Устанавливаем название дистрибутива
		listCtrl->SetItem(index, 2, item.founder); // Устанавливаем основателя
		listCtrl->SetItem(index, 3, wxString::Format(wxT("%d"), item.year)); // Устанавливаем год выпуска
	}
}

/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_EXIT
 */
 // Обработка события нажатия кнопки "Выход"
void MainForm::OnExitClick(wxCommandEvent& event)
{
	Close(true);
}


/*
 * wxEVT_COMMAND_LIST_ITEM_ACTIVATED event handler for ID_LISTCTRL
 */
 // Обработка события активации элемента списка
void MainForm::OnListctrlItemActivated(wxListEvent& event)
{
	long selectedIndex = event.GetIndex(); // Получаем индекс выделенного элемента
	if (selectedIndex != -1) // Если элемент не выделен 
	{
		// Снимаем выделение со всех элементов
		listCtrl->SetItemState(selectedIndex, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	}
	wxCommandEvent buttonEvent(wxEVT_BUTTON, ID_BUTTON_editBtn); // Создаем событие нажатия кнопки "Редактировать"
	buttonEvent.SetEventObject(this); // Устанавливаем объект события
	ProcessWindowEvent(buttonEvent); // Обрабатываем событие
}

