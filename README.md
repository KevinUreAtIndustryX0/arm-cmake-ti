# arm-cmake-ti

## Setup
[install the ARM TI toolchain](docs/toolchainInstall.md)

[install UniFlash](docs/uniflashInstall.md)

**NOTE:** Don't forget the environment variables UNIFLASH_PATH and 
TI_CGT_PATH

[install HALCoGen (Optional)](docs/HALCoGenOnMac.md)

## Running the Tests
`sh runTests.sh`

## How to Build
build: `sh buildARM.sh`

clean build: `sh buildARM.sh -c`

## How to Flash
`sh flashARM.sh`
 - Verify after flashing: `-v`
 - don't restart on flash: `-n`

## Helpful Links
If you want to know more about writing tests using gtest and FFF check out this
[repo](https://github.com/PillarTechnology/getting-started-tdd-in-c)

## Advanced
TODO: Replacing hal
TODO: switching ARM chips