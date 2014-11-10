`xvmouse`
=========

Synopsis
--------

`xvmouse` is a useful utility to allow you to control your mouse cursor
using your keyboard.  Combined with a window manager like `ion3`, it can
allow for an environment that's usable without a physical mouse attached to
your computer.

Usage
-----

Launch `xvmouse` from the command line, and background it. It should say
something like "grabbing keyboard" if things are going well, and then
nothing from then on.  To activate `xvmouse`, hold down `Alt` and `` ` ``
(backtick), and optionally release Alt.  While `` ` `` is held down, xvmouse
will still have control over your keyboard; release `` ` `` to give control
back to your system.

While xvmouse is active, use the keys `i`, `j`, `k`, and `l` to move the
cursor.  Hold down `;` and double-tap (or triple-tap, or quadruple-tap) any of
the movement keys to move the cursor rapidly (I like to call this feature
'tapjumping').  The left, right, and middle buttons are respectively bound
to `u`, `o` and `p`; pressing and releasing those keys is equivalent to pressing
and releasing their respective buttons on the mouse.

Use case
--------

Use it anywhere where you don't have a mouse easily accessible. On my
laptop, I still find the touchpad faster; but at work, where I keep my
keyboard in my lap, I use `xvmouse` exclusively to control my pointing
device.  (When used on a buckling-spring keyboard, the rapid tapping sounds
a lot like playing a video game ...)

Bugs / issues
-------------

`xvmouse` is customized for my needs, so it might not work perfectly for
you.  In fact, it was designed such that the only customizability is really
the source code (which is relatively short).  If you can think of something
that might make it better, let me know.

`xvmouse` polls the system for events every small number of milliseconds.
This might be fine on a desktop machine, but on a laptop, this might prevent
the CPU's power saving from kicking in.

Getting `xvmouse`
-----------------

`xvmouse` is now primarily available by GitHub, at
[https://github.com/jwise/xvmouse](https://github.com/jwise/xvmouse).

Contact
-------

Patches? Questions? Patches? Comments? Patches? Bug reports? Patches? 
Gripes?  Patches?  Send Joshua Wise an e-mail at
[joshua@joshuawise.com](mailto:joshua@joshuawise.com).
