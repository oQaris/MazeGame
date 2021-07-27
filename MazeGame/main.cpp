#include <iostream>
#include <conio.h>
#include <Windows.h>

using namespace std;

void gotoxy(int xpos, int ypos) {
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = xpos;
    scrn.Y = ypos;
    SetConsoleCursorPosition(hOuput, scrn);
}

void changeColor(WORD color) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, FOREGROUND_INTENSITY | color);
}

int indX = 0, indY = 0;
int col = 0, row = 0;

void printLen(int nStr, short **lab, char iam = '@') {
    gotoxy(0, nStr * 2 + 1);
    for (int j = 0; j < col; j++) {
        if (j != 0)
            cout << " ";

        if (nStr == indY && j == indX)
            cout << iam;
        else {
            if (lab[nStr][j] == 0 || lab[nStr][j] == 1 || lab[nStr][j] == 10 || lab[nStr][j] == 20)
                cout << " ";
            else
                switch (lab[nStr][j]) {
                    case 2:
                        cout << "|";
                        break;
                    case 3:
                        changeColor(FOREGROUND_GREEN);
                        cout << "Г";    // Топорик
                        changeColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
                        break;
                    case 4:
                        changeColor(FOREGROUND_GREEN);
                        cout << "+";    // Аптечка
                        changeColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
                        break;
                    case 5:
                        changeColor(FOREGROUND_GREEN);
                        cout << "%";    // Пружинка
                        changeColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
                        break;
                    case 6:
                        changeColor(FOREGROUND_GREEN);
                        cout << "-";    // Мост
                        changeColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
                        break;
                    case -1:
                        changeColor(FOREGROUND_RED);
                        cout << "о";    // Бомба
                        changeColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
                        break;
                    case -2:
                        changeColor(FOREGROUND_RED);
                        cout << "Ш";    // Динамит
                        changeColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
                        break;
                    default:
                        cout << "?";
                        break;    // Неизвестный символ в массиве
                }
        }
        if (j != col - 1)
            cout << " ";
    }
    for (int i = 0; i < 10; i++)
        cout << '\0';
}

void printBoard(int nStr, short **Lab) {
    gotoxy(0, nStr * 2 + 2);
    for (int i = 1; i < col * 3 - 1; i++) {
        if (Lab[nStr][i / 3] == 0)
            cout << " ";
        else if (Lab[nStr][i / 3] > 9) {
            changeColor(FOREGROUND_BLUE);
            cout << "-";
            changeColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
        } else
            cout << "-";
    }
    for (int i = 0; i < 10; i++)
        cout << '\0';
}

void showStock(short *Stock, short actEl) {
    gotoxy(col * 3 + 10, 1);
    if (actEl == 3)
        changeColor(FOREGROUND_BLUE);
    cout << "1 - Топорик (" << Stock[3] << ")";
    changeColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
    for (int i = 0; i < 10; i++)
        cout << '\0';
    gotoxy(col * 3 + 10, 2);
    if (actEl == 5)
        changeColor(FOREGROUND_BLUE);
    cout << "2 - Пружина (" << Stock[5] << ")";
    changeColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
    for (int i = 0; i < 10; i++)
        cout << '\0';
    gotoxy(col * 3 + 10, 3);
    if (actEl == 6)
        changeColor(FOREGROUND_BLUE);
    cout << "3 - Мост (" << Stock[6] << ")";
    changeColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
    for (int i = 0; i < 10; i++)
        cout << '\0';
    gotoxy(col * 3 + 10, 4);
    cout << "4 - Аптечка (" << Stock[4] << ")";
    for (int i = 0; i < 10; i++)
        cout << '\0';
}

void updateStatus(const char *str) {
    gotoxy(col * 3 + 10, 6);
    cout << str;
    for (int i = 0; i < 64 - strlen(str); i++)
        cout << '\0';
}

void fillArr(short **&arr, int num, const char *file) {
    FILE *f;
    f = fopen(file, "rb");
    if (f == nullptr)
        throw "Отсутствует файл с данными игры!";
    char buf;
    bool flag = false;
    while (!feof(f) && !flag) {
        buf = fgetc(f);
        if (/*если цифра*/ buf > 48 && buf < 58 && /*сравниваем с кодом*/ buf - 48 == num &&
                           /*если следущее двоеточие*/ fgetc(f) == ':')
            flag = true;
    }
    if (!flag)
        throw "Не найден уровень!";

    if (fscanf(f, "%d\n%d", &row, &col) != 2)
        throw "Ошибка с получением размеров лабиринта!";
    if (col <= 0 || col >= 2147483647 || row <= 0 || row >= 2147483647) {
        col = 0;
        row = 0;
        throw "Не корректные размеры лабиринта!";
    } else {
        int i, j, kol = 0;
        // Выделение памяти для динамического массива
        arr = new short *[row];
        for (i = 0; i < row; i++)
            arr[i] = new short[col];
        if (arr == nullptr)
            throw "Не хватает памяти для создания лабиринта!";
        else {
            bool error = false;
            // Получаем данные из файла
            for (i = 0; i < row; i++)
                for (j = 0; j < col; j++)
                    if (!feof(f) && !error) {
                        if (fscanf(f, "%d", &arr[i][j]) != 1 || arr[i][j] <= -32000 || arr[i][j] >= 32000)
                            error = true;
                        else
                            kol++;
                    }
            if (error)
                throw "В файле присутствуют некорректные данные!";
            else if (kol < row * col)
                throw "Количество элементов лабиринта меньше указанного!";
        }
    }
    if (fclose(f) < 0)
        throw "Ошибка при закрытии файла!";
}

