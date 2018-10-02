# Testing JSROOT graphics in batch mode

Idea to check how current JSROOT graphics works in browser headless mode, using RWebWindow.
To run example:

    root.exe -l -b --web chrome testBatch.cxx+
    
It reads data from hsimple.root file, sends histogram to headless browser, plot it with lego2 options, and sends png image back. Actions async and takes few seconds.

Currently if JSROOT runs in batch mode, WebGL automatically deactivated - even if browser claims support of WebGL. 

    