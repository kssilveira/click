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

#include <cassert>

#include "click/presenter.h"

namespace click {

namespace {

// Used in glut callbacks.
static View* g_view = NULL;

void DisplayFunc() {
  g_view->DisplayCallback();
}

void KeyboardFunc(unsigned char key, int x, int y) {
  g_view->KeyboardCallback(key, x, y);
}

void SpecialFunc(int key, int x, int y) {
  g_view->SpecialKeyCallback(key, x, y);
}

void ReshapeFunc(int width, int height) {
  g_view->ReshapeCallback(width, height);
}

void MouseFunc(int button, int state, int x, int y) {
  g_view->MouseCallback(button, state, x, y);
}

}  // namespace

View::View(int argc, char** argv) {
  assert(g_view == NULL);
  g_view = this;
  glutInit(&argc, argv);
}

int View::width() {
  return glutGet((GLenum) GLUT_WINDOW_WIDTH);
}

int View::height() {
  return glutGet((GLenum) GLUT_WINDOW_HEIGHT);
}

int View::screen_width() {
  return glutGet((GLenum) GLUT_SCREEN_WIDTH);
}

int View::screen_height() {
  return glutGet((GLenum) GLUT_SCREEN_HEIGHT);
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
  screen_pixels_ = new unsigned char[3 * screen_width() * screen_height()];
}

void View::MainLoop() {
  glutMainLoop();
}

bool View::IsFullScreen() {
  return width() == screen_width() && height() == screen_height();
}

void View::SaveScreen() {
  glReadPixels(0, 0, width(), height(), GL_RGB, GL_UNSIGNED_BYTE,
               screen_pixels_);
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  const int kLevel = 0;
  const int kBorder = 0;
  glTexImage2D(GL_TEXTURE_2D, kLevel, GL_RGB, width(), height(), kBorder,
      GL_RGB, GL_UNSIGNED_BYTE, screen_pixels_);
}

void View::LoadScreen(int map_width, int map_height, int x, int y) {
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  double points[][2] = {
      {x / static_cast<double>(width()),
       y / static_cast<double>(height())},
      {(x + map_width) / static_cast<double>(width()),
       (y + map_height) / static_cast<double>(height())}};

  glBegin(GL_POLYGON);
    glTexCoord2f(points[0][0], points[0][1]);
    glVertex2i(0, 0);
    glTexCoord2f(points[0][0], points[1][1]);
    glVertex2i(0, height());
    glTexCoord2f(points[1][0], points[1][1]);
    glVertex2i(width(), height());
    glTexCoord2f(points[1][0], points[0][1]);
    glVertex2i(width(), 0);
  glEnd();

  glDisable(GL_TEXTURE_2D);
}

void View::BeginDisplay() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, width(), height());
  gluOrtho2D(0, width(), 0, height());
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void View::EndDisplay() {
  glutSwapBuffers();
}

void View::DisplayCallback() {
  presenter_->DisplayCallback();
}

void View::KeyboardCallback(unsigned char key, int x, int y) {
  int special = glutGetModifiers();
  bool is_shift = special & GLUT_ACTIVE_SHIFT;
  bool is_ctrl = special & GLUT_ACTIVE_CTRL;
  bool is_alt = special & GLUT_ACTIVE_ALT;
  presenter_->KeyboardCallback(key, is_shift, is_ctrl, is_alt);
}

void View::SpecialKeyCallback(int key, int x, int y) {
}

void View::ReshapeCallback(int width, int height) {
}

void View::MouseCallback(int button, int state, int x, int y) {
}

}  // namespace click
