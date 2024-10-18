#include <check.h>

#include "../brick_game/tetris/backend.h"

START_TEST(test_update_current_state) {
  TState state;

  state.state = START;
  UpdateCurrentState(&state);
  ck_assert_int_eq(state.state, START);

  state.state = PLAYING;
  UpdateCurrentState(&state);
  ck_assert_int_eq(state.state, PLAYING);

  state.state = PAUSED;
  UpdateCurrentState(&state);
  ck_assert_int_eq(state.state, PAUSED);

  state.state = GAME_OVER;
  UpdateCurrentState(&state);
  ck_assert_int_eq(state.state, GAME_OVER);

  state.state = EXIT;
  UpdateCurrentState(&state);
  ck_assert_int_eq(state.state, EXIT);
}
END_TEST

START_TEST(test_game_fps) {
  TState state;
  int timer = 0;
  InitGame(&state);

  // 1. Тестируем, что игра в начальном состоянии
  state.state = PLAYING;
  state.level = 1;
  GameFPS(&state, &timer);
  ck_assert_int_eq(timer, 1);
  ck_assert_int_eq(state.state, PLAYING);

  // 2. Тестируем, что движение вниз невозможно
  timer = 20;
  state.state = PLAYING;
  state.level = 1;
  state.figureY = FIELD_HEIGHT - 4;
  GameFPS(&state, &timer);
  ck_assert_int_eq(state.state, PLAYING);
}
END_TEST

START_TEST(test_score) {
  TState state;
  InitGame(&state);

  // удаление 1 линии
  state.score = 0;
  state.level = 1;
  Score(&state, 1);
  ck_assert_int_eq(state.score, 100);
  ck_assert_int_eq(state.level, 1);

  // удаление 2 линий
  state.score = 0;
  Score(&state, 2);
  ck_assert_int_eq(state.score,
                   300);  // Проверяем, что очки добавлены правильно

  //  удаление 3 линий
  state.score = 0;
  Score(&state, 3);
  ck_assert_int_eq(state.score,
                   700);  // Проверяем, что очки добавлены правильно

  //  удаление 4 линий
  state.score = 0;
  Score(&state, 4);
  ck_assert_int_eq(state.score,
                   1500);  // Проверяем, что очки добавлены правильно

  // Тестируем повышение уровня
  state.score = 500;
  state.level = 1;
  Score(&state, 2);
  ck_assert_int_eq(state.level, 2);  // Уровень должен повыситься

  // Тестируем обновление максимального счёта
  state.maxScore = 2000;
  state.score = 2100;  // Превышаем предыдущий maxScore
  Score(&state, 1);    // Добавляем 100 очков
  ck_assert_int_eq(state.maxScore, 2200);  // maxScore должен обновиться

  // Тестируем, что счёт не повышает уровень выше 10
  state.level = 10;
  state.score = 6000;
  Score(&state, 4);
  ck_assert_int_eq(state.level, 10);  // Уровень не должен повышаться выше 10
}
END_TEST

START_TEST(test_delete_line) {
  TState state;
  InitGame(&state);

  // Создадим полную линию на второй строке
  for (int j = 1; j < FIELD_WIDTH - 1; j++) {
    state.field[1][j] = 1;
  }

  state.score = 0;
  state.level = 1;
  DeleteLine(&state);

  // Проверяем, что счёт обновился
  ck_assert_int_eq(state.score,
                   100);  // За одну удалённую линию добавляется 100 очков
  ck_assert_int_eq(state.level, 1);  // Уровень не должен измениться
}
END_TEST

START_TEST(test_fix_figure) {
  TState state;
  InitGame(&state);

  state.figureX = 3;
  state.figureY = 2;

  state.currentFigure[0][0] = 1;
  state.currentFigure[0][1] = 1;
  state.currentFigure[1][0] = 1;
  state.currentFigure[1][1] = 1;

  // Фиксируем фигуру на поле
  FixFigure(&state);

  ck_assert_int_eq(state.field[2][3], 1);
  ck_assert_int_eq(state.field[2][4], 1);
  ck_assert_int_eq(state.field[3][3], 1);
  ck_assert_int_eq(state.field[3][4], 1);
}
END_TEST

START_TEST(test_init_game) {
  TState state;
  InitGame(&state);
  ck_assert_int_eq(state.level, 1);
  ck_assert_int_eq(state.score, 0);
  ck_assert_int_ne(state.figureX, 0);
}
END_TEST

