/*
В этом учебном проекте мы создадим игру "Динозавр", которая работает на платформе Arduino с использованием дисплея LCD 1602 i2c.
Цель игры - управлять динозавром, который должен перепрыгивать препятствия (ветки и птицы), избегая столкновений. 
Игра будет выводить сообщение "Game Over" и уровень игрока при поражении.
*/
#include <LiquidCrystal_I2C.h>  // Подключение библиотеки для работы с LCD дисплеем

LiquidCrystal_I2C lcd(0x27, 16, 2); // Инициализация дисплея LCD 1602 i2c с адресом 0x27

// Создание пользовательских символов для динозавра, веток и птиц
byte DINO_STAND_PART_1[8] = {B00000, B00000, B00010, B00010, B00011, B00011, B00001, B00001};
byte DINO_STAND_PART_2[8] = {B00111, B00111, B00111, B00100, B11100, B11100, B11000, B01000};
byte DINO_RIGHT_FOOT_PART_1[8] = {B00000, B00000, B00010, B00010, B00011, B00011, B00001, B00001};
byte DINO_RIGHT_FOOT_PART_2[8] = {B00111, B00111, B00111, B00100, B11100, B11100, B11000, B00000};
byte DINO_LEFT_FOOT_PART_1[8] = {B00000, B00000, B00010, B00010, B00011, B00011, B00001, B00000};
byte DINO_LEFT_FOOT_PART_2[8] = {B00111, B00111, B00111, B00100, B11100, B11100, B11000, B01000};
byte TWO_BRANCHES_PART_1[8] = {B00000, B00100, B00100, B10100, B10100, B11100, B00100, B00100};
byte TWO_BRANCHES_PART_2[8] = {B00100, B00101, B00101, B10101, B11111, B00100, B00100, B00100};
byte BIRD_WINGS_PART1[8] = {B00001, B00001, B00001, B00001, B01001, B11111, B00000, B00000};
byte BIRD_WINGS_PART2[8] = {B00000, B10000, B11000, B11100, B11110, B11111, B00000, B00000};

// Объявление переменных для игры
int dinoCol1 = 1; // колонка динозавра, часть 1
int dinoCol2 = 2; // колонка динозавра, часть 2
int dinoRow = 1; // строка динозавра
unsigned long clock = 0; // для использования функции millis вместо delay
int period = 100; // период в миллисекундах
int flag = 1; // флаг для переключения между частями динозавра
int branchRow = 0; // строка ветки
int branchCol = 13; // колонка ветки
int period2 = 100; // период для обновления веток
unsigned long clock2 = 0; // второй таймер
int a = 0; // вспомогательная переменная для обновления экрана
int b = 1; // начальная позиция для столкновения
int c = 2; // начальная позиция для столкновения
int d = 0; // флаг для прыжка
unsigned long clock3 = 0; // третий таймер
int period3 = 100; // период для обновления очков
int points = 0; // счетчик очков
int points2 = 0; // счетчик для отображения десятков очков
int level = 0; // уровень игры (количество очков)
int randomNum = 0; // случайное число для определения препятствия
int birdCol = 13; // колонка птицы
int e = 0; // вспомогательная переменная
int birdRow = 1; // строка птицы
int currentSignal = 0; // текущее состояние кнопки
int previousSignal = 0; // предыдущее состояние кнопки
int f = 13; // вспомогательная переменная для очистки экрана
int acceleration = 1; // ускорение игры
unsigned long clock4 = 0; // четвертый таймер
int period4 = 800; // период для накопления очков
int button = 11; // пин подключения кнопки для прыжка динозавра
int buzzer = 10; // пин подключения зуммера для звуковых эффектов

