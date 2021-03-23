import sys
sys.path.append('../../Utilities/Ana/')

import math
import calculator

def calSig(yMin, yMax, pTMin, pTMax, massMin, massMax, useMTD=True):
  calculator.setup()
  h3scale = calculator.scaleHist(5.6, 'CMS-D0.root', 'Table 3/Hist1D_y1', 'output/Ds_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root', 'hGenYVsPt', -1, 1.)
  hsignal = calculator.getSignal('output/Ds_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root', useMTD, h3scale)
  hsignal.Scale(0.35) # Ds to D0 ratio
  (hbackground, scale) = calculator.getBackground('output/HYDJETS_bcharged_ana_AllEntries_Ds_PhiPi_BottomToTop.root', useMTD)
  (s, serr) = calculator.getCounts(hsignal, yMin, yMax, pTMin, pTMax, massMin, massMax)
  (b, berr) = calculator.getCounts(hbackground, yMin, yMax, pTMin, pTMax, massMin, massMax)
  sig = s/math.sqrt(s+b)
  sigErr = sig* math.sqrt( (serr/s - 0.5 * serr/(s+b)) **2 + ( 0.5*berr/(s+b) )**2 )
  h3scale.Delete()
  hsignal.Delete()
  hbackground.Delete()
  print sig, sigErr, s, serr, b, berr, s/serr, b/berr
  return (s, serr, b, berr, sig, sigErr)

yMinEdges = [0.]
yMaxEdges = [1.]
ptMinEdges = [0, 2, 3., 4., 5., 6., 8., 10.]
ptMaxEdges = [0, 2, 3., 4., 5., 6., 8., 10.]
for yMin, yMax in zip(yMinEdges, yMaxEdges):
  for pTMin, pTMax in zip(ptMinEdges, ptMaxEdges):
    masslw = 1.95
    massup = 1.986
    calSig(yMin, yMax, pTMin, pTMax, masslw, massup)
    calSig(yMin, yMax, pTMin, pTMax, masslw, massup, False)
