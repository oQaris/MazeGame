#include <iostream>
#include <conio.h>
#include <Windows.h>
using namespace std;

void gotoxy(int xpos, int ypos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos; scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput, scrn);
}

void changeColor(WORD color)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, FOREGROUND_INTENSITY | color);
}

int indX = 0, indY = 0;
int Col = 0, Row = 0;

void PrintLen(int nStr, short** Lab, char iam = '@')
{
	gotoxy(0, nStr * 2 + 1);
	for (int j = 0; j < Col; j++)
	{
		if (j != 0)
			cout << ' ';

		if (nStr == indY && j == indX)
			cout << iam;
		else
		{
			if (Lab[nStr][j] == 0 || Lab[nStr][j] == 1 || Lab[nStr][j] == 10 || Lab[nStr][j] == 20)
				cout << ' ';
			//else if (Lab[nStr][j] > 10 && Lab[nStr][j] < 20)
			//{
			//	changeColor(FOREGROUND_BLUE);
			//	cout << Lab[nStr][j] - 10;	// Кнопка
			//	changeColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			//}
			else
				switch (Lab[nStr][j])
				{
				case 2: cout << '|'; break;
				case 3:
					changeColor(FOREGROUND_GREEN);
					cout << 'Г';	// Топорик
					changeColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
					break;
				case 4:
					changeColor(FOREGROUND_GREEN);
					cout << '+';	// Аптечка
					changeColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
					break;
				case 5:
					changeColor(FOREGROUND_GREEN);
					cout << '%';	// Пружинка
					changeColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
					break;
				case 6:
					changeColor(FOREGROUND_GREEN);
					cout << '-';	// Мост
					changeColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
					break;
				case -1:
					changeColor(FOREGROUND_RED);
					cout << 'о';	// Бомба
					changeColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
					break;
				case -2:
					changeColor(FOREGROUND_RED);
					cout << 'Ш';	// Динамит
					changeColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
					break;
				default: cout << '?'; break;	// Неизвестный символ в массиве
				}
		}
		if (j != Col - 1)
			cout << ' ';
	}
	for (int i = 0; i < 10; i++)
		cout << '\0';
}

void PrintBoard(int nStr, short** Lab)
{
	gotoxy(0, nStr * 2 + 2);
	for (int i = 1; i < Col * 3 - 1; i++)
	{
		if (Lab[nStr][i / 3] == 0)
			cout << ' ';
		else if (Lab[nStr][i / 3] > 9)
		{
			changeColor(FOREGROUND_BLUE);
			cout << '-';
			changeColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
		}
		else
			cout << '-';
	}
	for (int i = 0; i < 10; i++)
		cout << '\0';
}

void ShowStock(short* Stock, short actEl)
{
	gotoxy(Col * 3 + 10, 1);
	if (actEl == 3)
		changeColor(FOREGROUND_BLUE);
	cout << "1 - Топорик (" << Stock[3] << ")";
	changeColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	for (int i = 0; i < 10; i++)
		cout << '\0';
	gotoxy(Col * 3 + 10, 2);
	if (actEl == 5)
		changeColor(FOREGROUND_BLUE);
	cout << "2 - Пружина (" << Stock[5] << ")";
	changeColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	for (int i = 0; i < 10; i++)
		cout << '\0';
	gotoxy(Col * 3 + 10, 3);
	if (actEl == 6)
		changeColor(FOREGROUND_BLUE);
	cout << "3 - Мост (" << Stock[6] << ")";
	changeColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	for (int i = 0; i < 10; i++)
		cout << '\0';
	gotoxy(Col * 3 + 10, 4);
	cout << "4 - Аптечка (" << Stock[4] << ")";
	for (int i = 0; i < 10; i++)
		cout << '\0';
}

void UpdateStatus(const char* str)
{
	gotoxy(Col * 3 + 10, 6);
	cout << str;
	for (int i = 0; i < 64 - strlen(str); i++)
		cout << '\0';
}