void setup() {
  for (int i = 2; i <= 17; i++)  pinMode(i, OUTPUT); // установка всех пинов от 2 до 17 в режим OUTPUT

  pinMode(button, INPUT); // установка пина кнопки в режим INPUT
  pinMode(buzzer, OUTPUT); // установка пина зуммера в режим OUTPUT

  lcd.init(); // инициализация LCD дисплея
  lcd.backlight(); // включение подсветки дисплея
  lcd.setCursor(0, 0); // установка курсора на начальную позицию
  lcd.print("   Created by");
  lcd.setCursor(0,1);
  lcd.print("Alash education");
  delay(4000);
  lcd.clear();
  lcd.createChar(0, DINO_STAND_PART_1); // создание пользовательских символов для динозавра
  lcd.createChar(1, DINO_STAND_PART_2);
  lcd.createChar(2, DINO_RIGHT_FOOT_PART_1);
  lcd.createChar(3, DINO_RIGHT_FOOT_PART_2);
  lcd.createChar(4, DINO_LEFT_FOOT_PART_1);
  lcd.createChar(5, DINO_LEFT_FOOT_PART_2);
  lcd.createChar(6, TWO_BRANCHES_PART_1); // создание пользовательских символов для веток
  lcd.createChar(7, TWO_BRANCHES_PART_2);
}

