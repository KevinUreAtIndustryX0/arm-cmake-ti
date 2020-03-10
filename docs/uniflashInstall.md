# Installing UniFlash
Get the UniFlash .dmg [here](http://www.ti.com/tool/UNIFLASH)

install program using install wizard. The app can't be found in spotlight, only the directory it is located in can. If
 you want to run the program you will need to navigate to the install directory. This is the default location 
 (/Applications/ti/uniflash_5.3.0).
 
 The flash script needs to know the directory where UniFlash is installed this is done by an environment variable named 
 UNIFLASH_PATH and should be set to your install directory. We recommend doing this in your bash profile.