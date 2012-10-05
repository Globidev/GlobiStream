GlobiStream
------------------------

GlobiStream is a friendly user interface for the CLI program "Livestreamer"
that you can find here : https://github.com/chrippa/livestreamer

It is necessary to install Livestreamer before building Globistream.
You will also need Qt 4.8 to build the project.

GlobiStream connects to the server application which monitors a given list of streams, checking
every minute to see if they are online or not.

Installation
------------------------

- qmake
- make release

This should suffice to build GlobiStream.
You can find the dynamic dependencies in the "libs" folder.

![Graphical Interface](http://dl.dropbox.com/u/3427186/GlobiScreenShots/Screenshot-30.08.2012.21.59.59.814.png)