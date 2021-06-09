#!/usr/bin/bash
root -b <<- _EOF_
.L fillLamC3PMass.C+
fillLamC3PMass(false, 0.045f, 1.0f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass.C+
fillLamC3PMass(false, 0.045f, 1.1f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass.C+
fillLamC3PMass(false, 0.045f, 1.2f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass.C+
fillLamC3PMass(false, 0.045f, 1.3f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass.C+
fillLamC3PMass(false, 0.045f, 1.4f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass.C+
fillLamC3PMass(false, 0.045f, 1.5f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass.C+
fillLamC3PMass(true, 0.050f, 1.0f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass.C+
fillLamC3PMass(true, 0.050f, 1.1f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass.C+
fillLamC3PMass(true, 0.050f, 1.2f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass.C+
fillLamC3PMass(true, 0.050f, 1.3f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass.C+
fillLamC3PMass(true, 0.050f, 1.4f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass.C+
fillLamC3PMass(true, 0.050f, 1.5f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass.C+
fillLamC3PMass(true, 0.065f, 1.0f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass.C+
fillLamC3PMass(true, 0.065f, 1.1f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass.C+
fillLamC3PMass(true, 0.065f, 1.2f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass.C+
fillLamC3PMass(true, 0.065f, 1.3f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass.C+
fillLamC3PMass(true, 0.065f, 1.4f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass.C+
fillLamC3PMass(true, 0.065f, 1.5f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass.C+
fillLamC3PMass(true, 0.065f, 1.0f, true)
.q
_EOF_
