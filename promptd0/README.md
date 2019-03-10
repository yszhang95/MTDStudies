1. fillMatchNtuple.C selects those matched and unswap D0 candidates, fillMatchSwapNtuple fill those swap candidates

2. mtdAcceptance.C 

3. invBetaPlot gives QA plots and study the RMS value of (1/beta - 1/beta_expected) vs P

4. calSig.C gives significance calculation

5. fillGen.C gives genPt.root, the histograms used for sig calculation

6. hyjetMass.C fill mass histograms used for sig calculation

7. promptD0mass.C fill mass histograms used for sig calculation

8. myAna.h stores consts used in each macro

# calculate significance vs y in bins of pt
1. fillPromptDMass fills Mass vs Pt and Y into a TH3F object, project it to z axis with different range of y and pT would give the yield.
2. fillHyJetsMass fills Mass vs Pt and Y into a TH3F object,
3. calSigAll.C calculate the significance, it project TH3F to z axis to get the yield
