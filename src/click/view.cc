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
// This is the view implementation.
#include "click/view.h"

#include <GL/glut.h>

namespace click {

namespace {

void DisplayFunc() {}
void KeyboardFunc(unsigned char key, int x, int y) {}
void SpecialFunc(int key, int x, int y) {}
void ReshapeFunc(int width, int height) {}
void MouseFunc(int button, int state, int x, int y) {}

}  // namespace

View::View(int argc, char** argv) {
  glutInit(&argc, argv);
}

void View::CreateWindow() {
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  const char* kWindowName = "";
  glutCreateWindow(kWindowName);
  glutFullScreen();
  glutDisplayFunc(DisplayFunc);
  glutKeyboardFunc(KeyboardFunc);
  glutSpecialFunc(SpecialFunc);
  glutReshapeFunc(ReshapeFunc);
  glutMouseFunc(MouseFunc);
}

void View::MainLoop() {
  glutMainLoop();
}

}  // namespace click
