// Copyright (c) 2012 Kaue Soares da Silveira
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files
// (the "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit
// persons to whom the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Author: silveira.kaue@gmail.com (Kaue Soares da Silveira)
//
// This is the presenter implementation.
#include "click/presenter.h"

#include <cstdlib>

#include "click/model.h"
#include "click/navigator.h"
#include "click/view.h"

namespace click {

Presenter::Presenter(Model* model, View* view)
    : model_(model), view_(view), navigator_(new Navigator()),
      is_first_call_to_display_(true) {
  view_->set_presenter(this);
}

void Presenter::Run() {
  view_->CreateWindow();
  view_->MainLoop();
}

Presenter::~Presenter() {
  delete navigator_;
  delete view_;
  delete model_;
}

void Presenter::KeyboardCallback(unsigned char key, bool is_shift,
                                 bool is_ctrl, bool is_alt) {
  const int kEsc = 27, kBackspace = 8, kEnter = 13;
  navigator_->Push(key);
  switch (key) {
    case '!':
    case '[':
    case ']':
    case kEsc:
      exit(0);
      break;
    case '0':
    case 'z':
    case kBackspace:
    case ' ':
      navigator_->Pop();
      break;
    case kEnter:
      view_->MouseClick(is_shift, is_ctrl, is_alt);
      break;
    default:
      break;
  }
  view_->PostRedisplay();
}

void Presenter::DisplayCallback() {
  if (!view_->IsFullScreen()) {
    return;
  }

  int window_width = view_->width(), window_height = view_->height();
  int map_width = window_width, map_height = window_height, x = 0, y = 0;
  int division = 3;
  navigator_->Navigate(division, window_width, window_height,
                       &map_width, &map_height, &x, &y);

  view_->BeginDisplay();

  if (is_first_call_to_display_) {
    is_first_call_to_display_ = false;
    view_->SaveScreen();
  }
  view_->LoadScreen(window_width, window_height, 0, 0);
  DrawMap(division, window_width, window_height, map_width, map_height, x,
          y);
  view_->EndDisplay();
}

void Presenter::DrawMap(int division, int window_width, int window_height,
                        int map_width, int map_height, int x, int y) {
  for (int i = 0; i < division; i++) {
    for (int j = 0; j < division; j++) {
      char letter = j * division + i + '1';
      view_->DrawSquare(i, j, division, map_width, map_height, x, y,
                        letter);
    }
  }
  view_->MouseMove(x + map_width / 2, window_height - y - map_height / 2);
}

}  // namespace click
