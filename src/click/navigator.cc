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
// This is the navigator implementation
#include "click/navigator.h"

namespace click {

Navigator::Navigator() {}

void Navigator::Navigate(int division, int window_width, int window_height,
                         int* map_width, int* map_height, int* x, int* y) {
  for (size_t i = 0; i < path_.size(); ++i) {
    *map_width /= division;
    *map_height /= division;

    char letter = path_[i] - '1';
    int cx = 0, cy = 0;
    cx = letter % division;
    cy = letter / division;
    *x += cx * *map_width;
    *y += cy * *map_height;
  }
}

void Navigator::Push(char letter) {
  if (letter >= '1' && letter <= '9') {
    path_.push_back(letter);
  }
}

void Navigator::Pop() {
  if (!path_.empty()) {
    path_.pop_back();
  }
}

}  // namespace click
