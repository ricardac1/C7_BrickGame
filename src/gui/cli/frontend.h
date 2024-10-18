#ifndef FRONTEND_H
#define FRONTEND_H

#include "../../brick_game/tetris/backend.h"
#define COLOR 2  // Цветовая палитра терминала (1 - белый, 2 - черный)

/**
 * Инициализирует библиотеку ncurses и настраивает основные параметры окна.
 */
void InitCurses();

/**
 * Отображает стартовый экран игры с информацией о начале игры.
 */
void StartScreen();

/**
 * Отображает экран паузы при приостановке игры.
 */
void PauseScreen();

/**
 * Отображает экран окончания игры с финальными результатами.
 */
void GameOverScreen();

/**
 * Рисует квадрат на экране с указанными координатами и размерами.
 * @param x Координата X левого верхнего угла квадрата.
 * @param y Координата Y левого верхнего угла квадрата.
 * @param width Ширина квадрата.
 * @param height Высота квадрата.
 */
void DrawSquare(int x, int y, int width, int height);

#endif
