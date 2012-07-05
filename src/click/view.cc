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
#include <X11/extensions/XTest.h>

#include <cassert>
#include <cctype>
#include <algorithm>

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

void View::PostRedisplay() {
  glutPostRedisplay();
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

void View::DrawSquare(int i, int j, int division,
                      int map_width, int map_height,
                      int dx, int dy, char letter) {
  int sx = map_width / division,
      sy = map_height / division,
      rx = i * sx,
      ry = j * sy;

  rx += dx;
  ry += dy;


  int color_index = 0;
#define COLORS_TOTAL 2
  double colors[COLORS_TOTAL][3] = { {1, 0, 0}, {0, 0, 1} };
  glColor3f(colors[color_index][0],
            colors[color_index][1],
            colors[color_index][2]);

  glLineWidth(3.0f);
  glBegin(GL_LINE_LOOP);
    glVertex2f(rx, ry);
    glVertex2f(rx, ry + sy);
    glVertex2f(rx + sx, ry + sy);
    glVertex2f(rx + sx, ry);
  glEnd();

  int s = std::max(sx, sy);
  int ssx = s / 3, ssy = s / 3;
  int rrx = rx + sx / 2 - ssx / 2, rry = ry + sy / 2 - ssy / 2;

  if (letter) {
    glPushMatrix();
      int coef = 2;
      glTranslatef(rrx + ssx / 4, rry + ssy / 4, 0);
      glScalef(map_width / static_cast<double>(coef * width()),
               map_height / static_cast<double>(coef * height()), 1.0f);
      glColor3f(colors[color_index][0],
                colors[color_index][1],
                colors[color_index][2]);
      glLineWidth(3.0f);
      char upper_letter = toupper(letter);
      glutStrokeCharacter(GLUT_STROKE_ROMAN, upper_letter);
    glPopMatrix();
  }
}

void View::MouseMove(int x, int y) {
  int window_pos_x = glutGet((GLenum)GLUT_WINDOW_X);
  int window_pos_y = glutGet((GLenum)GLUT_WINDOW_Y);
  x += window_pos_x;
  y += window_pos_y;
  const char* display_name = getenv("DISPLAY");
  Display* display = XOpenDisplay(display_name);
  int kScreenNumber = -1;
  int kDelay = 10;
  XTestFakeMotionEvent(display, kScreenNumber, x, y, kDelay);
  XFlush(display);
}

void View::MouseClick(bool is_shift, bool is_ctrl, bool is_alt) {
  const char* display_name = getenv("DISPLAY");
  Display* display = XOpenDisplay(display_name);
  glutHideWindow();
  enum { MOUSE_BUTTON_LEFT = 1, MOUSE_BUTTON_MIDDLE, MOUSE_BUTTON_RIGHT };
  int button = MOUSE_BUTTON_LEFT;
  if (is_ctrl && is_shift) {
    button = MOUSE_BUTTON_LEFT;
  } else if (is_shift && is_alt) {
    button = MOUSE_BUTTON_MIDDLE;
  } else if (is_shift) {
    button = MOUSE_BUTTON_RIGHT;
  } else if (is_alt) {
    button = MOUSE_BUTTON_LEFT;
  }
  int kDelay = 10;
  if (!(is_ctrl && is_alt) || (is_ctrl && is_alt && is_shift)) {
    const bool kIsPress = true;
    XTestFakeButtonEvent(display, button, kIsPress, kDelay);
  }
  if (!is_ctrl || (is_ctrl && is_alt)) {  // not drag-and-drop
    const bool kIsPress = false;
    XTestFakeButtonEvent(display, button, kIsPress, kDelay);
  }
  if (is_ctrl && is_alt && is_shift) {  // double click
    bool is_press = true;
    XTestFakeButtonEvent(display, button, is_press, kDelay);
    is_press = false;
    XTestFakeButtonEvent(display, button, is_press, kDelay);
  }
  XFlush(display);
  exit(0);
  glutShowWindow();
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
