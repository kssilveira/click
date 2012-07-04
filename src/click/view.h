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
// This file has the View classes.

#ifndef SRC_CLICK_VIEW_H_
#define SRC_CLICK_VIEW_H_

#include "click/macros.h"

namespace click {

class Presenter;

// This is the view. There should be only one instance of this class.
class View {
 public:
  View(int argc, char** argv);

  void set_presenter(Presenter* presenter) {
    presenter_ = presenter;
  }

  void CreateWindow();
  void MainLoop();

  // Callbacks.
  void DisplayCallback();
  void KeyboardCallback(unsigned char key, int x, int y);
  void SpecialKeyCallback(int key, int x, int y);
  void ReshapeCallback(int width, int height);
  void MouseCallback(int button, int state, int x, int y);

 private:
  Presenter* presenter_;
  DISALLOW_COPY_AND_ASSIGN(View);
};

}  // namespace click

#endif  // SRC_CLICK_VIEW_H_