void FillArr(short** &arr, int num, const char* file)
{
	FILE* f;
	f = fopen(file, "rb");
	if (f == NULL)
		throw "Отсутствует файл с данными игры!";
	char buf = NULL;
	bool flag = false;
	while (!feof(f) && !flag)
	{
		buf = fgetc(f);
		if (/*если цифра*/ buf > 48 && buf < 58 && /*сравниваем с кодом*/ buf - 48 == num && /*если следущее двоеточие*/ fgetc(f) == ':')
			flag = true;
	}
	if (!flag)
		throw "Не найден уровень!";

	if (fscanf(f, "%d\n%d", &Row, &Col) != 2)
		throw "Ошибка с получением размеров лабиринта!";
	if (Col <= 0 || Col >= 2147483647 || Row <= 0 || Row >= 2147483647)
	{
		Col = 0;
		Row = 0;
		throw "Не корректные размеры лабиринта!";
	}
	else
	{
		int i, j, kol = 0;
		// Выделение памяти для динамического массива
		arr = new short*[Row];
		for (i = 0; i < Row; i++)
			arr[i] = new short[Col];
		if (arr == NULL)
			throw "Не хватает памяти для создания лабиринта!";
		else
		{
			bool error = false;
			// Получаем данные из файла
			for (i = 0; i < Row; i++)
				for (j = 0; j < Col; j++)
					if (!feof(f) && !error)
					{
						if (fscanf(f, "%d", &arr[i][j]) != 1 || arr[i][j] <= -32000 || arr[i][j] >= 32000)
							error = true;
						else
							kol++;
					}
			if (error)
				throw "В файле присутствуют некорректные данные!";
			else if (kol < Row * Col)
				throw "Количество элементов лабиринта меньше указанного!";
		}
	}
	if (fclose(f) < 0)
		throw "Ошибка при закрытии файла!";
}

int CountLvl(const char* file)
{
	FILE* f;
	f = fopen(file, "rb");
	if (f == NULL)
		throw "Отсутствует файл с данными игры!";
	int count = 0;
	while (!feof(f))
	{
		if (fgetc(f) == ':')
			count++;
	}
	if (fclose(f) < 0)
		throw "Ошибка при закрытии файла!";
	return count;
}


