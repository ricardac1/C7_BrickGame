#include "backend.h"

void UpdateCurrentState(TState *state) {
  switch (state->state) {
    case START:
      UserInput(Start, 0, state);
      break;
    case PLAYING:
      UserInput(Action, 0, state);
      break;
    case PAUSED:
      UserInput(Pause, 0, state);
      break;
    case GAME_OVER:
      UserInput(Terminate, 0, state);
      break;
    case EXIT:
      UserInput(Terminate, 0, state);
      break;
  }
}

void KeyPress(TState *state, int kp) {
  GET_KEY();
  if (state->state == START) {
    switch (kp) {
      case ' ':
        state->state = PLAYING;  // Начало игры при нажатии пробела
        break;
      case 'q':
      case 'Q':
        state->state = EXIT;
        break;
    }
  } else if (state->state == PLAYING) {
    switch (kp) {
      case 'p':
      case 'P':
        state->state = PAUSED;
        break;
      case 'q':
      case 'Q':
        state->state = EXIT;
        break;
      case KEY_LEFT:
        UserInput(Left, 0, state);
        break;
      case KEY_RIGHT:
        UserInput(Right, 0, state);
        break;
      case KEY_UP:
        UserInput(Up, 0, state);
        break;
      case KEY_DOWN:
        UserInput(Down, 0, state);
        break;
    }
  } else if (state->state == PAUSED) {
    switch (kp) {
      case 'p':
      case 'P':
        state->state = PLAYING;
        break;
      case 'q':
      case 'Q':
        state->state = EXIT;
        break;
    }
  } else if (state->state == GAME_OVER) {
    switch (kp) {
      case 'q':
      case 'Q':
        state->state = EXIT;
        break;
    }
  }
}
void UserInput(UserAction_t action, bool hold, TState *state) {
  (void)hold;

  switch (action) {
    case Start:
      StartScreen();
      InitGame(state);
      break;
    case Pause:
      PauseScreen();
      break;
    case Terminate:
      GameOverScreen();
      break;
    case Left:
      MoveHorizontal(state, -1);
      PrintField(state);
      break;
    case Right:
      MoveHorizontal(state, 1);
      PrintField(state);
      break;
    case Up:
      MoveRotate(state);
      PrintField(state);
      break;
    case Down:
      DropDown(state);
      PrintField(state);
      state->state = PLAYING;
      break;
    case Action:
      PrintField(state);
      break;
  }
}

void InitGame(TState *state) {
  // Инициализация поля
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      if (i == 0 || j == 0 || j == FIELD_WIDTH - 1 || i == FIELD_HEIGHT - 1) {
        state->field[i][j] = 1;  // Ограждение поля
      } else {
        state->field[i][j] = 0;
      }
    }
  }

  // генерация рандомной фигуры
  GenerateFigure(state);
  SpawnFigure(state);

  // начальная позиция
  state->figureX = START_X;
  state->figureY = 1;
  state->score = 0;  // Инициализация счета нулем
  state->maxScore = 0;
  state->level = 1;  // Ставим 1 уровень
  LoadMaxScore(state);
  srand(time(NULL));
}

void GenerateFigure(TState *state) {
  int figure[7][4][4] = {
      // квадрат[]
      {{2, 2, 2, 2}, {2, 1, 1, 2}, {2, 1, 1, 2}, {2, 2, 2, 2}},
      // линия |
      {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
      // левый угол |__
      {{1, 0, 0, 2}, {1, 1, 1, 2}, {0, 0, 0, 2}, {2, 2, 2, 2}},
      // правый угол __|
      {{0, 0, 1, 2}, {1, 1, 1, 2}, {0, 0, 0, 2}, {2, 2, 2, 2}},
      // левый зигзаг
      {{1, 1, 0, 2}, {0, 1, 1, 2}, {0, 0, 0, 2}, {2, 2, 2, 2}},
      // правый зигзаг
      {{0, 1, 1, 2}, {1, 1, 0, 2}, {0, 0, 0, 2}, {2, 2, 2, 2}},
      // T фигура
      {{0, 1, 0, 2}, {1, 1, 1, 2}, {0, 0, 0, 2}, {2, 2, 2, 2}}};

  int r = rand() % 7;

  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      state->nextFigure[i][j] = figure[r][i][j];
    }
  }
}
bool SpawnFigure(TState *state) {
  // Копируем следующую фигуру как текущую
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      state->currentFigure[i][j] = state->nextFigure[i][j];
    }
  }
  // Сброс состояния фиксации фигуры
  state->isFigureFixed = false;

  // Определение начальной позиции по горизонтали
  state->figureX = START_X;
  state->figureY = 1;
  // Проверка на возможность размещения новой фигуры
  for (int y = 0; y < FIGURE_SIZE; y++) {
    for (int x = 0; x < FIGURE_SIZE; x++) {
      if (state->currentFigure[y][x] == 1 &&
          state->field[state->figureY + y][state->figureX + x] == 1) {
        return false;  // Фигура не может быть размещена, игра окончена
      }
    }
  }
  // Генерация следующей фигуры для будущего
  GenerateFigure(state);
  return true;  // Новая фигура успешно размещена
}

