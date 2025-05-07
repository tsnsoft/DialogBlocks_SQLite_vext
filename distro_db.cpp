// distro_db.cpp
#include "distro_db.h" // Подключение функций базы данных
#include <thread> // Для работы с потоками

// Инициализация базы данных
bool InitDatabase(sqlite3*& db) {
	// Проверяем, если база данных уже инициализирована
	if (sqlite3_open("distributions.db", &db) != SQLITE_OK) {
		db = nullptr;
		return false;
	}

	// Создаем таблицу, если она не существует
	const char* createTableSQL = "CREATE TABLE IF NOT EXISTS distributions(id INTEGER PRIMARY KEY, name TEXT, founder TEXT, year INTEGER);";
	if (sqlite3_exec(db, createTableSQL, nullptr, nullptr, nullptr) != SQLITE_OK) {
		sqlite3_close(db);
		db = nullptr;
		return false;
	}

	// Проверяем, есть ли уже данные в таблице
	sqlite3_stmt* stmt;
	if (sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM distributions;", -1, &stmt, nullptr) != SQLITE_OK) {
		sqlite3_close(db);
		db = nullptr;
		return false;
	}

	// Получаем количество записей в таблице
	int count = 0;
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		count = sqlite3_column_int(stmt, 0);
	}
	sqlite3_finalize(stmt);

	// Если таблица пуста, добавляем начальные данные
	if (count == 0) {
		const char* insertSQL =
			"INSERT INTO distributions(name, founder, year) VALUES "
			"('Ubuntu', 'Mark Shuttleworth', 2004),"
			"('Debian', 'Ian Murdock', 1993),"
			"('Linux Mint', 'Clement Lefebvre', 2006),"
			"('Fedora', 'Warren Togami', 2003),"
			"('Arch Linux', 'Judd Vinet', 2002),"
			"('Red Hat Linux', 'Marc Ewing', 1993),"
			"('Red Hat Enterprise Linux', 'Marc Ewing', 2002),"
			"('SUSE Linux', 'Roland Dyroff', 1994),"
			"('Manjaro', 'Philip Müller', 2011),"
			"('elementary OS', 'Daniel Foré', 2011),"
			"('Kali Linux', 'Mati Aharoni', 2013),"
			"('Gentoo', 'Daniel Robbins', 2000),"
			"('Slackware', 'Patrick Volkerding', 1993);";

		// Выполняем SQL-запрос для вставки данных
		if (sqlite3_exec(db, insertSQL, nullptr, nullptr, nullptr) != SQLITE_OK) {
			sqlite3_close(db);
			db = nullptr;
			return false;
		}
	}

	return true;
}

// Асинхронная загрузка данных
void LoadDataAsync(sqlite3* db, wxEvtHandler* eventHandler) {
	// Проверяем, инициализирована ли база данных
	if (!db) {
		wxThreadEvent event(EVT_LOAD_DATA);
		event.SetString("База данных не инициализирована.");
		wxQueueEvent(eventHandler, event.Clone());
		return;
	}

	// Запускаем новый поток для загрузки данных
	std::thread([db, eventHandler]() {
		wxThreadEvent event(EVT_LOAD_DATA);
		wxVector<DistributionData> data;

		sqlite3_stmt* stmt;
		const char* query = "SELECT id, name, founder, year FROM distributions ORDER by id desc";

		// Подготавливаем SQL-запрос
		if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
			event.SetString(wxString::Format("Ошибка SQL: %s", sqlite3_errmsg(db)));
			wxQueueEvent(eventHandler, event.Clone());
			return;
		}

		// Выполняем SQL-запрос и извлекаем данные
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			int id = sqlite3_column_int(stmt, 0);
			const unsigned char* name = sqlite3_column_text(stmt, 1);
			const unsigned char* founder = sqlite3_column_text(stmt, 2);
			int year = sqlite3_column_int(stmt, 3);

			data.push_back(DistributionData(
				id,
				name ? wxString::FromUTF8((const char*)name) : "",
				founder ? wxString::FromUTF8((const char*)founder) : "",
				year
			));
		}

		sqlite3_finalize(stmt); // Освобождаем подготовленный запрос
		event.SetPayload<wxVector<DistributionData>>(data); // Устанавливаем данные в событие
		wxQueueEvent(eventHandler, event.Clone()); // Отправляем событие в главный поток
		}).detach(); // Отделяем поток, чтобы он работал независимо
}

