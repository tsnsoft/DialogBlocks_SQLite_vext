// distro_db.h
#ifndef _DISTRO_DB_H_
#define _DISTRO_DB_H_

#include "wx/wx.h" // Подключение wxWidgets
#include <sqlite3.h> // Подключение SQLite3

// Структура для хранения данных о дистрибутивах Linux
struct DistributionData {
	int id; // Идентификатор
	wxString name; // Название дистрибутива
	wxString founder; // Основатель
	int year; // Год выпуска

	// Конструктор для инициализации структуры
	DistributionData(int i, const wxString& n, const wxString& f, int y)
		: id(i), name(n), founder(f), year(y) {
	}
};

// Объявление пользовательского события
wxDECLARE_EVENT(EVT_LOAD_DATA, wxThreadEvent);

// Инициализация базы данных
bool InitDatabase(sqlite3*& db);

// Асинхронная загрузка данных
void LoadDataAsync(sqlite3* db, wxEvtHandler* eventHandler);

// Добавление новой записи в базу данных
bool InsertDistribution(sqlite3* db, const wxString& name, const wxString& founder, int year, wxString& error);

// Удаление записи из базы данных
bool DeleteDistribution(sqlite3* db, int id, wxString& error);

// Обновление записи в базе данных
bool UpdateDistribution(sqlite3* db, int id, const wxString& name, const wxString& founder, int year, wxString& error);

// Фильтрация данных по заданному тексту
void FilterData(sqlite3* db, const wxString& filterText, wxVector<DistributionData>& data, wxString& error);

#endif // _DB_H_