START_TEST(check_fields) {
  TState state;
  InitGame(&state);

  ck_assert_ptr_nonnull(state.field);

  // Проверка значений поля
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      if (i == 0 || j == 0 || j == FIELD_WIDTH - 1 || i == FIELD_HEIGHT - 1) {
        // Ограждения должны быть равны 1
        ck_assert_int_eq(state.field[i][j], 1);
      } else {
        // Внутренние ячейки должны быть равны 0
        ck_assert_int_eq(state.field[i][j], 0);
      }
    }
  }
}
END_TEST

START_TEST(test_move_horizontal) {
  TState state;
  InitGame(&state);
  // Перемещаем фигуру вправо
  state.figureX = 1;
  MoveHorizontal(&state, 1);
  ck_assert_int_eq(state.figureX, 2);

  // Пробуем переместить фигуру в стену
  state.figureX = FIELD_WIDTH - 2;
  MoveHorizontal(&state, 1);
  ck_assert_int_eq(state.figureX, FIELD_WIDTH - 2);

  // Проверяем перемещение влево
  state.figureX = 2;
  MoveHorizontal(&state, -1);
  ck_assert_int_eq(state.figureX, 1);

  // Пробуем переместить фигуру в стену слева
  state.figureX = 2;
  MoveHorizontal(&state, -1);
  ck_assert_int_eq(state.figureX, 1);
}
END_TEST

START_TEST(test_move_down) {
  TState state;
  InitGame(&state);
  state.figureX = 4;
  state.figureY = 0;

  // Установим фигуру квадрата
  state.currentFigure[0][0] = 1;
  state.currentFigure[0][1] = 1;
  state.currentFigure[1][0] = 1;
  state.currentFigure[1][1] = 1;

  ck_assert_int_eq(MoveDown(&state), 1);
  ck_assert_int_eq(state.figureY, 1);  // Фигура должна переместиться вниз

  state.field[2][4] = 1;

  ck_assert_int_eq(MoveDown(&state), 0);
  ck_assert_int_eq(state.figureY, 1);  // Позиция не должна измениться
}
END_TEST

START_TEST(test_drop_down) {
  TState state;
  InitGame(&state);

  state.figureY = 1;
  state.currentFigure[0][0] = 1;
  state.currentFigure[0][1] = 1;
  state.currentFigure[1][0] = 1;
  state.currentFigure[1][1] = 1;

  DropDown(&state);

  ck_assert_int_eq(state.figureY, FIELD_HEIGHT - FIGURE_SIZE);
}
END_TEST
START_TEST(test_save_score) {
  TState state;
  state.maxScore = 12345;
  SaveScore(&state);

  FILE *file = fopen("score.txt", "r");
  ck_assert_ptr_nonnull(file);
  int savedScore;
  fscanf(file, "%d", &savedScore);
  fclose(file);

  // Проверяем, что сохранённый счёт соответствует ожидаемому
  ck_assert_int_eq(savedScore, state.maxScore);

  remove("score.txt");
}
END_TEST

START_TEST(user_input_start1) {
  TState state;
  state.state = START;

  KeyPress(&state, 'q');

  // Проверяем, что состояние изменилось на EXIT
  ck_assert_int_eq(state.state, EXIT);
}
END_TEST

START_TEST(user_input_start2) {
  TState state;
  state.state = START;

  KeyPress(&state, 'Q');

  ck_assert_int_eq(state.state, EXIT);
}

START_TEST(user_input_pause1) {
  TState state;
  state.state = PLAYING;

  KeyPress(&state, 'p');

  ck_assert_int_eq(state.state, PAUSED);
}
END_TEST

START_TEST(user_input_pause2) {
  TState state;
  state.state = PLAYING;

  KeyPress(&state, 'P');

  ck_assert_int_eq(state.state, PAUSED);
}
END_TEST

START_TEST(user_input_pause3) {
  TState state;
  state.state = PAUSED;

  KeyPress(&state, 'P');

  ck_assert_int_eq(state.state, PLAYING);
}
END_TEST

START_TEST(user_input_pause4) {
  TState state;
  state.state = PAUSED;

  KeyPress(&state, 'p');

  ck_assert_int_eq(state.state, PLAYING);
}
END_TEST

START_TEST(user_input_pause5) {
  TState state;
  state.state = PAUSED;

  KeyPress(&state, 'Q');

  ck_assert_int_eq(state.state, EXIT);
}
END_TEST

START_TEST(user_input_pause6) {
  TState state;
  state.state = PAUSED;

  KeyPress(&state, 'q');

  ck_assert_int_eq(state.state, EXIT);
}
END_TEST