int countLvl(const char *file) {
    FILE *f;
    f = fopen(file, "rb");
    if (f == nullptr)
        throw "Отсутствует файл с данными игры!";
    int count = 0;
    while (!feof(f)) {
        if (fgetc(f) == ':')
            count++;
    }
    if (fclose(f) < 0)
        throw "Ошибка при закрытии файла!";
    return count;
}


int main() {
    setlocale(LC_ALL, "ru");
    //system("chcp 65001");
    changeColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    int ch = 0;
    bool dir = true;
    bool waitDir = true;
    bool drop = false;
    bool life = true;
    int prevX = 0;
    int prevY = 0;
    short actEl = 0;
    int lvl = 1;
    int max_lvl = 0;
    short stock[11];
    short **lab = nullptr;

    try {
        max_lvl = countLvl("lab.bin");
    }
    catch (const char *ex) {
        system("cls");
        cout << ex << endl;
        _getch();
        return 0;
    }

    while (lvl <= max_lvl) {
        try {
            fillArr(lab, lvl, "lab.bin");
        }
        catch (const char *ex) {
            system("cls");
            cout << ex << endl;
            _getch();
            return 0;
        }
        // Обнуление инвентаря
        for (short &i : stock)
            i = 0;
        // Пременные по-умолчанию
        ch = 0;
        dir = true;
        waitDir = true;
        drop = false;
        life = true;
        indX = 0;
        indY = 0;
        prevX = 0;
        prevY = 0;
        actEl = 0;

        // Строим потолок
        for (int i = 0; i < col * 3 - 2; i++)
            cout << "-";
        for (int i = 0; i < 10; i++)
            cout << '\0';
        cout << endl;

        for (int y = 0; y < row; y++) {
            // Выводим лабиринт построчно
            printLen(y, lab);
            cout << endl;
            printBoard(y, lab);
            cout << endl;
        }
        showStock(stock, actEl);
        updateStatus("");
        gotoxy(col * 3 - 2, row * 2);

        while (ch != 27) // Esc = Restart
        {
            ch = _getch();
            // Если нажаты стрелочки и мы живы
            if (life && (ch == 0 || ch == 224 || ch == -32)) {
                ch = _getch();
                switch (ch) {
                    case 72: //Вверх
                        // Если не на первом этаже и есть пружинка и сверху дыра
                        if (indY != 0 && stock[5] > 0 && actEl == 5 && lab[indY - 1][indX] == 0)
                            indY--;
                        break;

                    case 80: //Вниз
                        // Если не на последнем этаже и есть топорик и под нами пол и снизу не вертикальная стенка
                        if (indY != row - 1 && stock[3] > 0 && actEl == 3 && lab[indY][indX] == 1 &&
                            lab[indY + 1][indX] != 2)
                            indY++;
                        break;

                    case 75: //Влево
                        // Если соответствующее направление и не на краю и нет стенки или если есть стенка, но есть активный топор
                        if (((waitDir || !dir) && indX != 0 && lab[indY][indX - 1] != 2) ||
                            (lab[indY][indX - 1] == 2 && stock[3] > 0 && actEl == 3))
                            indX--;
                        break;

                    case 77: //Вправо
                        if (((waitDir || dir) && indX != col - 1 && lab[indY][indX + 1] != 2) ||
                            (lab[indY][indX + 1] == 2 && stock[3] > 0 && actEl == 3))
                            indX++;
                        break;
                }

                // При движении по этажу
                if (prevX != indX) {
                    // Меняем направление, если впервые на этаже
                    if (waitDir) {
                        dir = ch != 75;
                        waitDir = false;
                    }
                    // Если использовали топорик (зашли на вертикальную стенку с активным топором), то отнимаем его и убираем стенку
                    if (lab[indY][indX] == 2 && stock[3] > 0 && actEl == 3) {
                        stock[3]--;
                        lab[indY][indX] = 1;
                        showStock(stock, actEl);
                    }
                    // Если используем Мост (стоим на дыре и активный Мост), то отнимаем его и делаем пол (не забывая обновлять границу)
                    if (lab[indY][indX] == 0 && stock[6] > 0 && actEl == 6) {
                        stock[6]--;
                        lab[indY][indX] = 1;
                        showStock(stock, actEl);
                        printBoard(indY, lab);
                    } else if (lab[indY][indX] == 0) // Падение
                        drop = true;
                }
                    // При вертикальном движении
                else if (prevY != indY) {
                    // Если использовали топорик (спустились вниз и сверху пол + активный топор), то отнимаем его и делаем дыру в потолке
                    if (indY > prevY && lab[indY - 1][indX] == 1 && stock[3] > 0 && actEl == 3) {
                        stock[3]--;
                        lab[indY - 1][indX] = 0;
                        showStock(stock, actEl);
                        printBoard(indY - 1, lab);
                    }
                    // Если использовали пружинку (поднялись вверх и стоим на дыре), то отнимаем её
                    if (indY < prevY && lab[indY][indX] == 0 && stock[5] > 0 && actEl == 5) {
                        stock[5]--;
                        showStock(stock, actEl);
                        printLen(prevY, lab);
                    } else if (lab[indY][indX] == 0) // Падение
                        drop = true;
                    // Обновляем предыдущий этаж
                    printLen(prevY, lab);
                    // Даём команду считать направлние
                    waitDir = true;
                }

                // Падение
                if (drop) {
                    while (lab[indY][indX] == 0)
                        indY++;
                    if (indY - prevY == 2) {
                        stock[4] -= 3;
                        showStock(stock, actEl);
                        if (stock[4] < 0) {
                            updateStatus("Вы разбились насмерть!");
                            life = false;
                        } else
                            updateStatus("Падение отняло 3 здоровья!");
                    }
                    if (indY - prevY > 2) {
                        stock[4] = 0;
                        showStock(stock, actEl);
                        updateStatus("Вы разбились насмерть, упав с большой высоты!");
                        life = false;
                    }
                    printLen(prevY, lab);
                    waitDir = true;
                    drop = false;
                }

                // Кладём предметы в инвентарь
                if (lab[indY][indX] > 2 && lab[indY][indX] < 10) {
                    stock[lab[indY][indX]]++;
                    char str[64];
                    switch (lab[indY][indX]) {
                        case 3:
                            sprintf(str, "Вы собрали Топорик");
                            break;
                        case 4:
                            sprintf(str, "Вы собрали Аптечку");
                            break;
                        case 5:
                            sprintf(str, "Вы собрали Пружину");
                            break;
                        case 6:
                            sprintf(str, "Вы собрали Мост");
                            break;
                        default:
                            sprintf(str, "Вы собрали нечто неизвестное!");
                            break;
                    }
                    lab[indY][indX] = 1;
                    // Выводим инвентарь
                    showStock(stock, actEl);
                    updateStatus(str);
                }
                // Применяем аптечку если надо
                if (lab[indY][indX] == -1) {
                    stock[4]--;
                    lab[indY][indX] = 1;
                    showStock(stock, actEl);
                    if (stock[4] < 0) {
                        updateStatus("Вас убила бомба!");
                        life = false;
                    } else
                        updateStatus("Бомба нанесла вам урон!");
                }
                // Опустошаем здоровье если динамит
                if (lab[indY][indX] == -2) {
                    stock[4] = 0;
                    lab[indY][indX] = 1;
                    showStock(stock, actEl);
                    updateStatus("Вы взорвались на динамите!");
                    life = false;
                }

                // Обновляем этаж
                if (life)
                    printLen(indY, lab);
                else
                    printLen(indY, lab, '#');
                // Если дошли до финиша
                if (lab[indY][indX] == 20) {
                    updateStatus("Победа! Для перехода на новый уровень нажмите Enter...");
                    gotoxy(col * 3 - 2, row * 2);
                    lvl++;
                    do { ch = _getch(); } while (ch != 13);
                    ch = 27;
                }
                // Место указателя на финише
                gotoxy(col * 3 - 2, row * 2);
                prevY = indY;
                prevX = indX;
            } else {
                switch (ch) {
                    case '1':
                        actEl = 3;
                        break;    // Топорик
                    case '2':
                        actEl = 5;
                        break;    // Пружинка
                    case '3':
                        actEl = 6;
                        break;    // Мост
                    default:
                        actEl = 0;
                        break;
                }
                // Выводим инвентарь
                showStock(stock, actEl);
                gotoxy(col * 3 - 2, row * 2);
            }
        }
        gotoxy(0, 0);
        delete[] lab;
    }
    updateStatus("Молодец! Ты прошёл игру");
    gotoxy(col * 3 - 2, row * 2);
    _getch();
    return 0;
}