int MoveDown(TState *state) {
  int flag = 0;

  // Проверяем, может ли фигура двигаться вниз
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (state->currentFigure[i][j] == 1 &&
          state->field[state->figureY + i + 1][state->figureX + j] == 1) {
        flag = 1;
        break;
      }
    }
  }
  // если flag = 0, то фигура может двигаться вниз
  if (!flag) {
    state->figureY++;
  }

  return !flag;
}
void MoveHorizontal(TState *state, int dx) {
  int flag = 0;

  // Проверка на возможность перемещения
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (state->currentFigure[i][j] == 1 &&
          state->field[state->figureY + i][state->figureX + j + dx] == 1) {
        flag = 1;
        break;
      }
    }
  }
  // Если нет столкновения, перемещаем фигуру
  if (!flag) {
    state->figureX += dx;
  }
}

void MoveRotate(TState *state) {
  int tempFigure[FIGURE_SIZE][FIGURE_SIZE];

  // Копируем текущую фигуру
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      tempFigure[i][j] = state->currentFigure[i][j];
    }
  }

  // Вращаем копию
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      state->currentFigure[j][FIGURE_SIZE - 1 - i] = tempFigure[i][j];
    }
  }

  // Проверка на столкновение после вращения
  int flag = 0;
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (state->currentFigure[i][j] == 1 &&
          state->field[state->figureY + i][state->figureX + j] == 1) {
        flag = 1;
        break;
      }
    }
  }

  // Если столкновение обнаружено, откатываем вращение
  if (flag) {
    for (int i = 0; i < FIGURE_SIZE; i++) {
      for (int j = 0; j < FIGURE_SIZE; j++) {
        state->currentFigure[i][j] = tempFigure[i][j];
      }
    }
  }
}

void DropDown(TState *state) {
  while (MoveDown(state)) {
  }
}

void FixFigure(TState *state) {
  for (int y = 0; y < FIGURE_SIZE; y++) {
    for (int x = 0; x < FIGURE_SIZE; x++) {
      if (state->currentFigure[y][x] == 1) {
        state->field[state->figureY + y][state->figureX + x] = 1;
      }
    }
  }

  refresh();
}

void DeleteLine(TState *state) {
  int linesdel = 0;
  for (int i = 1; i < FIELD_HEIGHT - 1; i++) {
    bool lineFull = true;
    for (int j = 1; j < FIELD_WIDTH - 1; j++) {
      if (state->field[i][j] == 0) {
        lineFull = false;
        break;
      }
    }
    if (lineFull) {
      linesdel++;
      for (int k = i; k > 1; k--) {  // Сдвигаем все строки сверху вниз
        for (int j = 1; j < FIELD_WIDTH - 1; j++) {
          state->field[k][j] = state->field[k - 1][j];
        }
      }
    }
  }
  if (linesdel > 0) {
    Score(state, linesdel);
  }
  // счет
}
void Score(TState *state, int linesdel) {
  switch (linesdel) {
    case 1:
      state->score += 100;
      break;
    case 2:
      state->score += 300;
      break;
    case 3:
      state->score += 700;
      break;
    case 4:
      state->score += 1500;
      break;
  }
  if (state->score / 600 > state->level - 1 && state->level < 10) {
    state->level++;
  }
  if (state->score > state->maxScore) {
    state->maxScore = state->score;
    SaveScore(state);
  }
}
void SaveScore(TState *state) {
  FILE *file = fopen("score.txt", "w");
  if (file) {
    fprintf(file, "%d", state->maxScore);
    fclose(file);
  } else {
    perror("Error opening file");
  }
}
void LoadMaxScore(TState *state) {
  FILE *file = fopen("score.txt", "r");
  if (file) {
    if (fscanf(file, "%d", &state->maxScore) != 1) {
      state->maxScore = 0;
    }
    fclose(file);
  } else {
    state->maxScore = 0;
    SaveScore(state);
    perror("Error opening file");
  }
}

void GameFPS(TState *state, int *timer) {
  int fps = START_SPEED - (state->level * 2);
  if (state->state == PLAYING && (*timer)++ >= fps) {
    if (!MoveDown(state)) {
      if (!state->isFigureFixed) {
        FixFigure(state);  // Фиксируем фигуру на поле
        state->isFigureFixed = true;
      }
      DeleteLine(state);
      if (state->isFigureFixed) {
        if (!SpawnFigure(state)) {
          state->state = GAME_OVER;
        } else {
          state->isFigureFixed =
              false;  // Флаг сбрасывается после успешного спавна
        }
      }
    }
    (*timer) = 0;
  }
}