START_TEST(user_input_over1) {
  TState state;
  state.state = GAME_OVER;

  KeyPress(&state, 'q');

  ck_assert_int_eq(state.state, EXIT);
}
END_TEST

START_TEST(user_input_over2) {
  TState state;
  state.state = GAME_OVER;

  KeyPress(&state, 'Q');

  ck_assert_int_eq(state.state, EXIT);
}
END_TEST

START_TEST(user_input_space) {
  TState state;
  state.state = START;

  KeyPress(&state, ' ');

  ck_assert_int_eq(state.state, PLAYING);
}
END_TEST

START_TEST(user_input_exit1) {
  TState state;
  state.state = PLAYING;

  KeyPress(&state, 'Q');
  ck_assert_int_eq(state.state, EXIT);
}
END_TEST

START_TEST(user_input_exit2) {
  TState state;
  state.state = PLAYING;

  KeyPress(&state, 'q');
  ck_assert_int_eq(state.state, EXIT);
}
END_TEST

START_TEST(user_input_left) {
  TState state;
  InitGame(&state);
  state.state = PLAYING;
  KeyPress(&state, KEY_LEFT);
  ck_assert_int_eq(state.state, PLAYING);
}
END_TEST

START_TEST(user_input_right) {
  TState state;
  InitGame(&state);
  state.state = PLAYING;
  KeyPress(&state, KEY_RIGHT);
  ck_assert_int_eq(state.state, PLAYING);
}
END_TEST

START_TEST(user_input_up) {
  TState state;
  InitGame(&state);
  state.state = PLAYING;
  KeyPress(&state, KEY_UP);
  ck_assert_int_eq(state.state, PLAYING);
}
END_TEST

START_TEST(user_input_down) {
  TState state;
  InitGame(&state);
  state.state = PLAYING;
  KeyPress(&state, KEY_DOWN);
  ck_assert_int_eq(state.state, PLAYING);
}
END_TEST

Suite *test_game_user_input_action(void) {
  Suite *s;
  s = suite_create("s21_game_user_input_action");
  TCase *tcase_user_input = tcase_create("USER_INPUT");
  tcase_add_test(tcase_user_input, user_input_start1);
  tcase_add_test(tcase_user_input, user_input_start2);
  tcase_add_test(tcase_user_input, user_input_pause1);
  tcase_add_test(tcase_user_input, user_input_pause2);
  tcase_add_test(tcase_user_input, user_input_pause3);
  tcase_add_test(tcase_user_input, user_input_pause4);
  tcase_add_test(tcase_user_input, user_input_pause5);
  tcase_add_test(tcase_user_input, user_input_pause6);
  tcase_add_test(tcase_user_input, user_input_over1);
  tcase_add_test(tcase_user_input, user_input_over2);
  tcase_add_test(tcase_user_input, user_input_space);
  tcase_add_test(tcase_user_input, user_input_exit1);
  tcase_add_test(tcase_user_input, user_input_exit2);

  tcase_add_test(tcase_user_input, user_input_left);
  tcase_add_test(tcase_user_input, user_input_right);

  tcase_add_test(tcase_user_input, user_input_up);
  tcase_add_test(tcase_user_input, user_input_down);

  suite_add_tcase(s, tcase_user_input);
  return s;
}
Suite *test_create(void) {
  Suite *s;
  s = suite_create("s21_initial_game");
  TCase *tcase_initial = tcase_create("CREATE");
  tcase_add_test(tcase_initial, test_init_game);
  tcase_add_test(tcase_initial, check_fields);
  tcase_add_test(tcase_initial, test_move_horizontal);
  tcase_add_test(tcase_initial, test_move_down);
  tcase_add_test(tcase_initial, test_drop_down);
  tcase_add_test(tcase_initial, test_save_score);
  tcase_add_test(tcase_initial, test_update_current_state);
  tcase_add_test(tcase_initial, test_fix_figure);
  tcase_add_test(tcase_initial, test_score);
  tcase_add_test(tcase_initial, test_delete_line);
  tcase_add_test(tcase_initial, test_game_fps);

  suite_add_tcase(s, tcase_initial);
  return s;
}
static int run_test_suite(Suite *test_suite) {
  int number_failed = 0;
  SRunner *sr = NULL;

  sr = srunner_create(test_suite);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed);
}
int main(void) {
  int number_failed = 0;
  Suite *suite_array[] = {

      test_create(), test_game_user_input_action(),

      NULL};

  for (size_t i = 0; suite_array[i]; ++i) {
    number_failed += run_test_suite(suite_array[i]);
  }
  printf("NUMBER FAILED %d\n", number_failed);
}
