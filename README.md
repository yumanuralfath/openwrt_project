# OpenWrt Application for MR3020

## Overview

This project contains a custom application built for the TP-Link MR3020 router running OpenWrt.  
It is designed to extend the functionality of the router with additional features or services.

## Features

- Lightweight and optimized for MR3020 hardware
- Compatible with OpenWrt 19.07.x
- Easy to build and deploy
- Supports basic configuration through LuCI or CLI

## Requirements

- TP-Link MR3020 with OpenWrt installed
- OpenWrt SDK or build system for compiling packages
- SSH access to the router

## Installation

### Build from source

```
1. Clone this repository:
   git clone https://github.com/yumanuralfath/openwrt_project.git
   cd openwrt_project

    Place your application in the package/ directory of the OpenWrt SDK.

    Build the package:

cd ~/openwrt-sdk-19.07.9
make package/my_package/compile V=s

Install the .ipk package on your MR3020:

    opkg install /path/to/my_package.ipk

Via SSH

scp my_package.ipk root@192.168.1.1:/tmp/
ssh root@192.168.1.1
opkg install /tmp/my_package.ipk

Usage

    Start or enable the service:

/etc/init.d/my_service start
/etc/init.d/my_service enable

Access logs:
logread -f
```
