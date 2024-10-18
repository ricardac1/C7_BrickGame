#ifndef BACKEND_H
#define BACKEND_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../gui/cli/frontend.h"
#define FIELD_WIDTH 12
#define FIELD_HEIGHT 20
#define FIGURE_SIZE 4
#define TETRAMINOS 7
#define START_SPEED 10
#define START_X 4

#ifdef TEST_MODE
#define GET_KEY()
#else
#define GET_KEY() kp = getch()
#endif

/**
 * Структура для хранения информации о текущем статусе игры
 */
typedef enum { START, PLAYING, PAUSED, GAME_OVER, EXIT } GameState;

/**
 * Структура для хранения информации о текущем состоянии игры
 */
typedef struct {
  int field[FIELD_HEIGHT][FIELD_WIDTH];         // Поле для игры
  int currentFigure[FIGURE_SIZE][FIGURE_SIZE];  // Текущая фигура
  int nextFigure[FIGURE_SIZE][FIGURE_SIZE];  // Следующая фигура
  int figureX;         // Позиция фигуры по оси X
  int figureY;         // Позиция фигуры по оси Y
  int score;           // Текущий счёт
  bool isFigureFixed;  // Зафиксирована ли фигура на поле
  int maxScore;        // Максимальный счёт
  int level;           // Текущий уровень
  GameState state;  // Текущее состояние игры
} TState;

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

/**
 * Обновляет текущее состояние игры в зависимости от изменений.
 */
void UpdateCurrentState(TState *state);

/**
 * Обрабатывает действия пользователя: движение, повороты, пауза и т.д.
 * @param action Действие пользователя.
 * @param state Указатель на текущее состояние игры.
 */
void UserInput(UserAction_t action, bool hold, TState *state);

/**
 * Обрабатывает нажатия клавиш для управления игрой.
 * @param state Указатель на текущее состояние игры.
 * @param kp Код нажатой клавиши.
 */
void KeyPress(TState *state, int kp);

/**
 * Инициализирует начальное состояние игры.
 * @param state Указатель на текущее состояние игры.
 */
void InitGame(TState *state);

/**
 * Отображает игровое поле.
 * @param state Указатель на текущее состояние игры.
 */
void PrintField(TState *state);

/**
 * Генерирует новую фигуру для игры.
 * @param state Указатель на текущее состояние игры.
 */
void GenerateFigure(TState *state);

/**
 * Отображает текущую фигуру на поле.
 * @param state Указатель на текущее состояние игры.
 */
void PrintFigure(TState *state);

/**
 * Перемещает фигуру по горизонтали.
 * @param state Указатель на текущее состояние игры.
 * @param dx Смещение по оси X.
 */
void MoveHorizontal(TState *state, int dx);

/**
 * Выполняет поворот фигуры.
 * @param state Указатель на текущее состояние игры.
 */
void MoveRotate(TState *state);

/**
 * Перемещает фигуру вниз.
 * @param state Указатель на текущее состояние игры.
 * @return 1, если перемещение возможно, иначе 0.
 */
int MoveDown(TState *state);

/**
 * "Роняет" фигуру вниз до конца.
 * @param state Указатель на текущее состояние игры.
 */
void DropDown(TState *state);

/**
 * Фиксирует текущую фигуру на игровом поле.
 * @param state Указатель на текущее состояние игры.
 */
void FixFigure(TState *state);

/**
 * Управляет игровым циклом с учётом FPS и уровня.
 * @param state Указатель на текущее состояние игры.
 * @param timer Указатель на счётчик FPS.
 */
void GameFPS(TState *state, int *timer);

/**
 * Спавнит новую фигуру в игре.
 * @param state Указатель на текущее состояние игры.
 * @return true, если спавн возможен, иначе false (игра окончена).
 */
bool SpawnFigure(TState *state);

/**
 * Отображает следующую фигуру.
 * @param state Указатель на текущее состояние игры.
 */
void PrintNextFigure(TState *state);

/**
 * Отображает зафиксированные фигуры на поле.
 * @param state Указатель на текущее состояние игры.
 */
void PrintFixFigure(TState *state);

/**
 * Удаляет заполненные линии с игрового поля.
 * @param state Указатель на текущее состояние игры.
 */
void DeleteLine(TState *state);

/**
 * Рассчитывает и обновляет счёт игрока.
 * @param state Указатель на текущее состояние игры.
 * @param linesdel Количество удалённых линий.
 */
void Score(TState *state, int linesdel);

/**
 * Сохраняет максимальный счёт в файл.
 * @param state Указатель на текущее состояние игры.
 */
void SaveScore(TState *state);

/**
 * Загружает максимальный счёт из файла.
 * @param state Указатель на текущее состояние игры.
 */
void LoadMaxScore(TState *state);

#endif
