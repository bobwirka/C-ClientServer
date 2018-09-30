# C++ Client/Server

Version 1.0 : 30-Sep-2018

Example C++ project that illustrates client server exchanges using AJAX with JSON data.

Client side code is written in TypeScript, and is the same as the Python and Node versions.

The server side uses two 3rd party libraries:
	mongoose:	provides web server (and web sockets!)
	jansson:	provides JSON decoding and encoding
	
Both libraries were cloned from github. Just copied mongoose.[c,h] to the project.
	
Note that jansson was converted from C to C++ to provide a "simplified" API. The conversion was
nothing more than a small modification to jansson.h and the inclusion of the new jansson.hpp.

The C files in the jansson library were renamed '.cpp', and the new header files (jansson.hpp and
jansson_private.h) replaced any other jansson header file references.

The CPP files were modified to include proper casting of 'malloc' so that the C++ compiler didn't choke.

Have compiled this in Linux, with the intention of cross compiling with mingw for Windows. The jansson code
has hooks for mingw. 

The client index.html page has 3 edit boxes that hold numbers to be sent to a 'calculate' function on
the server. There is a 'result' box, and a 'Calculate' button.

When the calculate button is pressed, the 3 values are collected into a JSON object and sent to the
server via AJAX. The server adds the 3 values together and returns the sum, which is displayed on
the web page.

Have added 'start.sh' script that will start the server and open a google chrome window that will
be resized when the application starts. Substitute chromium-browser for google-chrome-stable if
you're using chromium.

**BONUS FEATURE**

Added window resize to 'client/index.ts' so that if the browser is started as a new window in
application mode, the window will become smaller to fit the user interface. There will be no
tabs, bookmarks, etc.

Yes, it's then a Web App...

You can start Chrome or chromium from the command line to do this:
```sh
google-chrome-stable --new-window --app=http://localhost:8080/index.html
    --OR--
chromium-browser --new-window --app=http://localhost:8080/index.html
```
