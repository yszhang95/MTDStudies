#!/bin/bash
#fillHyJetsMass_0p5Layer.C
#fillHyJetsMass_1Layer.C
#fillHyJetsMass_1p5Layer.C
#fillPromptDMass_0p5Layer.C
#fillPromptDMass_1Layer.C
#fillPromptDMass_1p5Layer.C

#root -b <<- _EOF_
#.L fillHyJetsMass_0p5Layer.C+
#fillHyJetsMass("tdr.list", innerLost, true)
#.q
#_EOF_
#
#root -b <<- _EOF_
#.L fillHyJetsMass_0p5Layer.C+
#fillHyJetsMass("tdr.list", outerLost, true)
#.q
#_EOF_
#
#root -b <<- _EOF_
#.L fillHyJetsMass_0p5Layer.C+
#fillHyJetsMass("tdr.list", innerLost, false)
#.q
#_EOF_
#
#root -b <<- _EOF_
#.L fillHyJetsMass_0p5Layer.C+
#fillHyJetsMass("tdr.list", outerLost, false)
#.q
#_EOF_
#
#root -b <<- _EOF_
#.L fillHyJetsMass_1p5Layer.C+
#fillHyJetsMass("tdr.list", innerLost, true)
#.q
#_EOF_
#
#root -b <<- _EOF_
#.L fillHyJetsMass_1p5Layer.C+
#fillHyJetsMass("tdr.list", outerLost, true)
#.q
#_EOF_
#
#root -b <<- _EOF_
#.L fillHyJetsMass_1p5Layer.C+
#fillHyJetsMass("tdr.list", innerLost, false)
#.q
#_EOF_
#
#root -b <<- _EOF_
#.L fillHyJetsMass_1p5Layer.C+
#fillHyJetsMass("tdr.list", outerLost, false)
#.q
#_EOF_
#
#root -b <<- _EOF_
#.L fillHyJetsMass_1Layer.C+
#fillHyJetsMass("tdr.list", true)
#.q
#_EOF_
#
#root -b <<- _EOF_
#.L fillHyJetsMass_1Layer.C+
#fillHyJetsMass("tdr.list", false)
#.q
#_EOF_

root -b <<- _EOF_
.L fillPromptDMass_0p5Layer.C+
fillPromptDMass(innerLost, true)
.q
_EOF_

root -b <<- _EOF_
.L fillPromptDMass_0p5Layer.C+
fillPromptDMass(outerLost, true)
.q
_EOF_

root -b <<- _EOF_
.L fillPromptDMass_0p5Layer.C+
fillPromptDMass(innerLost, false)
.q
_EOF_

root -b <<- _EOF_
.L fillPromptDMass_0p5Layer.C+
fillPromptDMass(outerLost, false)
.q
_EOF_

root -b <<- _EOF_
.L fillPromptDMass_1p5Layer.C+
fillPromptDMass(innerLost, true)
.q
_EOF_

root -b <<- _EOF_
.L fillPromptDMass_1p5Layer.C+
fillPromptDMass(outerLost, true)
.q
_EOF_

root -b <<- _EOF_
.L fillPromptDMass_1p5Layer.C+
fillPromptDMass(innerLost, false)
.q
_EOF_

root -b <<- _EOF_
.L fillPromptDMass_1p5Layer.C+
fillPromptDMass(outerLost, false)
.q
_EOF_

root -b <<- _EOF_
.L fillPromptDMass_1Layer.C+
fillPromptDMass(true)
.q
_EOF_

root -b <<- _EOF_
.L fillPromptDMass_1Layer.C+
fillPromptDMass(false)
.q
_EOF_
