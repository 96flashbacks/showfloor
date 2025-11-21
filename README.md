# showfloor
![showfloorlogo](https://github.com/user-attachments/assets/255078c5-d31c-4284-a720-f1f2ddcefd4f)

## Introduction

**showfloor** is a reimplementation of [The Preservation Project](https://github.com/Sunlitspace542/tpp-mirror) (TPP) with improvements and details sourced from footage of the Shoshinkai 1995 demo. 

The goal is to carry on the hack’s legacy with greater accuracy to the source material while keeping the codebase clean and easy to modify.

## Installation

The build process is the same as the [SM64 Decomp's](https://github.com/n64decomp/sm64).
Follow their [README](https://github.com/n64decomp/sm64/blob/master/README.md#installation) for setup steps and prerequisites (dependencies, baserom, etc).

Clone and build:

```sh
git clone https://github.com/96flashbacks/showfloor.git
cd showfloor
make
```

### RSP Microcode

This project uses the same `GRUCODE` build variable as the SM64 Decomp, but with different options.

Default is `f3d_102695`.

- `f3d_102695`: gspFast3D from Ultra 64 OS release 102695 (U64 Aging Cartridge). Not HLE-compatible.
- `f3d_20E`: gspFast3D from OS 2.0E (bbgames' ostrees). HLE-compatible.

Build with a specific microcode:

```sh
make GRUCODE=f3d_20E
```

## Q&A

**Why reimplement TPP?**
- The original was cluttered with unused assets and built on an outdated SM64 Decomp refresh, making it harder to build and maintain. Starting fresh was cleaner and more practical.

**Can I use this as a base for another project?**
- Yes, but not recommended, as many vanilla assets were removed or edited, so you'd need to re-add them.

**Can I contribute to this repository?**
- Contrbutions are welcome. Please open an issue to report problems or suggest improvements, and feel free to submit pull requests.

**Was it worth it?**
- Yep.

For more questions, join our [Discord server](https://discord.gg/ynabeAS4eD).

## Credits

### '96flashbacks' Team

#### Main Programming
- stalechilibeans
- 1up Guy
- elinspectorardilla

#### Additional Programming
- ummheather
- iProgramInCpp

#### Assets
- Scraps
- ummheather
- 1up Guy
- stalechilibeans
- `K-Dubs΄

#### Research & Playtesting
- Scraps
- Sleekflash16
- duskewl/jadu
- `K-Dubs΄
  
### '96flashbacks' Server Contributors

#### Additional Programming
- Phil564
- Fluvian
- diesel64
- penguinlambz

#### Assets
- toonjoey
- Lurondor
- icehotlemonade
- Neverax
- Diamond Hero
- Holo
- sonicdude143

#### Music & Audio
- skibidigurt
- RetroDev
- emlimate
- XKoop7321

#### Miscellaneous Help
- pack.san
- Xiartic
- WooHale54
- timmy6259
- Zucchino
- xerox
  
#### Special Thanks
- na0micakes
- Matsilagi
- lunr
- CharD
- Tony (Hard4Games)
- aaali

### Original 'The Preservation Project' Team

#### Programming
- Fluvian
- Marionova
  
#### Assets
- EmmaNerd
- CDi-Fails
- Lurondor
- Robichu
- Zucchino
- Holo

#### Research & Playtesting
- Xiartic
- lunr
  
#### Original 'The Preservation Project' Contributors
- 1up Guy
- Guestpiki
- iProgramInCpp
- Toru the Red Fox
- CreamDream
- Diamond Hero

### Tools & Resources
- [Fast64](https://github.com/Fast-64/fast64)
- [SM64 Decomp](https://github.com/n64decomp/sm64)
- [64History’s Archives](https://archive.org/details/sm64-beta-content)
- [Dudaw’s Archives](https://archive.org/details/sm64brp_src_abandoned)
- [Roovahlees’s Archives](https://archive.org/details/@roovahlees)
- [VGM & Other Instrument Sources Spreadsheet](https://docs.google.com/spreadsheets/d/1JJBlHHDc65fhZmKUGLrDTLCm6rfUU83-kbuD8Y0zU0o/edit?gid=2047725819#gid=2047725819)
- [Retro Aesthetics’ Mario Series Matches](https://retroaesthetics.net/mario-series-matches/)
- [TCRF’s Page](https://tcrf.net/Prerelease:Super_Mario_64_(Nintendo_64)/Shoshinkai_1995_Demo)
- [Original TPP Source Code](https://github.com/Sunlitspace542/tpp-mirror)
