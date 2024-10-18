#include "brick_game/tetris/backend.h"
#include "gui/cli/frontend.h"
int main() {
  TState state;
  state.state = START;
  int timer = 0;
  InitCurses();
  while (state.state != EXIT) {
    UpdateCurrentState(&state);
    KeyPress(&state, 0);
    GameFPS(&state, &timer);
  }
  endwin();
  return 0;
}

void InitCurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  timeout(80);
  mousemask(0, NULL);
  start_color();
  init_pair(1, COLOR_BLACK, COLOR_WHITE);
  init_pair(2, COLOR_WHITE, COLOR_BLACK);
  bkgd(COLOR_PAIR(COLOR));
}