#include <iostream>
#include <sqlite3.h>
#include <string>

// Функция для проверки номера телефона в базе
bool checkPhoneNumber(sqlite3 *db, const std::string &phone) {
    std::string sql = "SELECT * FROM students WHERE phone_number = '" + phone + "';";
    sqlite3_stmt *stmt;
    bool found = false;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            found = true;
            std::cout << "\nУспешный вход! Найден студент:" << std::endl;
            std::cout << "----------------------------------------" << std::endl;
            std::cout << "ФИО: " << sqlite3_column_text(stmt, 1) << std::endl;
            std::cout << "Группа: " << sqlite3_column_text(stmt, 2) << std::endl;
            std::cout << "Телефон: " << sqlite3_column_text(stmt, 3) << std::endl;
        }
        sqlite3_finalize(stmt);
    }
    return found;
}

int main() {
    sqlite3 *db;
    
    std::cout << "=========================================" << std::endl;
    std::cout << "        УНИВЕРСИТЕТСКАЯ СИСТЕМА" << std::endl;
    std::cout << "=========================================" << std::endl;

    // Открываем базу данных
    if (sqlite3_open("school.db", &db) != SQLITE_OK) {
        std::cout << "Ошибка открытия базы данных" << std::endl;
        return 1;
    }

    // Экран входа
    std::string phone;
    std::cout << "\nВХОД В СИСТЕМУ" << std::endl;
    std::cout << "Введите ваш номер телефона (+7...): ";
    std::cin >> phone;

    // Проверяем номер в базе
    if (checkPhoneNumber(db, phone)) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ДОСТУП РАЗРЕШЕН!" << std::endl;
        std::cout << "Добро пожаловать в университетскую систему!" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        
        // Университетское расписание
        std::cout << "\nВАШЕ РАСПИСАНИЕ НА НЕДЕЛЮ:" << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "ПОНЕДЕЛЬНИК:" << std::endl;
        std::cout << "  09:00 - 10:30  Высшая математика (ауд. 301)" << std::endl;
        std::cout << "  11:00 - 12:30  Физика (лаб. 205)" << std::endl;
        std::cout << "  13:30 - 15:00  Иностранный язык (ауд. 112)" << std::endl;
        
        std::cout << "\nВТОРНИК:" << std::endl;
        std::cout << "  10:00 - 11:30  Программирование (ауд. 401)" << std::endl;
        std::cout << "  12:00 - 13:30  Базы данных (комп. класс 3)" << std::endl;
        
        std::cout << "\nСРЕДА:" << std::endl;
        std::cout << "  09:30 - 11:00  Алгоритмы и структуры данных (ауд. 305)" << std::endl;
        std::cout << "  11:30 - 13:00  Веб-технологии (комп. класс 2)" << std::endl;
        std::cout << "  14:00 - 15:30  Физкультура (спортзал)" << std::endl;
        
        std::cout << "\nЧЕТВЕРГ:" << std::endl;
        std::cout << "  10:00 - 11:30  Операционные системы (ауд. 410)" << std::endl;
        std::cout << "  12:00 - 13:30  Сетевые технологии (лаб. 208)" << std::endl;
        
        std::cout << "\nПЯТНИЦА:" << std::endl;
        std::cout << "  09:00 - 10:30  Теория вероятностей (ауд. 215)" << std::endl;
        std::cout << "  11:00 - 12:30  Проектная деятельность (ауд. 301)" << std::endl;
        std::cout << "=========================================" << std::endl;
        
    } else {
        std::cout << "\nОШИБКА ВХОДА!" << std::endl;
        std::cout << "Номер телефона не найден в системе." << std::endl;
        std::cout << "Проверьте правильность ввода или обратитесь в деканат." << std::endl;
    }

    sqlite3_close(db);
    std::cout << "\n=========================================" << std::endl;
    return 0;
}
