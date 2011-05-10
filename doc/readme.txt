


                         {{NAME}} V{{VERSION}}



1. Motivation
2. How to install it
3. How to use it
4. What files are necessary
5. Some websites
6. Source code
7. Questions and Problems



1. Motivation
=========================
	BeOS lacked both, an Universal-Scroll option (as known from Logitechs MouseWare program for windows) and support for 
interclicks. {{NAME}} scratches both of those needs.


2. How to install it
=========================
	Just launch "install.sh". Everything will be done for you :-)
Or as an alternative do everything manually:
  * Copy/Move the UniversalScroller filter in an InputServer-AddOn-Filter Directory, and afterwards
  * restart the inputserver by "input_server -q"



3. How to use it
=========================
After installing {{NAME}}, open a file (which doesn't fit entirely on your screen) in StyledEdit.
Press and hold down the primary and secondary mouse button (For typical mice, this resolves to the left and right mouse buttons).
Now move your mouse.
Voila. You scroll by just moving your mouse. That's fast and effective. Even better than just the slow scrolling wheel.
Thats too slow? Just press your Control key and you scroll ten times as fast.

Hold down the shift key while using the scrool wheel on your mouse, to scroll horizontally.

Try launching the "Preferences" Application from your install directory.
Here you can specify when scrolling should be turned on. (Standard is all combinations with the middle button held down)
And of course a minimum Movement can be set for the scrolling.
Behind the Tab "Scroll-Speed" you can set up the speed factors for scrolling (holding down the control key switches into fast mode).
"Use below factors for scroll-wheels" is used to apply the factors not only for the universal scroll, but also for scrolling done with scrolling wheels.
The next Tab is Clicks. That's where you specify whats happenning when you press a button or perform interclicks (i.e.: holding down a button and clicking with another one meanwhile), But you are not bound to specifying values such as PRIMARY_MOUSE_BUTTON or PRIMARY_MOUSE_BUTTON_DOUBLE. No you may enter programs. "StyledEdit" for example to load the Editor StyledEdit. I for example use /boot/home/CL-Amp_v36_x86_R5/CL-Amp to quickly launch CL-Amp via an interclick.
You can also specify cut, copy, and paste (which are self-explaining)

Additionally you can turn off {{NAME}} by pressing the Option (Windows) key.




4. What files are necessary
=========================
"UniversalScroller" - The actual input server add-on
"readme.txt" - This readme file
"install.sh" - The install-script
"uninstall.sh" - The uninstall-script
"Preferences" - The utitlity to cusomize your {{NAME}}



5. Some websites
=========================

{{URL}}
 -- The homepage for {{NAME}}



6. Source code
=========================

The source code of the program is available at
{{URL}}



7. Questions and Problems
=========================

Questions and Problems concerning this program please to
{{EMAIL}}
