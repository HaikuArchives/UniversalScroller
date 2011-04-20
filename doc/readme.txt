


                         UniversalScroller 3.0                             



1. Why this program
2. How to install it
3. How to use it
4. What files are necessary
5. Some websites
6. Questions and Problems
F. History



1. Why this program
=========================
	Because I REALLY LOVE the Universal-Scroll option of Logitechs MouseWare program. And I missed this one for BeOS. So here is it.
regards to Fulop Balint who helped me eliminatig the "minimizing" bug


2. How to install it
=========================
	Just launch "install.sh". Everything will be done for you :-)  [Now as script ... no longer as real App]
Or as an alternative do everything manually [Copy/Move the UniversalScroller in a an InputServer-AddOn-Filter Directory and restart the inputserver by "input_server -q" :(. ] But the "Install" is definitely handier.



3. How to use it
=========================
After installing open a document which doesn't fit entirely on your screen.
Press and hold down the tertiary (normally middle) mousebutton.
Now move your mouse.
Voila. You scroll by just moving your mouse. That's fast and effective. Even better than just the slow scrolling wheel.
Thats too slow? Just press your Control key and you scroll ten times as fast?

Another funny thing?
scroll using your scroll wheel and hold down the shift key. Scrolling horzontally !!

Here you can specify when scrolling should be turned on. (Standard is all combinations with the middle button held down)
And of course a minimum Movement can be set for the scrolling.

Try launching the "Preferences" Application from your install directory.
Behind the Tab "Speed" you can set up the speed factors for scrolling (holding down the control key switches into fast mode).
"Use factors for scroll-wheels" is used to apply the factors not only for the universal scroll, but also for scrolling done with scrolling wheels.
The next Tab is Clicks. That's where you specify whats happenning when you press a button or perform interclicks (i.e.: holding down a button and clicking with another one meanwhile), But you are not bound to specifying values such as PRIMARY_MOUSE_BUTTON or PRIMARY_MOUSE_BUTTON_DOUBLE. No you may enter programs. "StyledEdit" for example to load the Editor StyledEdit. I for example use /boot/home/CL-Amp_v36_x86_R5/CL-Amp to quickly launch CL-Amp via an interclick.
You can also specify cut, copy, and paste (which are self-explaining)

Additionally you can turn off Universal Scroller by pressing the Option (Windows) key.




4. What files are necessary
=========================
"UniversalScroller" - The actual input server Add-on
"readme.txt" - The readme file
"install.sh" - The Install-Script
"uninstall.sh" - The Uninstall-Script
"Preferences" - The Utitlity to Cusomize your UniversalScroller



5. Some websites
=========================

http://come.to/Troublemaker
The Troublemaker Homepage



6. Questions and Problems
=========================

Questions and Problems concerning this program please to
Troublemaker@altavista.net



F. History
=========================
2001.02.13 - V3.1 - Fixed pass-through bug, adding option to swallow clicks
2001.02.02 - V3.0 - Fixed minimizing bug (finally :)) adding copy, cut, paste interclicks 
2000.09.13 - Completely rewrote the whole input-server addon.
2000.08.18 - Added "middle click as left double click"-option with temporary turn off by SHIFT keys
2000.08.17 - Added swap of x and y Scrolling Wheel via SHIFT KEY
2000.08.16 - Adding support for double clicking the middle mouse button via TERTIARY_MOUSE_BUTTON emulation (interclicking)
2000.07.27 - Added interclick support
2000.07.27 - Completely rewrote Preferences
2000.07.26 - Added passsing through of clicks just process drags
2000.07.21 - Added speedup of scrolling (CTRL-Key), minimum movement Preferences
2000.07.01 - Added Install
2000.06.28 - First Version

