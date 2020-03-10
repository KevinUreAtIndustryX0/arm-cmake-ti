# Installing the Toolchain
get the TI compiler [here](http://www.ti.com/tool/download/ARM-CGT-19)

install program using install wizard. The flash script needs to know the directory where the TI compiler is installed
this is done by an environment variable named TI_CGT_PATH and should be set to your install directory 
We recommend doing this in your bash profile. **NOTE:** this is the path to the root of the code generation tools not 
the bin folder. For example `TI_CGT_PATH=/Applications/ti/ti-cgt-arm_19.6.0.STS`