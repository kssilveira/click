click
=====

control the mouse efficiently using the keyboard!

## Why?

for those who use vi/emacs, screen, vimperator, but still sometimes have to lose time taking the hands of the keyboard to click somewhere, now you can click efficiently anywhere using only the keyboard.

## How?

divinding the screen recursively so that it is possible to address any pixels with a logarithm number of keystrokes.

## Features

mouse movement, left/right/middle/double click, drag-and-drop.

## Installing

    $ sudo apt-get install freeglut3 freeglut3-dev binutils-gold libxtst-dev
    $ git clone git://github.com/kssilveira/click.git
    $ cd click && make

## Help

create a keyboard shortcut that executes click (System->Preferences->Keyboard Shortcuts)

commands:

- '1' to '9': move the mouse
- '0', 'z', space, backspace: go back one level
- enter: left click
- shift+enter: right click
- shift+alt+enter: middle click
- ctrl+shift+enter: left drag
- ctrl+alt+shift+enter: double click
- '!', '[', ']', esc: exit

## Branching Model

- [A successful Git branching model](http://nvie.com/posts/a-successful-git-branching-model/)
- [Understanding the Git Workflow](http://sandofsky.com/blog/git-workflow.html)

## Style

- [Google C++ Style Guide](http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml)
- [cpplint.py](http://google-styleguide.googlecode.com/svn/trunk/cpplint/cpplint.py)

## License

MIT License (MIT).