// Добавление новой записи в базу данных
bool InsertDistribution(sqlite3* db, const wxString& name, const wxString& founder, int year, wxString& error) {
	// Задаем SQL-запрос для вставки новой записи
	const char* sql = "INSERT INTO distributions(name, founder, year) VALUES(?, ?, ?)";
	sqlite3_stmt* stmt;

	// Подготавливаем SQL-запрос
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		error = wxString::Format("Ошибка подготовки SQL: %s", sqlite3_errmsg(db));
		return false;
	}

	// Привязываем параметры к запросу
	if (sqlite3_bind_text(stmt, 1, name.ToUTF8(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
		sqlite3_bind_text(stmt, 2, founder.ToUTF8(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
		sqlite3_bind_int(stmt, 3, year) != SQLITE_OK) {
		error = wxString::Format("Ошибка привязки SQL: %s", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return false;
	}

	// Выполняем SQL-запрос
	if (sqlite3_step(stmt) != SQLITE_DONE) {
		error = wxString::Format("Ошибка выполнения SQL: %s", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return false;
	}

	sqlite3_finalize(stmt); // Освобождаем подготовленный запрос
	return true;
}

// Удаление записи из базы данных
bool DeleteDistribution(sqlite3* db, int id, wxString& error) {
	// Задаем SQL-запрос для удаления записи
	const char* sql = "DELETE FROM distributions WHERE id = ?";
	sqlite3_stmt* stmt;

	// Подготавливаем SQL-запрос
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		error = wxString::Format("Ошибка подготовки SQL: %s", sqlite3_errmsg(db));
		return false;
	}

	// Привязываем параметры к запросу
	if (sqlite3_bind_int(stmt, 1, id) != SQLITE_OK) {
		error = wxString::Format("Ошибка привязки SQL: %s", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return false;
	}

	// Выполняем SQL-запрос
	if (sqlite3_step(stmt) != SQLITE_DONE) {
		error = wxString::Format("Ошибка выполнения SQL: %s", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return false;
	}

	sqlite3_finalize(stmt);
	return true;
}

// Обновление записи в базе данных
bool UpdateDistribution(sqlite3* db, int id, const wxString& name, const wxString& founder, int year, wxString& error) {
	// Задаем SQL-запрос для обновления записи
	const char* sql = "UPDATE distributions SET name = ?, founder = ?, year = ? WHERE id = ?";
	sqlite3_stmt* stmt;

	// Подготавливаем SQL-запрос
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		error = wxString::Format("Ошибка подготовки SQL: %s", sqlite3_errmsg(db));
		return false;
	}

	// Привязываем параметры к запросу
	if (sqlite3_bind_text(stmt, 1, name.ToUTF8(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
		sqlite3_bind_text(stmt, 2, founder.ToUTF8(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
		sqlite3_bind_int(stmt, 3, year) != SQLITE_OK ||
		sqlite3_bind_int(stmt, 4, id) != SQLITE_OK) {
		error = wxString::Format("Ошибка привязки SQL: %s", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return false;
	}

	// Выполняем SQL-запрос
	if (sqlite3_step(stmt) != SQLITE_DONE) {
		error = wxString::Format("Ошибка выполнения SQL: %s", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return false;
	}

	sqlite3_finalize(stmt); // Освобождаем подготовленный запрос
	return true;
}

// Фильтрация данных по заданному тексту
void FilterData(sqlite3* db, const wxString& filterText, wxVector<DistributionData>& data, wxString& error) {
	data.clear(); // Очищаем вектор данных
	sqlite3_stmt* stmt; // Подготовленный запрос
	
	// Задаем SQL-запрос для фильтрации данных
	wxString query = wxT("SELECT id, name, founder, year FROM distributions WHERE ");
	query += wxT("LOWER(name) LIKE ? OR ");
	query += wxT("LOWER(founder) LIKE ? OR ");
	query += wxT("CAST(year AS TEXT) LIKE ?;");

	// Подготавливаем SQL-запрос
	if (sqlite3_prepare_v2(db, query.mb_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		error = wxString::Format("Ошибка подготовки SQL: %s", sqlite3_errmsg(db));
		return;
	}

	// Привязываем параметры к запросу
	wxString wildcardFilter = wxT("%") + filterText.Lower() + wxT("%");
	if (sqlite3_bind_text(stmt, 1, wildcardFilter.ToUTF8(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
		sqlite3_bind_text(stmt, 2, wildcardFilter.ToUTF8(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
		sqlite3_bind_text(stmt, 3, wildcardFilter.ToUTF8(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
		error = wxString::Format("Ошибка привязки SQL: %s", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return;
	}

	// Выполняем SQL-запрос и извлекаем данные
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		const unsigned char* name = sqlite3_column_text(stmt, 1);
		const unsigned char* founder = sqlite3_column_text(stmt, 2);
		data.push_back(DistributionData(
			sqlite3_column_int(stmt, 0),
			name ? wxString::FromUTF8(reinterpret_cast<const char*>(name)) : wxT(""),
			founder ? wxString::FromUTF8(reinterpret_cast<const char*>(founder)) : wxT(""),
			sqlite3_column_int(stmt, 3)
		));
	}

	sqlite3_finalize(stmt); // Освобождаем подготовленный запрос
}