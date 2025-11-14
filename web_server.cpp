#include <crow.h>
#include <sqlite3.h>
#include <string>
#include <sstream>

// Функция для проверки номера телефона
std::string checkPhoneNumber(const std::string &phone) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    std::string result = "";

    if (sqlite3_open("school.db", &db) == SQLITE_OK) {
        std::string sql = "SELECT * FROM students WHERE phone_number = ?;";
        
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, phone.c_str(), -1, SQLITE_STATIC);
            
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                std::stringstream ss;
                ss << "Успешный вход!<br>";
                ss << "ФИО: " << sqlite3_column_text(stmt, 1) << "<br>";
                ss << "Группа: " << sqlite3_column_text(stmt, 2) << "<br>";
                ss << "Телефон: " << sqlite3_column_text(stmt, 3);
                result = ss.str();
            } else {
                result = "ОШИБКА: Номер телефона не найден в системе";
            }
            sqlite3_finalize(stmt);
        }
        sqlite3_close(db);
    }
    return result;
}

int main() {
    crow::SimpleApp app;

    // Главная страница с формой входа
    CROW_ROUTE(app, "/")([](){
        std::string html = R"(
        <!DOCTYPE html>
        <html>
        <head>
            <title>Университетская система</title>
            <meta charset="utf-8">
            <style>
                body { 
                    font-family: Arial, sans-serif; 
                    background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
                    margin: 0; padding: 0; height: 100vh;
                    display: flex; justify-content: center; align-items: center;
                }
                .login-container {
                    background: white; padding: 40px; border-radius: 10px;
                    box-shadow: 0 10px 30px rgba(0,0,0,0.3); width: 400px;
                }
                h1 { 
                    text-align: center; color: #333; margin-bottom: 30px;
                    border-bottom: 2px solid #667eea; padding-bottom: 10px;
                }
                .form-group { margin-bottom: 20px; }
                label { display: block; margin-bottom: 5px; color: #555; font-weight: bold; }
                input[type="text"] {
                    width: 100%; padding: 12px; border: 2px solid #ddd;
                    border-radius: 5px; font-size: 16px; box-sizing: border-box;
                }
                input[type="text"]:focus {
                    border-color: #667eea; outline: none;
                }
                button {
                    width: 100%; padding: 12px; background: #667eea;
                    color: white; border: none; border-radius: 5px;
                    font-size: 16px; cursor: pointer; font-weight: bold;
                }
                button:hover { background: #5a6fd8; }
                .result {
                    margin-top: 20px; padding: 15px; border-radius: 5px;
                    text-align: center; font-weight: bold;
                }
                .success { background: #d4edda; color: #155724; border: 1px solid #c3e6cb; }
                .error { background: #f8d7da; color: #721c24; border: 1px solid #f5c6cb; }
            </style>
        </head>
        <body>
            <div class="login-container">
                <h1>🎓 Университетская система</h1>
                <form method="post" action="/login">
                    <div class="form-group">
                        <label for="phone">Номер телефона:</label>
                        <input type="text" id="phone" name="phone" 
                               placeholder="+79161234567" required 
                               pattern="\+7[0-9]{10}">
                    </div>
                    <button type="submit">Войти в систему</button>
                </form>
                <div style="margin-top: 20px; text-align: center; color: #666;">
                    <strong>Тестовые номера:</strong><br>
                    +79161234567, +79031112233, +79265554466
                </div>
            </div>
        </body>
        </html>
        )";
        return html;
    });

    // Обработка формы входа
    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        auto phone = req.url_params.get("phone");
        
        if (!phone) {
            return crow::response(400, "Номер телефона не указан");
        }

        std::string result = checkPhoneNumber(std::string(phone));
        
        std::string html = R"(
        <!DOCTYPE html>
        <html>
        <head>
            <title>Результат входа</title>
            <meta charset="utf-8">
            <style>
                body { 
                    font-family: Arial, sans-serif; 
                    background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
                    margin: 0; padding: 0; height: 100vh;
                    display: flex; justify-content: center; align-items: center;
                }
                .result-container {
                    background: white; padding: 40px; border-radius: 10px;
                    box-shadow: 0 10px 30px rgba(0,0,0,0.3); width: 500px;
                    text-align: center;
                }
                h1 { color: #333; margin-bottom: 20px; }
                .success { color: #155724; background: #d4edda; padding: 20px; border-radius: 5px; }
                .error { color: #721c24; background: #f8d7da; padding: 20px; border-radius: 5px; }
                .schedule { 
                    margin-top: 20px; text-align: left; background: #f8f9fa; 
                    padding: 15px; border-radius: 5px; border-left: 4px solid #667eea;
                }
                .back-btn {
                    display: inline-block; margin-top: 20px; padding: 10px 20px;
                    background: #667eea; color: white; text-decoration: none;
                    border-radius: 5px; font-weight: bold;
                }
                .back-btn:hover { background: #5a6fd8; }
            </style>
        </head>
        <body>
            <div class="result-container">
        )";

        if (result.find("ОШИБКА") == std::string::npos) {
            // Успешный вход
            html += "<h1>✅ ДОСТУП РАЗРЕШЕН</h1>";
            html += "<div class='success'>" + result + "</div>";
            
            // Добавляем расписание
            html += R"(
                <div class="schedule">
                    <h3>📅 Ваше расписание на неделю:</h3>
                    <strong>Понедельник:</strong> Высшая математика 09:00-10:30 (ауд. 301)<br>
                    <strong>Вторник:</strong> Программирование 10:00-11:30 (ауд. 401)<br>
                    <strong>Среда:</strong> Алгоритмы и структуры данных 09:30-11:00 (ауд. 305)<br>
                    <strong>Четверг:</strong> Операционные системы 10:00-11:30 (ауд. 410)<br>
                    <strong>Пятница:</strong> Теория вероятностей 09:00-10:30 (ауд. 215)
                </div>
            )";
        } else {
            // Ошибка входа
            html += "<h1>❌ ОШИБКА ВХОДА</h1>";
            html += "<div class='error'>" + result + "</div>";
        }

        html += R"(
                <a href="/" class="back-btn">Вернуться на главную</a>
            </div>
        </body>
        </html>
        )";

        return crow::response(html);
    });

    std::cout << "🚀 Веб-сервер запущен на http://localhost:8080" << std::endl;
    std::cout << "📱 Открой браузер и перейди по адресу выше" << std::endl;
    
    app.port(8080).multithreaded().run();
}
