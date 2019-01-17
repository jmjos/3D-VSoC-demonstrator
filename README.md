# 3D VSoC Demonstrator

[![Build Status](https://travis-ci.com/jmjos/3D-VSoC-demonstrator.svg?branch=master)](https://travis-ci.com/jmjos/3D-VSoC-demonstrator)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/e7c97c7a500a4e2cbf211dd1c7712346)](https://www.codacy.com/app/jmjos/3D-VSoC-demonstrator?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=jmjos/3D-VSoC-demonstrator&amp;utm_campaign=Badge_Grade)
[![CodeFactor](https://www.codefactor.io/repository/github/jmjos/3d-vsoc-demonstrator/badge)](https://www.codefactor.io/repository/github/jmjos/3d-vsoc-demonstrator)

## Software

### Design
9 ADCs, 9 CPUs for Bayer filter

### Adresses

| adr | component   |
|-----|-------------|
| 0-8 | ADC ul to dr|
| 9-17| BAyer CPU ul to dr|

### Control
The first four fields of the control field refer to the window of the image, which is processed: field 0: xStart, 1: yStart, 2: xEnd, 3: yEnd
