# Installing HALCoGen on Mac

HALCoGen is Texas Instruments' Hardware Abstraction Layer Code Generator, and is necessary for configuring and starting a project on the TI ARM chips

## Pre-reqs

You will need [homebrew](https://brew.sh/), a command line package installer for mac.

## Installing WINE

WINE (Wine is not an emulator) is the software that enables running this windows application on Mac. To install wine open a terminal.
```
brew cask install xquartz
brew cask install wine-stable
```

## Installing HALCoGen

Download the HALCoGen installer zip from [here](http://www.ti.com/tool/HALCOGEN). unzip the installer exe. Use wine to run the exe with a command like `wine ~/Downloads/HALCoGen-04.07.01-installer.exe`. Wine will first prompt you to install dependencies such as mono and gecko, say yes to each one. When the HALCoGen wizard asks where you would like to install it you **MUST** click the browse button and choose a new location (as C:\\ does not exist on your mac). Your Mac Root is mounted in wine as Z:\\, using that drive you can navigate to where you want to install. Note that the HALCoGen installer cannot make new folders, nor can you type in the destination textbox so I ended up using `mkdir -p` to create a HALCoGen folder within my user's application directory, then using the install GUI to install there.

Towards the end of the setup another dialog will come up for installing the SafeTI Diagnostic Libraries, you need these if you plan on compiling. When the installer lists its features for install check the box for the board specific libraries (you can un-check the boards you aren't using). Once again you will need to specify a valid install path.

## Booting HALCoGen

To boot HALCoGen you need to use the wine cli. Open a Terminal and pass the HALCoGen exe to wine with a command like `wine ~/Applications/ti/Hercules/HALCoGen/v04.07.01/HALCOGEN.exe`. Note the terminal cannot be closed or otherwise used while HALCoGen is open