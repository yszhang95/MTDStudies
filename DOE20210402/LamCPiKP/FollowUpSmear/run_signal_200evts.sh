#!/usr/bin/bash
root -b <<- _EOF_
.L fillLamC3PMass_200evts.C+
fillLamC3PMass(false, 0.045f, 1.0f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass_200evts.C+
fillLamC3PMass(false, 0.045f, 1.3f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass_200evts.C+
fillLamC3PMass(false, 0.045f, 1.5f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass_200evts.C+
fillLamC3PMass(true, 0.050f, 1.0f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass_200evts.C+
fillLamC3PMass(true, 0.050f, 1.3f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass_200evts.C+
fillLamC3PMass(true, 0.050f, 1.5f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass_200evts.C+
fillLamC3PMass(true, 0.065f, 1.0f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass_200evts.C+
fillLamC3PMass(true, 0.065f, 1.3f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass_200evts.C+
fillLamC3PMass(true, 0.065f, 1.5f, false)
.q
_EOF_
root -b <<- _EOF_
.L fillLamC3PMass_200evts.C+
fillLamC3PMass(true, 0.065f, 1.0f, true)
.q
_EOF_