void loop() {
  if (millis() > clock + period) { // задержка для ног динозавра
    clock = millis(); // обновление времени
    if (flag == 1) {
      flag = 2; // переключение флага
    } else if (flag == 2) {
      flag = 1; // переключение флага
    }
  }

  if (millis() > clock2 + period2) { // задержка для скорости веток
    clock2 = millis(); // обновление времени
    branchCol = branchCol - 1; // перемещение ветки влево
    if (branchCol < 0) {
      branchCol = 13; // возврат ветки в начальную позицию
      period2 = period2 - acceleration; // ускорение ветки
      randomNum = random(0, 3); // генерация случайного числа при возврате к колонке 13
    }

    f = branchCol + 1; // установка переменной для очистки экрана
    lcd.setCursor(f, 1); // очищение нижней строки
    lcd.print(" ");
    f = branchCol + 1;
    lcd.setCursor(f, 0); // очищение верхней строки
    lcd.print(" ");
    lcd.setCursor(0, 1); // очищение верхней строки
    lcd.print(" ");
    lcd.setCursor(0, 0);
    lcd.print(" ");

    a = 1; // установка флага для обновления экрана
  }

  if (d == 0) { // проверка флага прыжка
    if (flag == 1) {
      lcd.setCursor(dinoCol1, dinoRow); // установка курсора на позицию динозавра
      lcd.write(byte(2)); // отображение первой части динозавра
      lcd.setCursor(dinoCol2, dinoRow);
      lcd.write(byte(3)); // отображение второй части динозавра
    }
    if (flag == 2) {
      lcd.setCursor(dinoCol1, dinoRow);
      lcd.write(byte(4)); // отображение первой части динозавра с другой ногой
      lcd.setCursor(dinoCol2, dinoRow);
      lcd.write(byte(5)); // отображение второй части динозавра с другой ногой
    }
  }

  if (a == 1) { // проверка флага для обновления экрана
    if (randomNum == 1) {
      branchRow = 1; // установка строки ветки
      lcd.createChar(6, TWO_BRANCHES_PART_1); // создание символа ветки
      lcd.setCursor(branchCol, branchRow); // установка курсора на позицию ветки
      lcd.write(byte(6)); // отображение ветки
    } else if (randomNum == 2) {
      branchRow = 1;
      lcd.createChar(7, TWO_BRANCHES_PART_2); // создание символа ветки
      lcd.setCursor(branchCol, branchRow);
      lcd.write(byte(7)); // отображение ветки
    } else { // часть для птицы
      birdCol = branchCol

 - 1; // установка колонки птицы
      branchRow = 0; // установка строки ветки
      lcd.createChar(6, BIRD_WINGS_PART1); // создание символа птицы
      lcd.setCursor(birdCol, branchRow); // установка курсора на позицию птицы
      lcd.write(byte(6)); // отображение первой части птицы
      lcd.createChar(7, BIRD_WINGS_PART2); // создание символа птицы
      lcd.setCursor(branchCol, branchRow); // установка курсора на позицию птицы
      lcd.write(byte(7)); // отображение второй части птицы
    }
    a = 0; // сброс флага для обновления экрана
  }

  // Проверка условий столкновения
  if (digitalRead(button) == HIGH && (branchCol == 1 || branchCol == 2 || birdCol == 1 || birdCol == 2) && branchRow == 0) {
    gameOver(); // вызов функции gameOver при столкновении
  }

  if ((branchCol == b || branchCol == c) && branchRow == 1) { // условие для ветки
    int note[] = {200, 150}; // массив нот для зуммера
    for (int i = 0; i < 2; i++) {
      tone(buzzer, note[i], 250); // воспроизведение звука
      delay(200); // задержка
    }
    gameOver(); // вызов функции gameOver при столкновении
  }

  // Прыжок при нажатии кнопки
  if (digitalRead(button) == HIGH) {
    b = 50; // установка значений b и c больше 15, чтобы не совпадать с движением колонки
    c = 50;

    if (d == 0) {
      lcd.setCursor(0, 1); // очищение нижней строки
      lcd.print("    ");
    }
    d = 1; // установка флага для прыжка

    lcd.setCursor(dinoCol1, 0); // установка курсора на позицию динозавра в прыжке
    lcd.write(byte(2)); // отображение первой части динозавра в прыжке
    lcd.setCursor(dinoCol2, 0); // установка курсора на позицию динозавра в прыжке
    lcd.write(byte(3)); // отображение второй части динозавра в прыжке

    if (millis() > clock4 + period4) { // задержка для накопления очков
      clock4 = millis(); // обновление времени
      int note[] = {600}; // массив нот для зуммера
      for (int i = 0; i < 1; i++) {
        tone(buzzer, note[i], 150); // воспроизведение звука
        delay(20); // задержка
      }
    }
  } else {
    b = 1; // возврат к начальному значению
    c = 2;
    d = 0; // сброс флага прыжка
  }

  // Задержка для накопления очков
  if (millis() > clock3 + period3) { // задержка для накопления очков
    clock3 = millis(); // обновление времени
    lcd.setCursor(14, 1); // установка курсора на позицию для вывода очков
    lcd.print(points); // вывод очков

    points = points + 1; // увеличение очков

    if (points == 100) { // проверка достижения 100 очков
      int note[] = {800, 900}; // массив нот для зуммера
      for (int i = 0; i < 2; i++) {
        tone(buzzer, note[i], 150); // воспроизведение звука
        delay(150); // задержка
        points = 0; // сброс очков
        points2 = points2 + 1; // увеличение десятков очков
        if (points2 == 100) {
          points2 = 0; // сброс десятков очков
        }
      }
    }

    lcd.setCursor(14, 1); // установка курсора на позицию для вывода очков
    lcd.print(points); // вывод очков
    lcd.setCursor(14, 0); // установка курсора на позицию для вывода десятков очков
    lcd.print(points2); // вывод десятков очков

    // детектор изменения состояния кнопки
    currentSignal = digitalRead(button); // считывание текущего состояния кнопки
    if (currentSignal != previousSignal) { // проверка изменения состояния кнопки
      lcd.setCursor(1, 0); // установка курсора на позицию для очистки экрана
      lcd.print("  "); // очистка экрана
    }
    previousSignal = currentSignal; // обновление состояния кнопки
  }
}

// Функция для отображения "Game Over" и уровня игры
void gameOver() {
  lcd.clear(); // очистка экрана
  lcd.setCursor(4, 0); // установка курсора на позицию для вывода сообщения
  lcd.print("GAME OVER"); // вывод сообщения "GAME OVER"
  lcd.setCursor(4, 1); // установка курсора на позицию для вывода уровня
  lcd.print("Level: "); // вывод текста "Level: "
  lcd.print(points + points2 * 100); // вывод уровня (общего количества очков)
  delay(2000); // задержка 2 секунды
  lcd.clear(); // очистка экрана
  branchCol = 15; // сброс колонки ветки
  period2 = 100; // сброс периода для веток
  points = 0; // сброс очков
  points2 = 0; // сброс десятков очков
}
