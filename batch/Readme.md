# Testing JSROOT graphics in batch mode

Idea to check how current JSROOT graphics works in browser headless mode, using RWebWindow.
To run example:

    root.exe -l -b --web=chrome testBatch.cxx

If run in batch mode, headless browser will be started.
Means no window appears but just browser procvess will try to connect to RWebWindow instance.
Once connection established, ROOT reads hsimple.root file, sends histogram to headless browser.
Browser will produce PNG and SVG image with lego2 options, and sends images back.
Actions are async and takes few seconds.