int main()
{
	setlocale(0, "");
	changeColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

	char ch = 0;
	bool dir = true;
	bool waitDir = true;
	bool drop = false;
	bool life = true;
	int prewX = 0;
	int prewY = 0;
	short actEl = 0;
	int lvl = 1;
	int max_lvl = 0;
	short Stock[11];
	short** Lab = NULL;

	try
	{
		max_lvl = CountLvl("Lab.bin");
	}
	catch (const char* ex)
	{
		system("cls");
		cout << ex << endl;
		_getch();
		return 0;
	}

Start:
	try
	{
		FillArr(Lab, lvl, "Lab.bin");
	}
	catch (const char* ex)
	{
		system("cls");
		cout << ex << endl;
		_getch();
		return 0;
	}
	// Обнуление инвентаря
	for (int i = 0; i < 11; i++)
		Stock[i] = 0;
	// Пременные по-умолчанию
	ch = 0;
	dir = true;
	waitDir = true;
	drop = false;
	life = true;
	indX = 0;
	indY = 0;
	prewX = 0;
	prewY = 0;
	actEl = 0;

	// Строим потолок
	for (int i = 0; i < Col * 3 - 2; i++)
		cout << '-';
	for (int i = 0; i < 10; i++)
		cout << '\0';
	cout << endl;

	for (int y = 0; y < Row; y++)
	{
		// Выводим лабиринт построчно
		PrintLen(y, Lab);
		cout << endl;
		PrintBoard(y, Lab);
		cout << endl;
	}
	ShowStock(Stock, actEl);
	UpdateStatus("");
	gotoxy(Col * 3 - 2, Row * 2);

	while (ch != 27) // Esc = Restart
	{
		ch = _getch();
		// Если нажаты стрелочки и мы живы
		if (life && (ch == 0 || ch == 224 || ch == -32))
		{
			ch = _getch();
			switch (ch)
			{
			case 72: //Вверх
				// Если не на первом этаже и есть пружинка и сверху дыра
				if (indY != 0 && Stock[5] > 0 && actEl == 5 && Lab[indY - 1][indX] == 0)
					indY--;
				break;

			case 80: //Вниз
				// Если не на последнем этаже и есть топорик и под нами пол и снизу не вертикальная стенка
				if (indY != Row - 1 && Stock[3] > 0 && actEl == 3 && Lab[indY][indX] == 1 && Lab[indY + 1][indX] != 2)
					indY++;
				break;

			case 75: //Влево
				// Если соответствующее направление и не на краю и нет стенки или если есть стенка, но есть активный топор
				if (((waitDir ? true : !dir) && indX != 0 && Lab[indY][indX - 1] != 2) || (Lab[indY][indX - 1] == 2 && Stock[3] > 0 && actEl == 3))
					indX--;
				break;

			case 77: //Вправо
				if (((waitDir ? true : dir) && indX != Col - 1 && Lab[indY][indX + 1] != 2) || (Lab[indY][indX + 1] == 2 && Stock[3] > 0 && actEl == 3))
					indX++;
				break;
			}

			// При движении по этажу
			if (prewX != indX)
			{
				// Меняем направление, если впервые на этаже
				if (waitDir)
				{
					dir = (ch == 75 ? false : true);
					waitDir = false;
				}
				// Если использовали топорик (зашли на вертикальную стенку с активным топором), то отнимаем его и убираем стенку
				if (Lab[indY][indX] == 2 && Stock[3] > 0 && actEl == 3)
				{
					Stock[3]--;
					Lab[indY][indX] = 1;
					ShowStock(Stock, actEl);
				}
				// Если используем Мост (стоим на дыре и активный Мост), то отнимаем его и делаем пол (не забывая обновлять границу)
				if (Lab[indY][indX] == 0 && Stock[6] > 0 && actEl == 6)
				{
					Stock[6]--;
					Lab[indY][indX] = 1;
					ShowStock(Stock, actEl);
					PrintBoard(indY, Lab);
				}
				else if (Lab[indY][indX] == 0) // Падение
					drop = true;
			}
			// При вертикальном движении
			else if (prewY != indY)
			{
				// Если использовали топорик (спустились вниз и сверху пол + активный топор), то отнимаем его и делаем дыру в потолке 
				if (indY > prewY && Lab[indY - 1][indX] == 1 && Stock[3] > 0 && actEl == 3)
				{
					Stock[3]--;
					Lab[indY - 1][indX] = 0;
					ShowStock(Stock, actEl);
					PrintBoard(indY - 1, Lab);
				}
				// Если использовали пружинку (поднялись вверх и стоим на дыре), то отнимаем её
				if (indY < prewY && Lab[indY][indX] == 0 && Stock[5] > 0 && actEl == 5)
				{
					Stock[5]--;
					ShowStock(Stock, actEl);
					PrintLen(prewY, Lab);
				}
				else if (Lab[indY][indX] == 0) // Падение
					drop = true;
				// Обновляем предыдущий этаж
				PrintLen(prewY, Lab);
				// Даём команду считать направлние
				waitDir = true;
			}

			// Падение
			if (drop)
			{
				while (Lab[indY][indX] == 0)
					indY++;
				if (indY - prewY == 2)
				{
					Stock[4] -= 3;
					ShowStock(Stock, actEl);
					if (Stock[4] < 0)
					{
						UpdateStatus("Вы разбились насмерть!");
						life = false;
					}
					else
						UpdateStatus("Падение отняло 3 здоровья!");
				}
				if (indY - prewY > 2)
				{
					Stock[4] = 0;
					ShowStock(Stock, actEl);
					UpdateStatus("Вы разбились насмерть, упав с большой высоты!");
					life = false;
				}
				PrintLen(prewY, Lab);
				waitDir = true;
				drop = false;
			}

			// Кладём предметы в инвентарь
			if (Lab[indY][indX] > 2 && Lab[indY][indX] < 10)
			{
				Stock[Lab[indY][indX]]++;
				char str[64];
				switch (Lab[indY][indX])
				{
				case 3: sprintf(str, "Вы собрали Топорик"); break;
				case 4: sprintf(str, "Вы собрали Аптечку"); break;
				case 5: sprintf(str, "Вы собрали Пружину"); break;
				case 6: sprintf(str, "Вы собрали Мост"); break;
				default: sprintf(str, "Вы собрали нечто неизвестное!"); break;
				}
				Lab[indY][indX] = 1;
				// Выводим инвентарь
				ShowStock(Stock, actEl);
				UpdateStatus(str);
			}
			// Применяем аптечку если надо
			if (Lab[indY][indX] == -1)
			{
				Stock[4]--;
				Lab[indY][indX] = 1;
				ShowStock(Stock, actEl);
				if (Stock[4] < 0)
				{
					UpdateStatus("Вас убила бомба!");
					life = false;
				}
				else
					UpdateStatus("Бомба нанесла вам урон!");
			}
			// Опустошаем здоровье если динамит
			if (Lab[indY][indX] == -2)
			{
				Stock[4] = 0;
				Lab[indY][indX] = 1;
				ShowStock(Stock, actEl);
				UpdateStatus("Вы взорвались на динамите!");
				life = false;
			}

			// Обновляем этаж
			if (life)
				PrintLen(indY, Lab);
			else
				PrintLen(indY, Lab, '№');
			// Если дошли до финиша
			if (Lab[indY][indX] == 20)
			{
				UpdateStatus("Победа! Для перехода на новый уровень нажмите Enter...");
				gotoxy(Col * 3 - 2, Row * 2);
				lvl++;
				do { ch = _getch(); } while (ch != 13);
				ch = 27;
			}
			// Место указателя на финише
			gotoxy(Col * 3 - 2, Row * 2);
			prewY = indY;
			prewX = indX;
		}
		else
		{
			switch (ch)
			{
			case '1': actEl = 3; break;	// Топорик
			case '2': actEl = 5; break;	// Пружинка
			case '3': actEl = 6; break;	// Мост
			default: actEl = 0; break;
			}
			// Выводим инвентарь
			ShowStock(Stock, actEl);
			gotoxy(Col * 3 - 2, Row * 2);
		}
	}
	gotoxy(0, 0);
	delete[] Lab;
	if (lvl <= max_lvl)
		goto Start;
	UpdateStatus("Молодец! Ты прошёл игру");
	gotoxy(Col * 3 - 2, Row * 2);
	_getch();
	return 0;
}